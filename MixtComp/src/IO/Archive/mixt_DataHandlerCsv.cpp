/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_DataHandlerCsv.h"

namespace mixt
{

DataHandlerCsv::DataHandlerCsv(std::string dataName,
                               std::string descName) :
    dataContent_(dataName,
                 false,
                 ";"),
    descContent_(descName,
                 false,
                 ";"),
    nbSamples_(0),
    nbVariables_(0)
{
  dataContent_.read();
  descContent_.read();
}

DataHandlerCsv::~DataHandlerCsv()
{}

void DataHandlerCsv::writeInfo(std::ostream& os) const
{
  // show content
   for (InfoMap::const_iterator it = info_.begin(); it != info_.end(); ++it)
     os << "name: " << it->first << ", model: " << it->second << std::endl;
}

bool DataHandlerCsv::addInfo(std::string const& idData, std::string const& idModel)
{
  // parse descriptor file
  std::pair<InfoMap::iterator,bool> ret;
  // check if identifier is already present
  ret = info_.insert(std::pair<std::string,std::string>(idData, idModel));
  // if name already exists, check if there is incoherence
  if (ret.second==false)
  {
     if (ret.first->second != idModel)
     {
#ifdef STK_DMANAGER_VERBOSE
       stk_cerr << _T("Incoherence in addInfo.\n");
#endif
       return false;
     }
  }
  return true;
}

bool DataHandlerCsv::listData()
{
  for (int j = 0; j < descContent_.sizeCol(); ++j)
  {
    std::string model = descContent_(0, j);
    std::string id    = descContent_(1, j);

    nbSamples_ = dataContent_.rows(0) - 1; // all columns are supposed to have the same number of rows, add check
    addInfo(id, model);
    std::vector<int>& v_pos = dataMap_[id]; // dataMap_[id] created if not already existing
    v_pos.push_back(j);
    ++nbVariables_;
  #ifdef MC_DEBUG
    std::cout << "DataHandlerCsv::readDataFromRListHelper()" << std::endl;
    std::cout << "\tid: " << id << std::endl;
    std::cout << "\tmodel: " << model << std::endl;
    std::cout << "\trList pos: " << j << std::endl;
  #endif
  }
  return true;
}

void DataHandlerCsv::writeDataMap() const
{
  std::cout << "Position of data in input: \n";
  for (DataMap::const_iterator it_id = dataMap_.begin(); it_id != dataMap_.end(); ++it_id)
  {
    std::cout << "\tname: " << (*it_id).first << "\n";
    for (std::vector<int>::const_iterator it_dp = (*it_id).second.begin(); it_dp != (*it_id).second.end(); ++it_dp)
      std::cout << "\t\trList_ position: " << (*it_dp) << std::endl;
  }
}

} /* namespace mixt */
