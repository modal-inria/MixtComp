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
                                         Eigen::Matrix<std::vector<std::pair<int, STK::Real> >,
                                                       Eigen::Dynamic,
                                                       Eigen::Dynamic>* p_dataStatStorage,
                                         STK::Real confidenceLevel) :
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

CategoricalDataStat::~CategoricalDataStat() {};

void CategoricalDataStat::sample(int ind)
{
  for (int j = 0; j < pm_augDataij_->data_.sizeCols(); ++j)
  {
    if (pm_augDataij_->misData_(ind, j).first != present_)
    {
      int currMod = pm_augDataij_->data_(ind,
                                         j);
      tempStat_[j][currMod] += 1.;
    }
  }
}

void CategoricalDataStat::sampleVals(int ind,
                                     int iteration,
                                     int iterationMax)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalDataStat::sampleVals, ind: " << ind << ", iteration: " << iteration << std::endl;
#endif
  if (iteration == 0) // clear the temporary statistical object
  {
    // initialize internal storage
    tempStat_.resize(pm_augDataij_->data_.sizeCols());

    // clear current individual
    for (int j = 0; j < pm_augDataij_->data_.sizeCols(); ++j)
    {
      (*p_dataStatStorage_)(ind, j) = std::vector<std::pair<int, STK::Real> >();
    }

    // creation of the objects for counting the modalities
    for (int j = 0; j < pm_augDataij_->data_.sizeCols(); ++j)
    {
      if (pm_augDataij_->misData_(ind, j).first != present_)
      {
        tempStat_[j] = STK::Array2DPoint<STK::Real>(STK::Range(pm_augDataij_->dataRanges_[j].min_, // no access to param, hence no need for the globRange_
                                                               pm_augDataij_->dataRanges_[j].range_),
                                                    0);
      }
    }

    // first sampling, on each missing variables
    sample(ind);
  }
  else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
  {
    // last sampling
    sample(ind);

    for (int j = 0; j < pm_augDataij_->data_.sizeCols(); ++j)
    {
      if (pm_augDataij_->misData_(ind, j).first != present_)
      {
        STK::Array2DPoint<STK::Real> proba;
        proba = tempStat_[j] / STK::Real(iterationMax + 1); // from count to probabilities
        STK::Array2DPoint<int> indOrder; // to store indices of ascending order
        STK::heapSort(indOrder, proba);
        STK::Real cumProb = 0.;
#ifdef MC_DEBUG
        std::cout << "pm_augDataij_->dataRanges_[j].max_: " << pm_augDataij_->dataRanges_[j].max_ << std::endl;
        std::cout << "pm_augDataij_->dataRanges_[j].min_ - 1: " << pm_augDataij_->dataRanges_[j].min_ - 1 << std::endl;
#endif
        for (int i = pm_augDataij_->dataRanges_[j].max_;
             i > pm_augDataij_->dataRanges_[j].min_ - 1;
             --i)
        {
          int currMod = indOrder[i];
          STK::Real currProba = proba[currMod];
          (*p_dataStatStorage_)(ind, j).push_back(std::pair<int, STK::Real>(currMod, currProba));
          cumProb += currProba;
#ifdef MC_DEBUG
          std::cout << "\ti: " << i << ", currMod: " << currMod << ", proba[currMod]: " << proba[currMod] << std::endl;
          std::cout << "\tcumProb: " << cumProb << std::endl;
          std::cout << "p_dataStatStorage_->elt(ind, j).back().first: " << (*p_dataStatStorage_)(ind, j).back().first << std::endl;
#endif
          if (cumProb > confidenceLevel_)
          {
            break;
          }
        }
      }
#ifdef MC_DEBUG
      for (std::vector<std::pair<int, STK::Real> >::const_iterator itVec = (*p_dataStatStorage_)(ind, j).begin();
           itVec != (*p_dataStatStorage_)(ind, j).end();
           ++itVec)
      {
        std::cout << "itVec->first: " << itVec->first << ", itVec->second: " << itVec->second << std::endl;
      }
#endif
    }
  }
  else // any other iteration: juste store the current value
  {
    sample(ind);
  }
}

} // namespace mixt
