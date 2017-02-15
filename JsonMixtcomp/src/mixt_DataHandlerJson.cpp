/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016
*/

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/


#include "mixt_DataHandlerJson.h"
#include "json.hpp"

namespace mixt {
DataHandlerJson::DataHandlerJson(const nlohmann::json resGetData_lm):
  nbVar_(0),
  nbInd_(0),
  dataList_(resGetData_lm) {}

void DataHandlerJson::writeInfo() const {
   for (std::map<std::string, std::string>::const_iterator it    = info_.begin(),
                                                           itEnd = info_.end();
        it != itEnd;
        ++it) {
     std::cout << "name: " << it->first << ", model: " << it->second << std::endl;
   }
}

std::string DataHandlerJson::listData() {
  bool LatentClassProvided = false;

  std::string warnLog;
  for (Index i = 0; i < dataList_.size(); ++i) {
     const nlohmann::json currList = dataList_[i];

     std::string model = currList["model"];
     std::string id = currList["id"];
     const nlohmann::json data = currList["data"];

     // add to info_, and perform various checks
     std::pair<std::map<std::string, std::string>::iterator, bool> ret; // parse descriptor file
     ret = info_.insert(std::pair<std::string, std::string>(id, model)); // check if identifier is already present
     if (ret.second == false) { // if name already exists, return a warning
       warnLog += std::string("Several variables bear the same name: ") + id + std::string(", while only a variable per name is allowed.\n");
     }
     if (data.size() == 0) {
       warnLog += std::string("Variable: ") + id + std::string(" has 0 samples.");
     }
     if (nbInd_ > 0 && nbInd_ != data.size()) {
       std::stringstream sstm;
       sstm << "Variable: " << id << " has " << data.size() << " individuals, while the previous variable had "
            << nbInd_ << " individuals. All variables must have the same number of individuals." << std::endl;
       warnLog += sstm.str();
     }

     nbInd_ = data.size(); // overwritten, because check has already been performed on the R side
     dataMap_[id] = i; // dataMap_[id] created if not already existing
     ++nbVar_;

     if (model == "LatentClass") {
       LatentClassProvided = true;
     }
   }

   if (!LatentClassProvided) {
     ++nbVar_; // albeit LatentClass has not been provided by the user, it is still a variable in the analysis
   }

  return warnLog;
}

void DataHandlerJson::writeDataMap() const {
  std::cout << "Position of data in input: \n";
  for (std::map<std::string, int> ::const_iterator it    = dataMap_.begin(),
                                                   itEnd = dataMap_.end();
       it != itEnd;
       ++it) {
    std::cout << "\tname: " << it->first << "\n";
    std::cout << "\t\trList_ position: " << it->second << std::endl;
  }
}


std::string DataHandlerJson::getData(std::string const& idData,
                                           Vector<std::string>& dataStr,
                                           Index& nbInd,
                                           std::string& paramStr) const {
  std::string warnLog;
  if (dataMap_.find(idData) != dataMap_.end()) { // check if the data requested is present in the input data
    Index pos = dataMap_.at(idData); // get the index of the element of the list corresponding to idData
    nbInd = nbInd_;
    dataStr.resize(nbInd_); // R has already enforced that all data has the same number of rows, and now all mixture are univariate

    const nlohmann::json  currVar = dataList_[pos]; // get current named list
    const nlohmann::json  data = currVar["data"];

    paramStr = currVar["paramStr"];

    for (int i = 0; i < nbInd_; ++i) {
      dataStr(i) = data[i].dump();
    }
  }
  else {
    std::stringstream sstm;
    sstm << "Data from the variable: " << idData << " has been requested but is absent from the provided data. "
         << "Please check that all the necessary data is provided." << std::endl;
    warnLog += sstm.str();
  }
  return warnLog;
}


nlohmann::json DataHandlerJson::jsonReturnType() const {

  nlohmann::json type;

  type["z_class"] = "LatentClass"; // z_class is always pushed first in the results

  for (std::map<std::string, std::string>::const_iterator it    = info_.begin(),
                                                          itEnd = info_.end();
             it != itEnd;
             ++it) {
    if (it->second != "LatentClass") { // z_class is ;always pushed first
      type[it->first] = it->second;
    }
  }
  return type;
}


} /* namespace mixt */
