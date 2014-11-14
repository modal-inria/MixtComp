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

namespace mixt
{

DataHandlerR::DataHandlerR() :
    nbSamples_(0),
    nbVariables_(0)
{}

DataHandlerR::~DataHandlerR()
{}

void DataHandlerR::writeInfo(std::ostream& os) const
{
  // show content
   for (InfoMap::const_iterator it = info_.begin(); it != info_.end(); ++it)
     os << "name: " << it->first << ", model: " << it->second << std::endl;
}

bool DataHandlerR::addInfo(std::string const& idData, std::string const& idModel)
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

bool DataHandlerR::listData(Rcpp::List rList)
{
  rList_ = rList;
  for (int i = 0; i < rList.size(); ++i)
  {
    Rcpp::List currList = rList_[i];

    std::string model = currList("model");
    std::string id = currList("id");
    Rcpp::CharacterVector data = currList["data"];

    nbSamples_ = data.size(); // overwritten, because check has already been performed on the R side
    addInfo(id, model);
    std::vector<int>& v_pos = dataMap_[id]; // dataMap_[id] created if not already existing
    v_pos.push_back(i);
    ++nbVariables_;
  #ifdef MC_DEBUG
    std::cout << "DataHandlerR::readDataFromRListHelper()" << std::endl;
    std::cout << "\tid: " << id << std::endl;
    std::cout << "\tmodel: " << model << std::endl;
    std::cout << "\trList pos: " << i << std::endl;
  #endif
  }
  return true;
}

void DataHandlerR::writeDataMap() const
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
