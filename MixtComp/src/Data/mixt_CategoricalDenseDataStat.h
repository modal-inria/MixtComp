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
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CATEGORICALDENSEDATASTAT_H
#define MIXT_CATEGORICALDENSEDATASTAT_H

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

class CategoricalDenseDataStat
{
  public:
    CategoricalDenseDataStat(AugmentedData<Vector<int> >& augData,
                             Real confidenceLevel);

    ~CategoricalDenseDataStat();

    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);

    void imputeData(int ind);

    const Matrix<Real>& getDataStatStorage() const {return dataStatStorage_;};

    void resizeStatStorage(int nbInd) {dataStatStorage_.resize(nbInd, augData_.dataRange_.max_);};

  private:
    /** Reference to augmented data */
    AugmentedData<Vector<int> >& augData_;

    /** Dense description of the missing values */
    Matrix<Real> dataStatStorage_;

    /** Confidence level */
    Real confidenceLevel_;

    void sample(int ind);
};

} // namespace mixt

#endif // MIXT_CATEGORICALDENSEDATASTAT_H
