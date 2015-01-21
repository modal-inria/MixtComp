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
 *  Created on: December 9, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_POISSONLIKELIHOOD_H
#define MIXT_POISSONLIKELIHOOD_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_PoissonStatistic.h"

namespace mixt
{

class PoissonLikelihood
{
  public:
    /** Constructor */
    PoissonLikelihood(const Vector<Real>* p_param,
                      const AugmentedData<Matrix<int> >* augData,
                      const Matrix<STK::Array2DPoint<int> >* p_dataStatStorage,
                      int nbClass);
    /** Destructor */
    virtual ~PoissonLikelihood();

    /** Compute the completed log-likelihood */
    void lnCompletedLikelihood(Vector<Real>* lnComp, int k);

    /** Compute the observed log-likelihood */
    void lnObservedLikelihood(Vector<Real>* lnComp, int k);

  private:
    /** Pointer to parameters table */
    const Vector<Real>* p_param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Matrix<int> >* p_augData_;

    /** Pointer to storage of statistics on missing values */
    const Matrix<STK::Array2DPoint<int> >* p_dataStatStorage_;

    /** Statistic object to describe Poisson law */
    PoissonStatistic poisson_;
};

} /* namespace mixt */

#endif // MIXT_POISSONLIKELIHOOD_H
