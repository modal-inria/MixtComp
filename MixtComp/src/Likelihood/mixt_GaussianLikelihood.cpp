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
    nbClass_(nbClass),
    p_param_(p_param),
    p_augData_(augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

GaussianLikelihood::~GaussianLikelihood()
{}

void GaussianLikelihood::lnCompletedLikelihood(Matrix<Real>* lnComp)
{
#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnCompletedLikelihood" << std::endl;
#endif
  // likelihood for present data
  for (int k = 0; k < nbClass_; ++k)
  {
    for (int i = 0; i < p_augData_->data_.rows(); ++i)
    {
      if (p_augData_->misData_(i, 0).first == present_)   // likelihood for present value
      {
        Real mean  = (*p_param_)(2 * k    );
        Real sd    = (*p_param_)(2 * k + 1);

        Real logProba = normal_.lpdf(p_augData_->data_(i, 0),
                                     mean,
                                     sd);

        (*lnComp)(i, k) += logProba;
      }
      else // likelihood for missing values, imputation by the expectation
      {
        Real mean  = (*p_param_)(2 * k    );
        Real sd    = (*p_param_)(2 * k + 1);

        Real logProba = normal_.lpdf((*p_dataStatStorage_)(i, 0)[0],
                                      mean,
                                      sd);

        (*lnComp)(i, k) += logProba;
      }
    }
  }
}

void GaussianLikelihood::lnObservedLikelihood(Matrix<Real>* lnComp)
{
#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnObservedLikelihood" << std::endl;
#endif
  // likelihood for present data
  for (int k = 0; k < nbClass_; ++k)
  {
    for (int i = 0; i < p_augData_->data_.rows(); ++i)
    {
      Real mean  = (*p_param_)(2 * k    );
      Real sd    = (*p_param_)(2 * k + 1);

#ifdef MC_DEBUG
      std::cout << "i: " << i << std::endl;
      std::cout << "\tmean: " << mean << std::endl;
      std::cout << "\tsd: " << sd << std::endl;
#endif
      Real logProba;

      switch(p_augData_->misData_(i, 0).first)   // likelihood for present value
      {
        case present_:
        {
          Real mean  = (*p_param_)(2 * k    );
          Real sd    = (*p_param_)(2 * k + 1);

          logProba = normal_.lpdf(p_augData_->data_(i, 0),
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
          Real infBound  = p_augData_->misData_(i, 0).second[0];
          Real supBound  = p_augData_->misData_(i, 0).second[1];
          Real infCdf = normal_.cdf(infBound,
                                    mean,
                                    sd);
          Real supCdf = normal_.cdf(supBound,
                                    mean,
                                    sd);
#ifdef MC_DEBUG
          std::cout << "\tmissingIntervals_" << std::endl;
          std::cout << "\tinfBound: " << infBound << "\tsupBound: " << supBound << std::endl;
          std::cout << "\tinfCdf: " << infCdf << "\tsupCdf: " << supCdf << std::endl;
#endif
          logProba = std::log(supCdf - infCdf);
        }
        break;

        case missingLUIntervals_:
        {
          Real supBound = p_augData_->misData_(i, 0).second[0];
          Real supCdf = normal_.cdf(supBound,
                                    mean,
                                    sd);
#ifdef MC_DEBUG
          std::cout << "\tmissingLUIntervals_" << std::endl;
          std::cout << "\tsupBound: " << supBound << std::endl;
          std::cout << "\tsupCdf: " << supCdf << std::endl;
#endif
          logProba = std::log(supCdf);
        }
        break;

        case missingRUIntervals_:
        {
          Real infBound = p_augData_->misData_(i, 0).second[0];
          Real infCdf = normal_.cdf(infBound,
                                    mean,
                                    sd);
#ifdef MC_DEBUG
          std::cout << "\tmissingRUIntervals_" << std::endl;
          std::cout << "\tinfBound: " << infBound << std::endl;
          std::cout << "\tinfCdf: " << infCdf << std::endl;
#endif
          logProba = std::log(1. - infCdf);
        }
        break;

        default:
        {}
        break;
      }
#ifdef MC_DEBUG
      std::cout << "\tlogProba: " << logProba << std::endl;
#endif
      (*lnComp)(i, k) += logProba;
    }
  }
}

} /* namespace mixt */
