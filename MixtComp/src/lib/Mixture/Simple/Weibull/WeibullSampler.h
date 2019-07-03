/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLSAMPLER_H
#define WEIBULLSAMPLER_H

#include <Data/AugmentedData.h>
#include <LinAlg/LinAlg.h>
#include "WeibullStatistic.h"

namespace mixt {

class WeibullSampler {
  public:
    WeibullSampler(AugmentedData<Vector<Real> >& augData, const Vector<Real>& param, Index nbClass);

    void samplingStepNoCheck(Index i, Index z_i);
  private:
    AugmentedData<Vector<Real> >& augData_;

    const Vector<Real>& param_;

    WeibullStatistic weibull_;
};

} // namespace mixt

#endif // WEIBULLSAMPLER_H
