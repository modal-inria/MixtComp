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
 *  Created on: July 1, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianDataStat.h"
#include "DManager/include/STK_HeapSort.h"

namespace mixt
{

GaussianDataStat::GaussianDataStat(const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij,
                                   std::map<int, std::map<int, STK::Array2DVector<STK::Real> > >* p_dataStatStorage,
                                   STK::Real confidenceLevel) :
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

GaussianDataStat::~GaussianDataStat() {};

void GaussianDataStat::initialize() {};

void GaussianDataStat::sampleVals(int sample,
                                  int iteration,
                                  int iterationMax)
{
  if (iteration == 1) // clear the temporary statistical object
  {
    tempStat_.clear();
    // creation of the vectors to store the sampled values
    for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisVar it_misVar = pm_augDataij_->misData_.find(sample)->second.begin();
         it_misVar != pm_augDataij_->misData_.find(sample)->second.end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      tempStat_[var] = STK::Array2DVector<STK::Real>(iterationMax, 0.);
    }

    // first sampling, on each missing variables
    for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisVar it_misVar = pm_augDataij_->misData_.find(sample)->second.begin();
         it_misVar != pm_augDataij_->misData_.find(sample)->second.end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      STK::Real currVal = pm_augDataij_->data_(sample,
                                               var);
      tempStat_[var][iteration] = currVal;
    }
  }
  else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
  {
    for (std::map<int, STK::Array2DVector<STK::Real> >::const_iterator it_misVar = tempStat_.begin();
         it_misVar != tempStat_.end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      STK::Array2DVector<int> indOrder; // to store indices of ascending order
      STK::heapSort(indOrder, it_misVar->second);
      STK::Real realIndLow = confidenceLevel_ / 2. * iterationMax;
      STK::Real realIndHigh = (1. - confidenceLevel_ / 2.) * iterationMax;

      STK::Array2DVector<STK::Real> tempVec(3);
      tempVec[0] = it_misVar->second.mean();
      tempVec[1] =  (1. - (realIndLow  - int(realIndLow ))) * it_misVar->second[indOrder[int(realIndLow )    ]]
                  + (1. - (int(realIndLow ) - realIndLow )) * it_misVar->second[indOrder[int(realIndLow ) + 1]];
      tempVec[2] =  (1. - (realIndHigh - int(realIndHigh))) * it_misVar->second[indOrder[int(realIndHigh)    ]]
                  + (1. - (int(realIndHigh) - realIndHigh)) * it_misVar->second[indOrder[int(realIndHigh) + 1]];
      (*p_dataStatStorage_)[sample][var] = tempVec;
    }
  }
  else
  {
    // first sampling, on each missing variables
    for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisVar it_misVar = pm_augDataij_->misData_.find(sample)->second.begin();
         it_misVar != pm_augDataij_->misData_.find(sample)->second.end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      STK::Real currVal = pm_augDataij_->data_(sample,
                                               var);
      tempStat_[var][iteration] = currVal;
    }
  }
}
} // namespace mixt
