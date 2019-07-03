/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef POISSONSAMPLER_H
#define POISSONSAMPLER_H

#include "../../../LinAlg/LinAlg.h"
#include "../../../Data/AugmentedData.h"
#include "PoissonStatistic.h"

namespace mixt
{

class IMixture;

class PoissonSampler
{
  public:
    PoissonSampler(AugmentedData<Vector<int> >& augData,
                   const Vector<Real>& param,
                   int nbClass);

    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    AugmentedData<Vector<int> >& augData_;

    const Vector<Real>& param_;

    /** Statistic object to describe Poisson law */
    PoissonStatistic poisson_;
};

} // namespace mixt

#endif // POISSONSAMPLER_H
