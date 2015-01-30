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
#include "../Various/mixt_Def.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

GaussianLikelihood::GaussianLikelihood(const Vector<Real>* p_param,
                                       const AugmentedData<Matrix<Real> >* augData,
                                       const Matrix<RowVector<Real> >* p_dataStatStorage,
                                       int nbClass) :
    p_param_(p_param),
    p_augData_(augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

GaussianLikelihood::~GaussianLikelihood()
{}

void GaussianLikelihood::lnCompletedLikelihood(Vector<Real>* lnComp, int k)
{
#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnCompletedLikelihood" << std::endl;
#endif
  // likelihood for present data
  for (int j = 0; j < p_augData_->data_.cols(); ++j)
  {
    for (int i = 0; i < p_augData_->data_.rows(); ++i)
    {
      if (p_augData_->misData_(i, j).first == present_)   // likelihood for present value
      {
        Real mean  = (*p_param_)(2 * k    , j);
        Real sd    = (*p_param_)(2 * k + 1, j);

        Real logProba = normal_.lpdf(p_augData_->data_(i, j),
                                     mean,
                                     sd);

        (*lnComp)(i) += logProba;
      }
      else // likelihood for missing values, imputation by the expectation
      {
        Real mean  = (*p_param_)(2 * k    , j);
        Real sd    = (*p_param_)(2 * k + 1, j);

        Real logProba = normal_.lpdf((*p_dataStatStorage_)(i, j)[0],
                                     mean,
                                     sd);

        (*lnComp)(i) += logProba;
      }
    }
  }
}

void GaussianLikelihood::lnObservedLikelihood(Vector<Real>* lnComp, int k)
{
#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnObservedLikelihood" << std::endl;
#endif
  // likelihood for present data
  for (int j = 0; j < p_augData_->data_.cols(); ++j)
  {
    for (int i = 0; i < p_augData_->data_.rows(); ++i)
    {
      Real mean  = (*p_param_)(2 * k    , j);
      Real sd    = (*p_param_)(2 * k + 1, j);

      Real logProba;

      switch(p_augData_->misData_(i, j).first)   // likelihood for present value
      {
        case present_:
        {
          Real mean  = (*p_param_)(2 * k    , j);
          Real sd    = (*p_param_)(2 * k + 1, j);

          logProba = normal_.lpdf(p_augData_->data_(i, j),
                                  mean,
                                  sd);
        }
        break;

        case missing_: // no contribution to the observed likelihood
        {
#ifdef MC_DEBUG
          std::cout << "\tmissing" << std::endl;
#endif
          logProba = 0.;
        }
        break;

        case missingIntervals_:
        {
          Real infBound  = p_augData_->misData_(i, j).second[0];
          Real supBound  = p_augData_->misData_(i, j).second[1];
#ifdef MC_DEBUG
      std::cout << "\tmissingIntervals_" << std::endl;
      std::cout << "\tleftBound: " << leftBound << "\tboost::math::cdf(norm, leftBound): " << boost::math::cdf(norm, leftBound) << std::endl;
      std::cout << "\trightBound: " << rightBound << "\tboost::math::cdf(norm, rightBound): " << boost::math::cdf(norm, rightBound) << std::endl;
#endif
          logProba = std::log(  normal_.cdf(supBound,
                                            mean,
                                            sd)
                              - normal_.cdf(infBound,
                                            mean,
                                            sd));
        }
        break;

        case missingLUIntervals_:
        {
#ifdef MC_DEBUG
      std::cout << "\tmissingLUIntervals_" << std::endl;
#endif
          Real supBound = p_augData_->misData_(i, j).second[0];

          logProba = std::log(normal_.cdf(supBound,
                                          mean,
                                          sd));
        }
        break;

        case missingRUIntervals_:
        {
#ifdef MC_DEBUG
      std::cout << "\tmissingRUIntervals_" << std::endl;
#endif
          Real infBound = p_augData_->misData_(i, j).second[0];


          logProba = std::log(1. - normal_.cdf(infBound,
                                               mean,
                                               sd));
        }
        break;

        default:
        {}
        break;
      }

      (*lnComp) += logProba;

#ifdef MC_DEBUG
      std::cout << "\tlogProba: " << logProba << std::endl;
#endif
    }
  }
}

} /* namespace mixt */
