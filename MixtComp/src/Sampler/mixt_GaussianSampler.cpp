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
 *  Created on: Feb 18, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianSamplerIterator.h"
#include "mixt_GaussianSampler.h"

namespace mixt
{
GaussianSampler::GaussianSampler(const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij,
                                 const STK::Array2D<STK::Real>* p_param) :
    pm_augDataij_(pm_augDataij),
    p_param_(p_param)
{}

GaussianSampler::GaussianSampler(const GaussianSampler& sampler) :
    pm_augDataij_(sampler.pm_augDataij_),
    p_param_(sampler.p_param_),
    p_zi_(sampler.p_zi_)
{}

GaussianSampler::~GaussianSampler()
{}

GaussianSamplerIterator GaussianSampler::begin()
{
  GaussianSamplerIterator iterator(p_param_,
                                   p_zi_,
                                   pm_augDataij_->v_missing_.begin(),
                                   pm_augDataij_->v_missing_.end(),
                                   pm_augDataij_->v_missingFiniteValues_.begin(),
                                   pm_augDataij_->v_missingFiniteValues_.end(),
                                   pm_augDataij_->v_missingIntervals_.begin(),
                                   pm_augDataij_->v_missingIntervals_.end(),
                                   pm_augDataij_->v_missingLUIntervals_.begin(),
                                   pm_augDataij_->v_missingLUIntervals_.end(),
                                   pm_augDataij_->v_missingRUIntervals_.begin(),
                                   pm_augDataij_->v_missingRUIntervals_.end());
  return iterator;
}
GaussianSamplerIterator GaussianSampler::end()
{
  GaussianSamplerIterator iterator(p_param_,
                                   p_zi_,
                                   pm_augDataij_->v_missing_.end(),
                                   pm_augDataij_->v_missing_.end(),
                                   pm_augDataij_->v_missingFiniteValues_.end(),
                                   pm_augDataij_->v_missingFiniteValues_.end(),
                                   pm_augDataij_->v_missingIntervals_.end(),
                                   pm_augDataij_->v_missingIntervals_.end(),
                                   pm_augDataij_->v_missingLUIntervals_.end(),
                                   pm_augDataij_->v_missingLUIntervals_.end(),
                                   pm_augDataij_->v_missingRUIntervals_.end(),
                                   pm_augDataij_->v_missingRUIntervals_.end());
  return iterator;
}

void GaussianSampler::setZi(const STK::CArrayVector<int>* p_zi)
{
  p_zi_ = p_zi;
}
} // namespace mixt
