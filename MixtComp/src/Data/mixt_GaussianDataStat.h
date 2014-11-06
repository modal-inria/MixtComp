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

#ifndef MIXT_GAUSSIANDATASTAT_H
#define MIXT_GAUSSIANDATASTAT_H

#include "Arrays/include/STK_Array2D.h"
#include "mixt_AugmentedData.h"

namespace mixt
{
class GaussianDataStat
{
  public:
    GaussianDataStat(const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij,
                     std::map<int, std::map<int, STK::Array2DVector<STK::Real> > >* p_dataStatStorage,
                     STK::Real confidenceLevel);
    ~GaussianDataStat();
    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);
  private:
    /** pointer to data array */
    const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij_;
    /** Sparse description of the missing values */
    std::map<int, std::map<int, STK::Array2DVector<STK::Real> > >* p_dataStatStorage_;

    /** Array to store values across iterations, for a given individual, map : var -> vector of sampled values */
    std::map<int, STK::Array2DVector<STK::Real> > tempStat_;
    /** Confidence level */
    STK::Real confidenceLevel_;
};

} // namespace mixt

#endif // MIXT_GAUSSIANDATASTAT_H
