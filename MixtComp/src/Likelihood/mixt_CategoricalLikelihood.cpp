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
                                             const AugmentedData<STK::Array2D<int> >* augData) :
    p_param_(p_param),
    p_augData_(augData)
{}

CategoricalLikelihood::~CategoricalLikelihood()
{}

void CategoricalLikelihood::lnLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
    {
      if (p_augData_->present_(i, j) == true)   // likelihood for present value
      {
        int nbModalities = p_augData_->dataRanges_[j].range_;
        STK::Real proba = p_param_->elt(k * nbModalities + p_augData_->data_(i, j),
                                        j);
        lnComp->elt(i) += std::log(proba);
      }
    }
  }

  // likelihood for finite number of values, loop on missing individuals
  for (AugmentedData<STK::Array2D<int> >::ConstIt_MisInd itInd = p_augData_->misData_.begin();
       itInd != p_augData_->misData_.end();
       ++itInd)
  {
    // loop on missing variables
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar itVar = itInd->second.begin();
        itVar != itInd->second.end();
        ++itVar)
    {
      switch(itVar->second.first) // (iterator on variables)->(access mapped elements).(get MisType)
      {
        case missingFiniteValues_:
        {
          int i = itInd->first;
          int j = itVar->first;

          int nbModalities = p_augData_->dataRanges_[j].range_;

          STK::Real proba = 0.;

          for (std::vector<int>::const_iterator itMiss = itVar->second.second.begin(); // (iterator on variables)->(access mapped elements).(vector of values of MisVal)
               itMiss != itVar->second.second.end();
               ++itMiss)
          {
            proba += p_param_->elt(k * nbModalities + *itMiss,
                                   j);
          }
          lnComp->elt(i) += std::log(proba);
        }
        break;
      }
    }
  }
}

} /* namespace mixt */
