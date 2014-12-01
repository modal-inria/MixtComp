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
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ParamExtractorR.h"

namespace mixt
{

ParamExtractorR::ParamExtractorR()
{}

ParamExtractorR::~ParamExtractorR()
{}

void ParamExtractorR::exportParam(std::string idName,
                                  const STK::Array2D<STK::Real>* p_param)
{
  Rcpp::NumericMatrix paramR(p_param->sizeRows(), p_param->sizeCols());

  for (int i = 0; i < p_param->sizeRows(); ++i)
    for (int j = 0; j < p_param->sizeCols(); ++j)
      paramR(i, j) = p_param->elt(i, j);

  param_[idName] = paramR;

#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::exportParam, param_.size():  " << param_.size() << std::endl;
#endif
}

Rcpp::List ParamExtractorR::rcppReturnParam() const
{
  return param_;
}

} // namespace mixt
