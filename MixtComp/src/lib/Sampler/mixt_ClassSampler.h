/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 4, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CLASSSAMPLER_H
#define MIXT_CLASSSAMPLER_H

#include "../Data/mixt_ZClassInd.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Mixture/Simple/Categorical/MultinomialStatistic.h"

namespace mixt
{

class MixtureComposer;

class ClassSampler
{
  public:
    ClassSampler(ZClassInd& ZClassInd,
                 const Matrix<Real>& tik,
                 int nbClass);

    /**
     * Sample new values for the missing variables of the given individual. Do not uses MixtureComposer::checkSampleCondition
     * to speed-up the sampling process. Used during rejection sampling.
     * @param i individual for which z must be sampled
     * */
    void sStepNoCheck(int i);
  private:
    int nbClass_;

    ZClassInd& zClassInd_;

    const Matrix<Real>& tik_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* MIXT_CLASSSAMPLER_H */
