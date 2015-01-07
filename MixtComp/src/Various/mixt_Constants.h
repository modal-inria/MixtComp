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
 *  Created on: August 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_CONSTANTS_H_
#define MIXT_CONSTANTS_H_

#include "STKernel/include/STK_Real.h"

namespace mixt
{

extern const STK::Real minStat; // threshold to have equiprobabilities in categorical sampler
extern const STK::Real probaNonMisClass; // minimal in-sample probability to avoid rejection
extern const int moduloMisClass; // modulo used to trigger misclassification step
extern const int minIndPerClass; // minimal number of individual in a class to accept a z sampling
extern const STK::Real epsilon; // very small value of real to check for near zero values
extern const int minModality; // minimal modality for categorical models

} // namespace mixt

#endif /* MIXT_CONSTANTS_H_ */
