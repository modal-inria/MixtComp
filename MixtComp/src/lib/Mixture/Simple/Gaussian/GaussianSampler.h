/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 14 feb. 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_GAUSSIANSAMPLER_H
#define MIXT_GAUSSIANSAMPLER_H

#include "../../../LinAlg/mixt_LinAlg.h"
#include "../../../Data/mixt_AugmentedData.h"
#include "../../../Statistic/NormalStatistic.h"


namespace mixt
{

class GaussianSampler
{
  public:
    GaussianSampler(AugmentedData<Vector<Real> >& augData,
                    const Vector<Real>& param,
                    int nbClass);

    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    AugmentedData<Vector<Real> >& augData_;

    const Vector<Real>& param_;

    UniformStatistic uniform_;

    NormalStatistic normal_;

    ExponentialStatistic exponential_;
};

} // namespace mixt

#endif /* MIXT_GAUSSIANSAMPLER_H */
