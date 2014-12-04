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

#ifndef MIXT_PARAMSETTERCOMPOSERR_H
#define MIXT_PARAMSETTERCOMPOSERR_H

#include "Arrays/include/STK_CArrayPoint.h"

namespace mixt
{

class ParamSetterComposerR
{
  public:
    ParamSetterComposerR(Rcpp::NumericVector prop);
    ~ParamSetterComposerR();
    STK::CArrayPoint<STK::Real> getProportions() const;
  private:
    Rcpp::NumericVector prop_;
};

} // namespace mixt

#endif // MIXT_PARAMSETTERCOMPOSERR_H
