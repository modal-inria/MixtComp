/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
