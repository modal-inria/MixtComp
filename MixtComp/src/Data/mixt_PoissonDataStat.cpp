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

#include "mixt_PoissonDataStat.h"
#include "DManager/include/STK_HeapSort.h"

namespace mixt
{

PoissonDataStat::PoissonDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij,
                                 STK::Array2D<STK::Array2DPoint<int> >* p_dataStatStorage,
                                 STK::Real confidenceLevel) :
    pm_augDataij_(pm_augDataij),
    p_dataStatStorage_(p_dataStatStorage),
    confidenceLevel_(confidenceLevel)
{}

PoissonDataStat::~PoissonDataStat() {};

void PoissonDataStat::sample(int ind,
                             int iteration)
{
  if (pm_augDataij_->misData_(ind, 0).first != present_)
  {
    int currVal = pm_augDataij_->data_(ind,
                                       0);
    tempStat_[iteration] = currVal;
  }
}

void PoissonDataStat::sampleVals(int ind,
                                 int iteration,
                                 int iterationMax)
{
#ifdef MC_DEBUG
  std::cout << "GaussianDataStat::sampleVals" << std::endl;
#endif
  if (iteration == 0) // clear the temporary statistical object
  {
    if (pm_augDataij_->misData_(ind, 0).first != present_)
    {
      tempStat_ = STK::Array2DVector<STK::Real>(iterationMax + 1,
                                                0.);
    }

#ifdef MC_DEBUG
    std::cout << "p_dataStatStorage_->sizeRows(): " << p_dataStatStorage_->sizeRows() << ", p_dataStatStorage_->sizeCols(): "<< p_dataStatStorage_->sizeCols() << std::endl;
#endif
    // clear global individual
    p_dataStatStorage_->elt(ind, 0) = STK::Array2DPoint<STK::Real>(3, 0.);

    // first sampling
    sample(ind, iteration);
  }
  else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
  {
    // last sampling
    sample(ind, iteration);

    if (pm_augDataij_->misData_(ind, 0).first != present_)
    {
    }
  }
  else
  {
    // standard sampling
    sample(ind, iteration);
  }
}
} // namespace mixt
