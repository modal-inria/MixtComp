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

#include "Arrays/include/STK_Array2D.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

class CategoricalDataStat
{
  public:
    CategoricalDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij,
                        std::map<int, std::map<int, std::vector<std::pair<int, STK::Real> > > >* p_dataStatStorage,
                        STK::Real confidenceLevel);
    ~CategoricalDataStat();
    /** initialize both posMissing and statMissing */
    void initialize();
    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);
  private:
    /** Set the number of modalities, to count the results */
    void setModalities();

    // number of modalities
    int nbModalities_;
    // pointer to data array
    const AugmentedData<STK::Array2D<int> >* pm_augDataij_;
    /** Sparse description of the missing values */
    std::map<int, std::map<int, std::vector<std::pair<int, STK::Real> > > >* p_dataStatStorage_;

    /** Array to store values across iterations, for a given individual, map : var->modalities vector*/
    std::map<int, STK::Array2DPoint<STK::Real> > tempStat_;
    /** Confidence level */
    STK::Real confidenceLevel_;
};

} // namespace mixt

#endif // MIXT_CATEGORICALDATASTAT_H
