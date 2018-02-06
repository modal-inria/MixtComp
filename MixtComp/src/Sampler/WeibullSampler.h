/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLSAMPLER_H
#define WEIBULLSAMPLER_H

#include "LinAlg/mixt_LinAlg.h"
#include "Data/mixt_AugmentedData.h"
#include "Statistic/WeibullStatistic.h"

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
