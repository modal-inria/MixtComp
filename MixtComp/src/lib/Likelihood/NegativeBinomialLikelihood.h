/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#ifndef NEGATIVEBINOMIALLIKELIHOOD_H
#define NEGATIVEBINOMIALLIKELIHOOD_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/NegativeBinomialStatistic.h"

namespace mixt
{

class NegativeBinomialLikelihood
{
  public:
    /** Constructor */
	NegativeBinomialLikelihood(const Vector<Real>& param,
                      const AugmentedData<Vector<int> >& augData,
                      int nbClass);

    /** Compute the completed log-likelihood */
    Real lnCompletedProbability(int i, int k) const;

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(int i, int k) const;

  private:
    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<int> >& augData_;

    /** Statistic object to describe Negative Binomial law */
    NegativeBinomialStatistic negativeBinomial_;
};

} /* namespace mixt */

#endif // NEGATIVEBINOMIALLIKELIHOOD_H
