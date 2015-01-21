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

#ifndef MIXT_POISSONDATASTAT_H
#define MIXT_POISSONDATASTAT_H

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_AugmentedData.h"

namespace mixt
{
class PoissonDataStat
{
  public:
    PoissonDataStat(const AugmentedData<Matrix<int> >* pm_augDataij,
                    Matrix<STK::Array2DPoint<int> >* p_dataStatStorage,
                    Real confidenceLevel,
                    int nbClass);
    ~PoissonDataStat();
    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);
  private:
    /** pointer to data array */
    const AugmentedData<Matrix<int> >* pm_augDataij_;
    /** Description of the missing values */
    Matrix<STK::Array2DPoint<int> >* p_dataStatStorage_;

    /** Array to count sampled values across iterations, for the current individual, access: tempStat_[i]
     * i: iteration
     * Variables are ignored, as they are not supported in the global framework*/
    Vector<int> stat_;

    /** Confidence level */
    Real confidenceLevel_;

    void sample(int ind,
                int iteration);
};

} // namespace mixt

#endif // MIXT_POISSONDATASTAT_H
