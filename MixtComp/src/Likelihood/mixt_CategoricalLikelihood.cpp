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

namespace mixt
{

CategoricalLikelihood::CategoricalLikelihood(const STK::Array2D<STK::Real>* p_param,
                                             const AugmentedData<STK::Array2D<int> >* p_augData,
                                             const STK::Array2D<std::vector<std::pair<int, STK::Real> > >* p_dataStatStorage) :
    p_param_(p_param),
    p_augData_(p_augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

CategoricalLikelihood::~CategoricalLikelihood()
{}

void CategoricalLikelihood::lnCompletedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
  for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
  {
    for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
    {
      if (p_augData_->misData_(i, j).first == present_) // likelihood for present data
      {
        int nbModalities = p_augData_->globalRange_.range_;
        STK::Real proba = p_param_->elt(k * nbModalities + p_augData_->data_(i, j),
                                        j);
        lnComp->elt(i) += std::log(proba);
      }
      else // likelihood for estimated missing values, imputation by the mode
      {
        int nbModalities = p_augData_->globalRange_.range_;
        STK::Real proba = p_param_->elt(k * nbModalities + p_dataStatStorage_->elt(i, j)[0].first,
                                        j);
        lnComp->elt(i) += std::log(proba); // added lnLikelihood using the mode
      }
    }
  }
}

void CategoricalLikelihood::lnObservedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalLikelihood::lnObservedLikelihood" << std::endl;
#endif
  for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
  {
    for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
    {
#ifdef MC_DEBUG
          std::cout << "\ti: " << i << ", j: " << j << std::endl;
#endif
      switch (p_augData_->misData_(i, j).first)
      {
        case present_: // likelihood for present data
        {
          int nbModalities = p_augData_->globalRange_.range_;
          STK::Real proba = p_param_->elt(k * nbModalities + p_augData_->data_(i, j),
                                          j);
          lnComp->elt(i) += std::log(proba);
        }
        break;

        case missing_: // no contribution to the observed likelihood
        {}
        break;

        case missingFiniteValues_: // adding the contributions of the various modalities
        {
#ifdef MC_DEBUG
          std::cout << "missingFiniteValues" << std::endl;
          std::cout << "p_param_->sizeRows(): " << p_param_->sizeRows() << ", p_param_->sizeCols(): " << p_param_->sizeCols() << std::endl;
#endif
          int nbModalities = p_augData_->globalRange_.range_;

          STK::Real proba = 0.;

          for (std::vector<int>::const_iterator itMiss = p_augData_->misData_(i, j).second.begin();
               itMiss != p_augData_->misData_(i, j).second.end();
               ++itMiss)
          {
#ifdef MC_DEBUG
            std::cout << "k: " << k << ", j: " << j << ", nbModalities: " << nbModalities << ", *itMiss: " << *itMiss << std::endl;
#endif
            proba += p_param_->elt(k * nbModalities + *itMiss,
                                   j);
          }
          lnComp->elt(i) += std::log(proba);
        }
        break;

        default:
        {}
        break;
      }
    }
  }
}

} /* namespace mixt */
