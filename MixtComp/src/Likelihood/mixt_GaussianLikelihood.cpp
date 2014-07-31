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
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianLikelihood.h"
#include "STatistiK/include/STK_Law_Normal.h"

namespace mixt
{

GaussianLikelihood::GaussianLikelihood(const STK::Array2D<STK::Real>* p_param,
                                       const AugmentedData<STK::Array2D<STK::Real> >* augData) :
    p_param_(p_param),
    p_augData_(augData)
{}

GaussianLikelihood::~GaussianLikelihood()
{}

void GaussianLikelihood::lnLikelihood(STK::Array2DVector* lnComp, int k)
{
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
    {
      STK::Real mean  = p_param_->elt(2*k    , j);
      STK::Real sd    = p_param_->elt(2*k + 1, j);

      if (p_augData_->data_(i, j) != STK::Arithmetic<STK::Real>::NA())   // likelihood for present value
      {
        lnComp->elt(i) += STK::Law::Normal::lpdf(p_augData_->data_(i, j),
                                                 mean,
                                                 sd);
      }
    }
  }

  // partially observed data, missing intervals
  for (iv_missingIntervals it = p_augData_->v_missingIntervals_.begin();
       it != p_augData_->v_missingIntervals_.end();
       ++it)
  {
    for (int k = 0; k < p_prop_->sizeCols(); ++k)
    {
      int i = it->first.first;
      int j = it->first.second;

      STK::Real mean  = p_param_->elt(2*k    , j);
      STK::Real sd    = p_param_->elt(2*k + 1, j);

      STK::Law::Normal normal(mean, sd);

      lnComp->elt(i) += std::log(normal.cdf(it->second.second) -
                                 normal.cdf(it->second.first ));
    }
  }

  // partially observed data, missing left unbounded interval
  for (iv_missingLUIntervals it = p_augData_->v_missingLUIntervals_.begin();
       it != p_augData_->v_missingLUIntervals_.end();
       ++it)
  {
    for (int k = 0; k < p_prop_->sizeCols(); ++k)
    {
      int i = it->first.first;
      int j = it->first.second;

      STK::Real mean  = p_param_->elt(2*k    , j);
      STK::Real sd    = p_param_->elt(2*k + 1, j);

      STK::Law::Normal normal(mean, sd);

      lnComp->elt(i) += std::log(1. - normal.cdf(it->second));
    }
  }

  // partially observed data, missing right unbounded interval
  for (iv_missingLUIntervals it = p_augData_->v_missingRUIntervals_.begin();
       it != p_augData_->v_missingRUIntervals_.end();
       ++it)
  {
    for (int k = 0; k < p_prop_->sizeCols(); ++k)
    {
      int i = it->first.first;
      int j = it->first.second;

      STK::Real mean  = p_param_->elt(2*k    , j);
      STK::Real sd    = p_param_->elt(2*k + 1, j);

      STK::Law::Normal normal(mean, sd);

      lnComp->elt(i) += std::log(normal.cdf(it->second));
    }
  }
}

} /* namespace mixt */
