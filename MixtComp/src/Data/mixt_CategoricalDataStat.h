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

#ifndef MIXT_CATEGORICALDATASTAT_H
#define MIXT_CATEGORICALDATASTAT_H

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

class CategoricalDataStat
{
  public:
    CategoricalDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij,
                        Eigen::Matrix<std::vector<std::pair<int, STK::Real> >,
                                      Eigen::Dynamic,
                                      Eigen::Dynamic>* p_dataStatStorage,
                        STK::Real confidenceLevel,
                        int nbClass);
    ~CategoricalDataStat();
    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);
  private:
    // number of classes
    int nbClass_;
    // pointer to data array
    const AugmentedData<STK::Array2D<int> >* pm_augDataij_;
    /** Sparse description of the missing values */
    Eigen::Matrix<std::vector<std::pair<int, STK::Real> >,
                                          Eigen::Dynamic,
                                          Eigen::Dynamic>* p_dataStatStorage_;

    /** Array to count sampled values across iterations, for the current individual,
     * i: modalities */
    STK::Array2DVector<int> stat_;

    /** Confidence level */
    STK::Real confidenceLevel_;

    void sample(int ind);
};

} // namespace mixt

#endif // MIXT_CATEGORICALDATASTAT_H
