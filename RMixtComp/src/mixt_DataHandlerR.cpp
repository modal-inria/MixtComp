/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Nov 21, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_DataHandlerR.cpp
 *  @brief In this file 
 **/

#include "mixt_DataHandlerR.h"

namespace mixt {

DataHandlerR::DataHandlerR(Rcpp::List rList) :
    nbInd_(0),
    nbVar_(0),
    rList_(rList)
{}

void DataHandlerR::writeInfo() const {
   for (std::map<std::string, std::string>::const_iterator it    = info_.begin(),
                                                           itEnd = info_.end();
        it != itEnd;
        ++it) {
     std::cout << "name: " << it->first << ", model: " << it->second << std::endl;
   }
}

std::string DataHandlerR::listData() {
  bool LatentClassProvided = false;

  std::string warnLog;
  for (Index i = 0; i < rList_.size(); ++i) {
    Rcpp::List currList = rList_[i];

    std::string model = currList["model"];
    std::string id = currList["id"];
    Rcpp::CharacterVector data = currList["data"];

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

void DataHandlerR::writeDataMap() const {
#ifdef MC_VERBOSE
  std::cout << "Position of data in input: \n";
  for (std::map<std::string, int> ::const_iterator it    = dataMap_.begin(),
                                                   itEnd = dataMap_.end();
       it != itEnd;
       ++it) {
    std::cout << "\tname: " << it->first << "\n";
    std::cout << "\t\trList_ position: " << it->second << std::endl;
  }
#endif
}

std::string DataHandlerR::getData(std::string const& idData,
                                  Vector<std::string>& dataStr,
                                  Index& nbInd,
                                  std::string& paramStr) const {
  std::string warnLog;
  if (dataMap_.find(idData) != dataMap_.end()) { // check if the data requested is present in the input data
    Index pos = dataMap_.at(idData); // get the index of the element of the rList_ corresponding to idData
    nbInd = nbInd_;
    dataStr.resize(nbInd_); // R has already enforced that all data has the same number of rows, and now all mixture are univariate

    Rcpp::List currVar = rList_[pos]; // get current named list
    Rcpp::CharacterVector data = currVar("data");

    paramStr = Rcpp::as<std::string>(currVar("paramStr"));

    for (int i = 0; i < nbInd_; ++i) {
      dataStr(i) = data[i];
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

Rcpp::List DataHandlerR::rcppReturnType() const {
  std::vector<std::string> listName;
  std::vector<std::string> listType;
  listName.reserve(nbVar_);
  listType.reserve(nbVar_);

  listName.push_back("z_class"); // z_class is always pushed first in the results
  listType.push_back("LatentClass");

  for (std::map<std::string, std::string>::const_iterator it    = info_.begin(),
                                                          itEnd = info_.end();
             it != itEnd;
             ++it) {
    if (it->second != "LatentClass") { // z_class is always pushed first
      listName.push_back(it->first);
      listType.push_back(it->second);
    }
  }

  Rcpp::CharacterVector name = Rcpp::wrap(listName);
  Rcpp::List type = Rcpp::wrap(listType);
  type.attr("names") = name;

  return type;
}

} /* namespace mixt */
