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

/** Source of the implementation:
 * http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm */

#ifndef MIXT_POISSONDATASTAT_H
#define MIXT_POISSONDATASTAT_H

#include "Arrays/include/STK_Array2D.h"
#include "mixt_AugmentedData.h"

namespace mixt
{
class PoissonDataStat
{
  public:
    PoissonDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij,
                    STK::Array2D<STK::Array2DPoint<int> >* p_dataStatStorage,
                    STK::Real confidenceLevel);
    ~PoissonDataStat();
    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);
  private:
    /** pointer to data array */
    const AugmentedData<STK::Array2D<int> >* pm_augDataij_;
    /** Sparse description of the missing values */
    STK::Array2D<STK::Array2DPoint<int> >* p_dataStatStorage_;

    /** Array to count sampled values across iterations, for the current individual, access: tempStat_[i]
     * i: iteration
     * Variables are ignored, as they are not supported in the global framework*/
    STK::Array2DVector<int> tempStat_;

    /** Confidence level */
    STK::Real confidenceLevel_;

    void sample(int ind,
                int iteration);
};

} // namespace mixt

#endif // MIXT_POISSONDATASTAT_H
