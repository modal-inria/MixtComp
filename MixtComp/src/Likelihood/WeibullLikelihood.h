/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: February the 1st, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLLIKELIHOOD_H
#define WEIBULLLIKELIHOOD_H

#include "LinAlg/mixt_LinAlg.h"
#include "Data/mixt_AugmentedData.h"
#include "Statistic/WeibullStatistic.h"

namespace mixt {

class WeibullLikelihood {
  public:
    /** Constructor */
    WeibullLikelihood(const Vector<Real>& param, const AugmentedData<Vector<Real> >& augData, Index nbClass);

    /** Compute the completed log-likelihood */
    Real lnCompletedProbability(Index i, Index k) const;

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(Index i, Index k) const;

  private:

    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<Real> >& augData_;

    WeibullStatistic weibull_;
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H */
