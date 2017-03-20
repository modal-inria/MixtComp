/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
    GaussianLikelihood(const Vector<Real>& param,
                       const AugmentedData<Vector<Real> >& augData,
                       int nbClass);

    /** Compute the completed log-likelihood */
    Real lnCompletedProbability(int i, int k);

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(int i, int k);

  private:

    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<Real> >& augData_;

    NormalStatistic normal_;
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H */
