/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#ifndef NEGATIVEBINOMIALSAMPLER_H
#define NEGATIVEBINOMIALSAMPLER_H

#include "../../../LinAlg/mixt_LinAlg.h"
#include "../../../Data/mixt_AugmentedData.h"
#include "NegativeBinomialStatistic.h"

namespace mixt
{

class IMixture;

class NegativeBinomialSampler
{
  public:
	NegativeBinomialSampler(AugmentedData<Vector<int> >& augData,
							const Vector<Real>& param, int nbClass);

    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    AugmentedData<Vector<int> >& augData_;

    const Vector<Real>& param_;

    /** Statistic object to describe Negative Binomial law */
    NegativeBinomialStatistic negativeBinomial_;
};

} // namespace mixt

#endif // NEGATIVEBINOMIALSAMPLER_H
