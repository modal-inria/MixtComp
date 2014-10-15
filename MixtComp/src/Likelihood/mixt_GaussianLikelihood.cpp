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

namespace mixt
{

GaussianLikelihood::GaussianLikelihood(const STK::Array2D<STK::Real>* p_param,
                                       const AugmentedData<STK::Array2D<STK::Real> >* augData) :
    p_param_(p_param),
    p_augData_(augData)
{}

GaussianLikelihood::~GaussianLikelihood()
{}

void GaussianLikelihood::lnLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
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
        lnComp->elt(i) += STK::Law::Normal::lpdf(p_augData_->data_(i, j),
                                                 mean,
                                                 sd);
      }
    }
  }

  // loop on missing individuals
  for (ConstIt_MisInd itInd = p_augData_->misData_.begin();
       itInd != p_augData_->misData_.end();
       ++itInd)
  {
    // loop on missing variables
    for (ConstIt_MisVar itVar = itInd->begin();
        itVar != itInd->end();
        ++itVar)
    {
      switch(itVar->second.first)
      {
        case missingIntervals_:
        {
          int i = itInd->first;
          int j = itVar->first;

          STK::Real mean  = p_param_->elt(2*k    , j);
          STK::Real sd    = p_param_->elt(2*k + 1, j);

          STK::Real leftBound  = itVar->second.second[0];
          STK::Real rightBound = itVar->second.second[1];

          STK::Law::Normal normal(mean, sd);

          lnComp->elt(i) += std::log(normal.cdf(rightBound) -
                                     normal.cdf(leftBound));
        }
        break;

        case missingLUIntervals_:
        {
          int i = itInd->first;
          int j = itVar->first;

          STK::Real mean  = p_param_->elt(2*k    , j);
          STK::Real sd    = p_param_->elt(2*k + 1, j);

          STK::Real leftBound  = itVar->second.second[0];

          STK::Law::Normal normal(mean, sd);

          lnComp->elt(i) += std::log(1. - normal.cdf(leftBound));
        }
        break;

        case missingRUIntervals_:
        {
          int i = itInd->first;
          int j = itVar->first;

          STK::Real mean  = p_param_->elt(2*k    , j);
          STK::Real sd    = p_param_->elt(2*k + 1, j);

          STK::Real rightBound  = itVar->second.second[0];

          STK::Law::Normal normal(mean, sd);

          lnComp->elt(i) += std::log(normal.cdf(rightBound));
        }
      }
    }
  }
} /* namespace mixt */
