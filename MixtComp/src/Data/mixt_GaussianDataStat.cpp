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
                                   STK::Array2D<STK::Array2DPoint<STK::Real> >* p_dataStatStorage,
                                   STK::Real confidenceLevel,
                                   int nbClass) :
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

GaussianDataStat::~GaussianDataStat() {};

void GaussianDataStat::sample(int ind,
                              int iteration)
{
  if (pm_augDataij_->misData_(ind, 0).first != present_)
  {
    STK::Real currVal = pm_augDataij_->data_(ind,
                                             0);
    stat_[iteration] = currVal;
  }
}

void GaussianDataStat::sampleVals(int ind,
                                  int iteration,
                                  int iterationMax)
{
#ifdef MC_DEBUG
  std::cout << "GaussianDataStat::sampleVals" << std::endl;
#endif
  if (pm_augDataij_->misData_(ind, 0).first != present_)
  {
    if (iteration == 0) // clear the temporary statistical object
    {
      // initialize internal storage
      stat_.resize(iterationMax + 1);

  #ifdef MC_DEBUG
      std::cout << "p_dataStatStorage_->sizeRows(): " << p_dataStatStorage_->sizeRows() << ", p_dataStatStorage_->sizeCols(): "<< p_dataStatStorage_->sizeCols() << std::endl;
  #endif
      // clear current individual
      for (int j = 0; j < pm_augDataij_->data_.sizeCols(); ++j)
      {
        p_dataStatStorage_->elt(ind, j) = STK::Array2DPoint<STK::Real>(3, 0.);
      }

      // first sampling
      sample(ind, iteration);
    }
    else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
    {
      // last sampling
      sample(ind, iteration);

#ifdef MC_DEBUG
      std::cout << "GaussianDataStat::sampleVals, last iteration" << std::endl;
      std::cout << "j: " << j << std::endl;
      std::cout << "p_dataStatStorage_->sizeRows(): " << p_dataStatStorage_->sizeRows() << ", p_dataStatStorage_->sizeCols(): " << p_dataStatStorage_->sizeCols() << std::endl;
      std::cout << "tempStat_[j].sizeRows(): " << tempStat_[j].sizeRows() << std::endl;
      std::cout << "tempStat_[j]: " << std::endl;
      std::cout << tempStat_[j] << std::endl;
#endif
      STK::Array2DVector<int> indOrder; // to store indices of ascending order
      STK::heapSort(indOrder, stat_);
      STK::Real alpha = (1. - confidenceLevel_) / 2.;
      STK::Real realIndLow = alpha * iterationMax;
      STK::Real realIndHigh = (1. - alpha) * iterationMax;

      STK::Array2DPoint<STK::Real> tempPoint(3);
      tempPoint[0] = stat_.mean();
      tempPoint[1] =  (1. - (realIndLow  - int(realIndLow ))) * stat_[indOrder[int(realIndLow )    ]]
                    + (      realIndLow  - int(realIndLow ) ) * stat_[indOrder[int(realIndLow ) + 1]];
      tempPoint[2] =  (1. - (realIndHigh - int(realIndHigh))) * stat_[indOrder[int(realIndHigh)    ]]
                    + (      realIndHigh - int(realIndHigh) ) * stat_[indOrder[int(realIndHigh) + 1]];
      p_dataStatStorage_->elt(ind, 0) = tempPoint;
#ifdef MC_DEBUG
      std::cout << "confidenceLevel_: " << confidenceLevel_ << std::endl;
      std::cout << "alpha: " << alpha << std::endl;
      std::cout << "realIndLow: " << realIndLow << std::endl;
      std::cout << "realIndHigh: " << realIndHigh << std::endl;
      std::cout << "tempVec: " << tempVec << std::endl;
#endif
    }
    else
    {
      // standard sampling
      sample(ind, iteration);
    }
  }
}

} // namespace mixt
