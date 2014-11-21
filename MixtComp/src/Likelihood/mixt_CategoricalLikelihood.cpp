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
                                             const std::map<int, std::map<int, std::vector<std::pair<int, STK::Real> > > >* p_dataStatStorage) :
    p_param_(p_param),
    p_augData_(p_augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

CategoricalLikelihood::~CategoricalLikelihood()
{}

void CategoricalLikelihood::lnCompletedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
    {
      if (p_augData_->present_(i, j) == true)   // likelihood for present value
      {
        int nbModalities = p_augData_->globalRange_.range_;
        STK::Real proba = p_param_->elt(k * nbModalities + p_augData_->data_(i, j),
                                        j);
        lnComp->elt(i) += std::log(proba);
      }
    }
  }

  // likelihood for estimated missing values
  std::map<int, std::map<int, std::pair<MisType, std::vector<int>   > > >::const_iterator itSamplePos;
  std::map<int, std::map<int, std::vector<std::pair<int, STK::Real> > > >::const_iterator itSampleData;
  for(itSamplePos  = p_augData_->misData_.begin(),
      itSampleData = p_dataStatStorage_->begin();
      itSamplePos  != p_augData_->misData_.end() &&
      itSampleData != p_dataStatStorage_->end();
      ++itSamplePos,
      ++itSampleData)
  {
    std::map<int, std::pair<MisType, std::vector<int>   > >::const_iterator itVarPos;
    std::map<int, std::vector<std::pair<int, STK::Real> > >::const_iterator itVarData;
    for(itVarPos  = itSamplePos->second.begin(),
        itVarData = itSampleData->second.begin();
        itVarPos  != itSamplePos->second.end() &&
        itVarData != itSampleData->second.end();
        ++itVarPos,
        ++itVarData)
    {
      int i = itSamplePos->first;
      int j = itVarPos->first;

#ifdef MC_DEBUG
      std::cout << "\tGaussianLikelihood::lnCompletedLikelihood, missing var, i: " << i << ", j: " << j << std::endl;
#endif

      int nbModalities = p_augData_->globalRange_.range_;
      STK::Real proba = p_param_->elt(k * nbModalities + itVarData->second[0].first,
                                      j);
      lnComp->elt(i) += std::log(proba); // added lnLikelihood using the mode
    }
  }

}

void CategoricalLikelihood::lnObservedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
    {
      if (p_augData_->present_(i, j) == true)   // likelihood for present value
      {
        int nbModalities = p_augData_->globalRange_.range_;
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
        case missing_:
        {}
        break;

        case missingFiniteValues_:
        {
          int i = itInd->first;
          int j = itVar->first;

          int nbModalities = p_augData_->globalRange_.range_;

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
