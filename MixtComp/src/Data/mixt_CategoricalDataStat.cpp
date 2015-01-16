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
#include "../Various/mixt_Constants.h"
#include "DManager/include/STK_HeapSort.h"

namespace mixt
{

CategoricalDataStat::CategoricalDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij,
                                         Eigen::Matrix<std::vector<std::pair<int, STK::Real> >,
                                                       Eigen::Dynamic,
                                                       Eigen::Dynamic>* p_dataStatStorage,
                                         STK::Real confidenceLevel,
                                         int nbClass) :
    nbClass_(nbClass),
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

CategoricalDataStat::~CategoricalDataStat() {};

void CategoricalDataStat::sample(int ind)
{
  if (pm_augDataij_->misData_(ind, 0).first != present_)
  {
    int currMod = pm_augDataij_->data_(ind,
                                       0);
    stat_[currMod] += 1.;
  }
}

void CategoricalDataStat::sampleVals(int ind,
                                     int iteration,
                                     int iterationMax)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalDataStat::sampleVals, ind: " << ind << ", iteration: " << iteration << std::endl;
#endif
  if (pm_augDataij_->misData_(ind, 0).first != present_)
  {
    if (iteration == 0) // clear the temporary statistical object
    {
      // initialize internal storage
#ifdef MC_DEBUG
      std::cout << "\tminModality: " << minModality << std::endl;
      std::cout << "\tnbClass_: " << nbClass_ << std::endl;
#endif
      stat_.resize(STK::Range(minModality,
                              nbClass_));

      // clear current individual
      (*p_dataStatStorage_)(ind, 0) = std::vector<std::pair<int, STK::Real> >();

      // first sampling, on each missing variables
      sample(ind);
    }
    else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
    {
      // last sampling
      sample(ind);

      STK::Array2DVector<STK::Real> proba = stat_ / STK::Real(iterationMax + 1); // from count to probabilities
      STK::Array2DPoint<int> indOrder; // to store indices of ascending order
      STK::heapSort(indOrder, proba);
      STK::Real cumProb = 0.;

      for (int i = nbClass_;
           i > minModality - 1;
           --i)
      {
        int currMod = indOrder[i];
        STK::Real currProba = proba[currMod];
        (*p_dataStatStorage_)(ind, 0).push_back(std::pair<int, STK::Real>(currMod, currProba));
        cumProb += currProba;
#ifdef MC_DEBUG
        std::cout << "\ti: " << i << ", currMod: " << currMod << ", proba[currMod]: " << proba[currMod] << std::endl;
        std::cout << "\tcumProb: " << cumProb << std::endl;
        std::cout << "p_dataStatStorage_->elt(ind, j).back().first: " << (*p_dataStatStorage_)(ind, 0).back().first << std::endl;
#endif
        if (cumProb > confidenceLevel_)
        {
          break;
        }
      }
    }
#ifdef MC_DEBUG
    for (std::vector<std::pair<int, STK::Real> >::const_iterator itVec = (*p_dataStatStorage_)(ind, 0).begin();
         itVec != (*p_dataStatStorage_)(ind, 0).end();
         ++itVec)
    {
      std::cout << "itVec->first: " << itVec->first << ", itVec->second: " << itVec->second << std::endl;
    }
#endif
    else // any other iteration: juste store the current value
    {
      sample(ind);
    }
  }
}

} // namespace mixt
