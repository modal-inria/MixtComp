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
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_REMOVEMISSING_H
#define MIXT_REMOVEMISSING_H

#include "../../Data/mixt_AugmentedData.h"
#include "Arrays/include/STK_Array2D.h"
#include "STatistiK/include/STK_Law_Uniform.h"
#include "STatistiK/include/STK_Law_Categorical.h"

namespace mixt
{

/** Utility function to lookup the data set and remove missing values
 *  coordinates. */
void removeMissing(AugmentedData<STK::Array2D<STK::Real> >& m_augDataij);

void removeMissing(AugmentedData<STK::Array2D<STK::Real> >& m_augDataij,
                   const STK::Array2DVector<bool>* proba);

void removeMissing(AugmentedData<STK::Array2D<int> >& m_augDataij);

void removeMissing(AugmentedData<STK::Array2D<int> >& m_augDataij,
                   const STK::Array2DVector<bool>* proba);

} /* namespace mixt */

#endif /* MIXT_REMOVEMISSING_H */
