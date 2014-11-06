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

#include "mixt_CategoricalDataStat.h"
#include "DManager/include/STK_HeapSort.h"

namespace mixt
{

CategoricalDataStat::CategoricalDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij,
                                         std::map<int, std::map<int, std::vector<std::pair<int, STK::Real> > > >* p_dataStatStorage,
                                         STK::Real confidenceLevel) :
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

CategoricalDataStat::~CategoricalDataStat() {};

void CategoricalDataStat::sampleVals(int sample,
                                     int iteration,
                                     int iterationMax)
{
  if (iteration == 1) // clear the temporary statistical object
  {
    tempStat_.clear();
    // creation of the objects for counting the modalities
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar it_misVar = pm_augDataij_->getInd(sample).begin(); // find iterator is necessary to access an element on a const map
         it_misVar != pm_augDataij_->getInd(sample).end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      tempStat_[var] = STK::Array2DPoint<STK::Real>(STK::Range(pm_augDataij_->dataRanges_[var].min_, // no access to param, hence no need for the globRange_
                                                               pm_augDataij_->dataRanges_[var].range_),
                                                    0);
    }

    // first sampling, on each missing variables
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar it_misVar = pm_augDataij_->getInd(sample).begin();
         it_misVar != pm_augDataij_->getInd(sample).end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      int currMod = pm_augDataij_->data_(sample,
                                         var);
      tempStat_[var][currMod] += 1.;
    }
  }
  else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
  {
    for(std::map<int, STK::Array2DPoint<STK::Real> >::const_iterator it = tempStat_.begin();
        it != tempStat_.end();
        ++it)
    {
      int var = it->first;
      STK::Array2DPoint<STK::Real> proba;
      proba = it->second / STK::Real(iteration); // from count to probabilities
      STK::Array2DPoint<int> indOrder; // to store indices of ascending order
      STK::heapSort(indOrder, proba);
      STK::Real cumProb = 0.;
      for (int i = pm_augDataij_->dataRanges_[var].range_ - 1; i > -1; --i)
      {
        int currMod = indOrder[i];
        STK::Real currProba = proba[currMod];
        (*p_dataStatStorage_)[sample][var].push_back(std::pair<int, STK::Real>(currMod, currProba));
        cumProb += currProba;
        if (cumProb > confidenceLevel_)
        {
          break;
        }
      }
    }
  }
  else
  {
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar it_misVar = pm_augDataij_->getInd(sample).begin();
         it_misVar != pm_augDataij_->getInd(sample).end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      int currMod = pm_augDataij_->data_(sample,
                                         var);
      tempStat_[var][currMod] += 1;
    }
  }
}

} // namespace mixt
