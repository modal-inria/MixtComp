/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 27, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_CATEGORICALSAMPLER_H
#define MIXT_CATEGORICALSAMPLER_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{

class IMixture;

class CategoricalSampler
{
  public:
    CategoricalSampler(const IMixture& mixture,
                       AugmentedData<Vector<int> >& augData,
                       const Vector<Real>& param,
                       int nbClass);

    /** Sample new values for the missing variables of the given individual */
    void samplingStepCheck  (int i,
                             int z_i);
    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    const IMixture& mixture_;

    int nbClass_;

    AugmentedData<Vector<int> >& augData_;

    const Vector<Real>& param_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* MIXT_CATEGORICALSAMPLER_H */
