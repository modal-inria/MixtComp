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
 *  Created on: December 9, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_PoissonLikelihood.h"
#include "../Various/mixt_Def.h"
#include <boost/math/distributions/poisson.hpp>

namespace mixt
{

PoissonLikelihood::PoissonLikelihood(const STK::Array2D<STK::Real>* p_param,
                                     const AugmentedData<STK::Array2D<int> >* augData,
                                     const STK::Array2D<STK::Array2DPoint<int> >* p_dataStatStorage) :
    p_param_(p_param),
    p_augData_(augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

PoissonLikelihood::~PoissonLikelihood()
{}

void PoissonLikelihood::lnCompletedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
#ifdef MC_DEBUG
   std::cout << "PoissonLikelihood::lnCompletedLikelihood" << std::endl;
#endif
  // likelihood for present data

  STK::Real mean  = p_param_->elt(k, 0);
  boost::math::poisson pois(mean);
  STK::Real proba;

  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    if (p_augData_->misData_(i, 0).first == present_)   // likelihood for present value
    {
      proba = boost::math::pdf(pois,
                               p_augData_->data_(i, 0));
    }
    else // likelihood for missing values, imputation by the expectation (temporary placeholder ...)
    {
      proba = boost::math::pdf(pois,
                               p_dataStatStorage_->elt(i, 0)[0]);
    }
    lnComp->elt(i) += std::log(proba);
  }
}

void PoissonLikelihood::lnObservedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
#ifdef MC_DEBUG
      std::cout << "PoissonLikelihood::lnObservedLikelihood" << std::endl;
#endif
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeCols(); ++i)
  {
    STK::Real mean = p_param_->elt(k, 0);
    boost::math::poisson pois(mean);

    STK::Real proba;

    switch(p_augData_->misData_(i, 0).first)   // likelihood for present value
    {
      case present_:
      {
        proba = boost::math::pdf(pois,
                                 p_augData_->data_(i, 0));
      }
      break;

      case missing_: // no contribution to the observed likelihood
      {
#ifdef MC_DEBUG
        std::cout << "\tmissing" << std::endl;
#endif
      }
      break;

      default:
      {}
      break;
    }
    lnComp->elt(i) += std::log(proba);

#ifdef MC_DEBUG
    std::cout << "\tproba: " << proba << std::endl;
#endif
  }
}

} /* namespace mixt */
