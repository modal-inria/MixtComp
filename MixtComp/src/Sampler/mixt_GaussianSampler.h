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
 *  Created on: 14 feb. 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_GAUSSIANSAMPLER_H
#define MIXT_GAUSSIANSAMPLER_H

#include "stkpp/projects/Arrays/include/STK_Array2D.h"

#include "stkpp/projects/STatistiK/include/STK_Law_Exponential.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Uniform.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Normal.h"

#include "../Data/mixt_AugmentedData.h"
#include "mixt_GaussianSamplerIterator.h"

namespace mixt
{

class GaussianSampler
{
  public:
    GaussianSampler(const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij,
                    const STK::Array2D<STK::Real>* p_param,
                    const STK::CArrayVector<int>* p_zi);
    GaussianSampler(const GaussianSampler& sampler);
    ~GaussianSampler();
    GaussianSamplerIterator begin();
    GaussianSamplerIterator end();
  private:
    const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij_;
    const STK::Array2D<STK::Real>* p_param_;
    const STK::CArrayVector<int>* p_zi_;
};

} // namespace mixt

#endif /* MIXT_GAUSSIANSAMPLER_H */
