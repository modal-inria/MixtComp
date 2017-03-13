/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 4, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CLASSSAMPLER_H
#define MIXT_CLASSSAMPLER_H

#include "../Composer/mixt_ZClassInd.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{

class MixtureComposer;

class ClassSampler
{
  public:
    ClassSampler(const MixtureComposer& composer,
                 ZClassInd& ZClassInd,
                 const Matrix<Real>& tik,
                 int nbClass);

    /**
     * Sample new values for the missing variables of the given individual. Using MixtureComposer::checkSampleCondition
     * to check if each value of z_i is valid or not. Used during sampling, and false during rejection sampling
     * @param i individual for which z must be sampled
     * */
    void sStepCheck  (int i);

    /**
     * Sample new values for the missing variables of the given individual. Do not uses MixtureComposer::checkSampleCondition
     * to speed-up the sampling process. Used during rejection sampling.
     * @param i individual for which z must be sampled
     * */
    void sStepNoCheck(int i);
  private:
    const MixtureComposer& composer_;

    int nbClass_;

    ZClassInd& zClassInd_;

    const Matrix<Real>& tik_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* MIXT_CLASSSAMPLER_H */
