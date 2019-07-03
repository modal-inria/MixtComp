/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: May 4, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_COMPOSER_CLASSSAMPLER_H
#define LIB_COMPOSER_CLASSSAMPLER_H

#include <Composer/ZClassInd.h>
#include "../LinAlg/LinAlg.h"
#include "../Data/AugmentedData.h"
#include "../Statistic/MultinomialStatistic.h"

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

#endif /* CLASSSAMPLER_H */
