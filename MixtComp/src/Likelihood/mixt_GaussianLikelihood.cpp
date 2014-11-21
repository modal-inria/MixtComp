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
#include "../Various/mixt_Def.h"
#include <boost/math/distributions/normal.hpp>

namespace mixt
{

GaussianLikelihood::GaussianLikelihood(const STK::Array2D<STK::Real>* p_param,
                                       const AugmentedData<STK::Array2D<STK::Real> >* augData,
                                       const std::map<int, std::map<int, STK::Array2DVector<STK::Real> > >* p_dataStatStorage) :
    p_param_(p_param),
    p_augData_(augData),
    p_dataStatStorage_(p_dataStatStorage)
{}

GaussianLikelihood::~GaussianLikelihood()
{}

void GaussianLikelihood::lnCompletedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
    {
      if (p_augData_->present_(i, j) == true)   // likelihood for present value
      {
        STK::Real mean  = p_param_->elt(2*k    , j);
        STK::Real sd    = p_param_->elt(2*k + 1, j);
        boost::math::normal norm(mean, sd);

        STK::Real proba = boost::math::cdf(norm,
                                           p_augData_->data_(i, j));

        lnComp->elt(i) += std::log(proba);
      }
    }
  }

  // likelihood for estimated missing values
  // imputation of missing value and export of their statistics
  std::map<int, std::map<int, std::pair<MisType, std::vector<STK::Real>   > > >::const_iterator itSamplePos;
  std::map<int, std::map<int, STK::Array2DVector<STK::Real>                 > >::const_iterator itSampleData;
  for(itSamplePos  = p_augData_->misData_.begin(),
      itSampleData = p_dataStatStorage_->begin();
      itSamplePos  != p_augData_->misData_.end() &&
      itSampleData != p_dataStatStorage_->end();
      ++itSamplePos,
      ++itSampleData)
  {
    std::map<int, std::pair<MisType, std::vector<STK::Real>   > >::const_iterator itVarPos;
    std::map<int, STK::Array2DVector<STK::Real>                 >::const_iterator itVarData;
    for(itVarPos  = itSamplePos->second.begin(),
        itVarData = itSampleData->second.begin();
        itVarPos  != itSamplePos->second.end() &&
        itVarData != itSampleData->second.end();
        ++itVarPos,
        ++itVarData)
    {
      int i = itSamplePos->first;
      int j = itVarPos->first;

      STK::Real mean  = p_param_->elt(2*k    , j);
      STK::Real sd    = p_param_->elt(2*k + 1, j);
      boost::math::normal norm(mean, sd);

      STK::Real proba = boost::math::cdf(norm,
                                         itVarData->second[0]);
      lnComp->elt(i) += proba;

#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnCompletedLikelihood, missing var, i: " << i << ", j: " << j << std::endl;
      std::cout << "\tproba: " << proba << std::endl;
#endif
    }
  }
}

void GaussianLikelihood::lnObservedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
{
#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnObservedLikelihood" << std::endl;
#endif
  // likelihood for present data
  for (int i = 0; i < p_augData_->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData_->data_.sizeCols(); ++j)
    {
      if (p_augData_->present_(i, j) == true)   // likelihood for present value
      {
        STK::Real mean  = p_param_->elt(2*k    , j);
        STK::Real sd    = p_param_->elt(2*k + 1, j);
        boost::math::normal norm(mean, sd);

        STK::Real proba = boost::math::cdf(norm,
                                           p_augData_->data_(i, j));
      }
    }
  }

  // loop on missing individuals
  for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisInd itInd = p_augData_->misData_.begin();
       itInd != p_augData_->misData_.end();
       ++itInd)
  {
    // loop on missing variables
    for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisVar itVar = itInd->second.begin();
        itVar != itInd->second.end();
        ++itVar)
    {
      int i = itInd->first;
      int j = itVar->first;

      STK::Real mean  = p_param_->elt(2*k    , j);
      STK::Real sd    = p_param_->elt(2*k + 1, j);
      boost::math::normal norm(mean, sd);

      STK::Real proba;
      STK::Real logProba;

#ifdef MC_DEBUG
      std::cout << "missing var, i: " << i << ", j: " << j << std::endl;
      std::cout << "\tmean: " << mean << ", sd: " << sd << std::endl;
#endif

      switch(itVar->second.first)
      {
        case missing_: // no contribution to the oberved likelihood
        {
#ifdef MC_DEBUG
      std::cout << "\tmissing" << std::endl;
#endif
        }
        break;

        case missingIntervals_:
        {
          STK::Real leftBound  = itVar->second.second[0];
          STK::Real rightBound = itVar->second.second[1];
#ifdef MC_DEBUG
      std::cout << "\tmissingIntervals_" << std::endl;
      std::cout << "\tleftBound: " << leftBound << "\tboost::math::cdf(norm, leftBound): " << boost::math::cdf(norm, leftBound) << std::endl;
      std::cout << "\trightBound: " << rightBound << "\tboost::math::cdf(norm, rightBound): " << boost::math::cdf(norm, rightBound) << std::endl;
#endif
          proba = boost::math::cdf(norm, rightBound) -
                  boost::math::cdf(norm, leftBound);
        }
        break;

        case missingLUIntervals_:
        {
#ifdef MC_DEBUG
      std::cout << "\tmissingLUIntervals_" << std::endl;
#endif
          STK::Real leftBound  = itVar->second.second[0];

          proba = 1. - boost::math::cdf(norm, leftBound);
        }
        break;

        case missingRUIntervals_:
        {
#ifdef MC_DEBUG
      std::cout << "\tmissingRUIntervals_" << std::endl;
#endif
          STK::Real rightBound  = itVar->second.second[0];

          proba = 1. - boost::math::cdf(norm, rightBound);
        }
        break;
      }

      logProba = std::log(proba);
      lnComp->elt(i) += logProba;

#ifdef MC_DEBUG
      std::cout << "\tproba: " << proba << std::endl;
      std::cout << "\tlogProba: " << logProba << std::endl;
#endif
    }
  }
}

} /* namespace mixt */
