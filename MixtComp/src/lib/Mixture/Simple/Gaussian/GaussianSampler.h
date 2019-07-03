/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: 14 feb. 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef GAUSSIANSAMPLER_H
#define GAUSSIANSAMPLER_H

#include "../../../LinAlg/LinAlg.h"
#include "../../../Data/AugmentedData.h"
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

#endif /* GAUSSIANSAMPLER_H */
