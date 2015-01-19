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
 *  Created on: Nov 19, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ParamSetterR.h"

namespace mixt
{

ParamSetterR::ParamSetterR(const Rcpp::List param) :
    param_(param)
{}

ParamSetterR::~ParamSetterR()
{}

void ParamSetterR::getParam(std::string idName,
                            STK::Array2DVector<STK::Real>& param) const
{
  Rcpp::List logStat = param_[idName];
  Rcpp::NumericMatrix currParam = logStat["stat"];
  int nRows = currParam.nrow();
  int nCols = currParam.ncol();
  int nVars = nCols / 3; // because two out of three columns contain confidence intervals
  param.resize(nRows,
               nVars);

  for (int i = 0; i < nRows; ++i)
  {
    for (int j = 0; j < nVars; ++j)
    {
      param(i, j) = currParam(i, j * 3);
    }
  }

#ifdef MC_DEBUG
  std::cout << "ParamSetterR::getParam, idName: " << idName << std::endl;
  std::cout << "param:" << std::endl;
  std::cout << param << std::endl;
#endif
}

} // namespace mixt
