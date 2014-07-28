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

GaussianLikelihood::GaussianLikelihood(std::string idName,
                                       const STK::IDataHandler* handler,
                                       const STK::CArrayPoint<STK::Real>* p_prop,
                                       const STK::Array2D<STK::Real>* p_param,
                                       const AugmentedData<STK::Array2D<STK::Real> >* augData) :
    p_prop_(p_prop),
    p_param_(p_param),
    p_augData_(augData)
{
  int dummyVar;
  // fill presentData_ with the original data set containing NA information
  handler->getData(idName,
                   presentData_,
                   dummyVar);
}

GaussianLikelihood::~GaussianLikelihood()
{

}

STK::Real GaussianLikelihood::lnLikelihood()
{
  STK::Real lnLikelihood = 0.;
  STK::Array2D<STK::Real> lnComp(presentData_.sizeRows(),
                                 p_prop_->sizeCols(),
                                 0.);

  // likelihood for present data
  for (int i = 0; i < presentData_.sizeRows(); ++i)
  {
    for (int k = 0; k < p_prop_->sizeCols(); ++k)
    {
      for (int j = 0; j < presentData_.sizeCols(); ++j)
      {
        STK::Real mean  = p_param_->elt(2*k    , j);
        STK::Real sd    = p_param_->elt(2*k + 1, j);

        if (presentData_(i, j) != STK::Arithmetic<STK::Real>::NA())   // likelihood for present value
        {
          lnComp(i, k) += STK::Law::Normal::lpdf(presentData_(i, j),
                                                 mean,
                                                 sd);
        }
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

      lnComp(i, k) += std::log(normal.cdf(it->second.second) -
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

      lnComp(i, k) += std::log(1. - normal.cdf(it->second));
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

      lnComp(i, k) += std::log(normal.cdf(it->second));
    }
  }

  // Compute the likelihood for each sample, using the mixture model
  for (int i = 0; i < presentData_.sizeRows(); ++i)
  {
    STK::Real max = lnComp.row(i).maxElt();
    STK::Real sum = (lnComp.row(i) -= max).exp().dot(*p_prop_);
    lnLikelihood += max + std::log(sum);
  }

  return lnLikelihood;
}

} /* namespace mixt */
