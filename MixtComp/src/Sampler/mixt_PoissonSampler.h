/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_POISSONSAMPLER_H
#define MIXT_POISSONSAMPLER_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_PoissonStatistic.h"

namespace mixt
{

class IMixture;

class PoissonSampler
{
  public:
    PoissonSampler(const IMixture& mixture,
                   AugmentedData<Vector<int> >& augData,
                   const Vector<Real>& param,
                   int nbClass);

    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    const IMixture& mixture_;

    AugmentedData<Vector<int> >& augData_;

    const Vector<Real>& param_;

    /** Statistic object to describe Poisson law */
    PoissonStatistic poisson_;
};

} // namespace mixt

#endif // MIXT_POISSONSAMPLER_H
