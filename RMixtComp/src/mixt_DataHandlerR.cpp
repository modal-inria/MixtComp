/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 */

/*
 * Project:  MixtComp
 * created on: Nov 21, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_DataHandlerR.cpp
 *  @brief In this file 
 **/

#include "mixt_DataHandlerR.h"

namespace mixt
{

DataHandlerR::DataHandlerR()
{
  // TODO Auto-generated constructor stub
}

DataHandlerR::~DataHandlerR()
{
  // TODO Auto-generated destructor stub
}

bool DataHandlerR::readDataFromRList(Rcpp::List rList)
{
  for (int i = 0; i < rList.size(); ++i)
  {
    Rcpp::S4 s4 = rList[i];
    std::string modelname = s4.slot("model");
    Rcpp::NumericMatrix nm = s4.slot("data");

    nbSamples_ = nm.nrow(); // overwritten, because check has already been performed on the R side
    for (int j = 0; j < nm.ncol(); ++j)
    {
      std::string id;
      addInfo(id,modelname);
      std::vector<DataPos>& v_pos = dataMap_[id];
      v_pos.push_back(DataPos(i,j));
      ++nbVariables_;
    }
  }
  return true;
}

void DataHandlerR::getData(std::string const& idData, STK::Array2D<int>& data, int& nbVariable) const
{

}

void DataHandlerR::getData(std::string const& idData, STK::Array2D<STK::Real>& data, int& nbVariable) const
{

}

void DataHandlerR::getData(std::string const& idData, STK::Array2D<std::string>& data, int& nbVariable) const
{

}

} /* namespace mixt */
