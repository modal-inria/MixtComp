/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef GAUSSIANLIKELIHOOD_H
#define GAUSSIANLIKELIHOOD_H

#include <LinAlg/LinAlg.h>
#include "../../../Data/AugmentedData.h"
#include "../../../Statistic/NormalStatistic.h"

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
    Real lnCompletedProbability(int i, int k) const;

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(int i, int k) const;

  private:

    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<Real> >& augData_;

    NormalStatistic normal_;
};

} /* namespace mixt */

#endif /* GAUSSIANLIKELIHOOD_H */
