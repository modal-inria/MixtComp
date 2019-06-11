/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 27, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef CATEGORICALSAMPLER_H
#define CATEGORICALSAMPLER_H

#include "../../../LinAlg/LinAlg.h"
#include "../../../Data/AugmentedData.h"
#include "../../../Statistic/MultinomialStatistic.h"

namespace mixt
{

class CategoricalSampler
{
  public:
    CategoricalSampler(AugmentedData<Vector<int> >& augData,
                       const Vector<Real>& param,
                       int nbClass);

    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    int nbClass_;

    AugmentedData<Vector<int> >& augData_;

    const Vector<Real>& param_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* CATEGORICALSAMPLER_H */
