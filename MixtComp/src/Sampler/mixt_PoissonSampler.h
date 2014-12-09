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
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_POISSONSAMPLER_H
#define MIXT_POISSONSAMPLER_H

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_CArrayVector.h"
#include "../Data/mixt_AugmentedData.h"

namespace mixt
{

class PoissonSampler
{
  public:
    PoissonSampler(AugmentedData<STK::Array2D<int> >* p_augData,
                   const STK::Array2D<STK::Real>* p_param);
    ~PoissonSampler();
    /** Sample new values for the missing variables of the given individual */
    void sampleIndividual(int i, int z_i);
  private:
    AugmentedData<STK::Array2D<int> >* p_augData_;
    const STK::Array2D<STK::Real>* p_param_;
    const STK::CArrayVector<int>* p_zi_;
};

} // namespace mixt

#endif // MIXT_POISSONSAMPLER_H
