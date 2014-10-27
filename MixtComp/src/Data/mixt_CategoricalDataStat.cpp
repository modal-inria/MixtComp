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
    nbModalities_(0),
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

CategoricalDataStat::~CategoricalDataStat() {};

void CategoricalDataStat::initialize()
{
  setModalities();
#ifdef MC_DEBUG
  std::cout << "CategoricalDataStat, initializing statMissing_ and posMissing_" << std::endl;
/*  std::cout << "statMissing_" <<  std::endl;
  std::cout << statMissing_ << std::endl;
  std::cout << "posMissing_" <<  std::endl;
  std::cout << posMissing_ << std::endl; */
#endif
}

void CategoricalDataStat::setModalities()
{
  std::vector<Range<int> >::const_iterator it   (pm_augDataij_->dataRanges_.begin());
  std::vector<Range<int> >::const_iterator itEnd(pm_augDataij_->dataRanges_.end  ());

  // compute global min and max, as in InitializeMixtureImpl<STK::Clust::Categorical_pjk_>
  int min = it->min_;
  int max = it->max_;

  ++it;

  for (; it != itEnd; ++it)
  {
    int currMin = it->min_;
    int currMax = it->max_;
    if (currMin < min) min = currMin;
    if (currMax > max) max = currMax;
  }

  nbModalities_ = max - min + 1;
#ifdef MC_DEBUG
  std::cout << "CategoricalDataStat::setModalities(): " << std::endl
            << "\tmin: " << min << std::endl
            << "\tmax: " << max << std::endl
            << "\tnbModalities_: " << nbModalities_ << std::endl;
#endif
}

void CategoricalDataStat::sampleVals(int sample,
                                     int iteration,
                                     int iterationMax)
{
  if (iteration == 1) // clear the temporary statistical object
  {
    tempStat_.clear();
    // creation of the objects for counting the modalities
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar it_misVar = pm_augDataij_->misData_.find(sample)->second.begin(); // find iterator is necessary to access an element on a const map
         it_misVar != pm_augDataij_->misData_.find(sample)->second.end();
         ++it_misVar)
    {
      int var = it_misVar->first;
      tempStat_[var] = STK::Array2DPoint<STK::Real>(STK::Range(pm_augDataij_->dataRanges_[var].min_,
                                                               nbModalities_),
                                                    0);
    }

    // first sampling, on each missing variables
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar it_misVar = pm_augDataij_->misData_.find(sample)->second.begin();
         it_misVar != pm_augDataij_->misData_.find(sample)->second.end();
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
      for (int i = nbModalities_ - 1; i > -1; --i)
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
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar it_misVar = pm_augDataij_->misData_.find(sample)->second.begin();
         it_misVar != pm_augDataij_->misData_.find(sample)->second.end();
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
