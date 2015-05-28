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
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_GAUSSIANLIKELIHOOD_H
#define MIXT_GAUSSIANLIKELIHOOD_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_NormalStatistic.h"

namespace mixt
{

class GaussianLikelihood
{
  public:
    /** Constructor */
    GaussianLikelihood(const Vector<Real>* p_param,
                       const AugmentedData<Vector<Real> >* augData,
                       const Vector<RowVector<Real> >* p_dataStatStorage,
                       int nbClass);
    /** Destructor */
    virtual ~GaussianLikelihood();

    /** Compute the completed log-likelihood */
    Real lnCompletedProbability(int i, int k);

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(int i, int k);

  private:
    int nbClass_;

    /** Pointer to parameters table */
    const Vector<Real>* p_param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<Real> >* p_augData_;

    /** Pointer to storage of statistics on missing values */
    const Vector<RowVector<Real> >* p_dataStatStorage_;

    NormalStatistic normal_;
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H */
