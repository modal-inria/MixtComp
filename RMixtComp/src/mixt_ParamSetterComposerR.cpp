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

#include <Rcpp.h>
#include "mixt_ParamSetterComposerR.h"

namespace mixt
{

ParamSetterComposerR::ParamSetterComposerR(Rcpp::NumericVector prop) :
    prop_(prop)
{}

ParamSetterComposerR::~ParamSetterComposerR()
{}

STK::Array2DVector<STK::Real> ParamSetterComposerR::getProportions() const
{
  STK::Array2DVector<STK::Real> prop(prop_.size(),
                                   0.);
  for (int i = 0; i < prop_.size(); ++i)
  {
    prop[i] = prop_[i];
  }
  return prop;
}

} // namespace mixt
