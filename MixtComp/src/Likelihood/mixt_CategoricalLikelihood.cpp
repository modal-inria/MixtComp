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
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_CategoricalLikelihood.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

CategoricalLikelihood::CategoricalLikelihood(const Vector<Real>* p_param,
                                             const AugmentedData<Vector<int> >* p_augData,
                                             const Vector<std::vector<std::pair<int, Real> > >* p_dataStatStorage,
                                             int nbClass) :
    nbClass_(nbClass),
    p_param_(p_param),
    p_augData_(p_augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

CategoricalLikelihood::~CategoricalLikelihood()
{}

Real CategoricalLikelihood::lnCompletedProbability(int i, int k)
{
  int nbModalities = p_param_->rows() / nbClass_;

  int ind = k * nbModalities + p_augData_->data_(i, 0) - minModality;
  Real proba = (*p_param_)(ind);
#ifdef MC_DEBUG
  if (proba < epsilon)
  {
    Real sum = 0.;
    std::cout << "Null proba detected, k: " << k << std::endl;
    std::cout << "p_augData_->data_(i, j) - minModality: " << p_augData_->data_(i, 0) - minModality << std::endl;
    std::cout << "param: " << std::endl;
    for (int p = 0; p < nbModalities; ++p)
    {
      std::cout << p_param_->elt(k * nbModalities + p) << std::endl;
      sum += p_param_->elt(k * nbModalities + p);
    }
    std::cout << "sum: " << sum << std::endl;
  }
#endif
  return std::log(proba);
}

Real CategoricalLikelihood::lnObservedProbability(int i, int k)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalLikelihood::lnObservedLikelihood" << std::endl;
#endif
  int nbModalities = p_param_->rows() / nbClass_;
  Real proba;
#ifdef MC_DEBUG
          std::cout << "\ti: " << i << ", j: " << j << std::endl;
#endif
  switch (p_augData_->misData_(i, 0).first)
  {
    case present_: // likelihood for present data
    {
      proba = (*p_param_)(k * nbModalities + p_augData_->data_(i, 0) - minModality);
    }
    break;

    case missing_: // no contribution to the observed likelihood
    {
      proba = 1.;
    }
    break;

    case missingFiniteValues_: // adding the contributions of the various modalities
    {
#ifdef MC_DEBUG
      std::cout << "missingFiniteValues" << std::endl;
      std::cout << "p_param_->rows(): " << p_param_->rows() << ", p_param_->cols(): " << p_param_->cols() << std::endl;
#endif
      proba = 0.;

      for (std::vector<int>::const_iterator itMiss = p_augData_->misData_(i, 0).second.begin();
           itMiss != p_augData_->misData_(i, 0).second.end();
           ++itMiss)
      {
#ifdef MC_DEBUG
        std::cout << "k: " << k << ", j: " << j << ", nbModalities: " << nbModalities << ", *itMiss: " << *itMiss << std::endl;
#endif
        proba += (*p_param_)(k * nbModalities + *itMiss - minModality);
      }
    }
    break;

    default:
    {}
    break;
  }
  return std::log(proba);
}

} /* namespace mixt */
