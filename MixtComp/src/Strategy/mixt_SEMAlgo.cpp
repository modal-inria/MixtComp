/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#include "mixt_SEMAlgo.h"
#include "../Various/mixt_Constants.h"
#include "../Various/mixt_Timer.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Various.h"

namespace mixt {

SEMAlgo::SEMAlgo(MixtureComposer* p_composer,
                 int nbIter) :
    p_composer_(p_composer),
    nbIter_(nbIter) {}

void SEMAlgo::runCheck(RunType runType,
                       RunProblemType& runPb,
                       SamplerType sampler,
                       int group,
                       int groupMax) {
  Timer myTimer;

  if (runType == burnIn_) {
    myTimer.setName("SEM: burn-in");
  }
  else if (runType == run_) {
    myTimer.setName("SEM: run");
  }

  for (int iter = 0; iter < nbIter_; ++iter) {
    myTimer.iteration(iter, nbIter_ - 1);
    writeProgress(group,
                  groupMax,
                  iter,
                  nbIter_ - 1);

    p_composer_->eStep();

    if (sampler == rejectSampler_) { // use reject sampling
      p_composer_->sStepNoCheck(); // no checkSampleCondition performed, to increase speed of sampling
      p_composer_->samplingStepNoCheck(doNotPerformInitialization_);

      int sampleCond = p_composer_->checkSampleCondition(); // since we are not in initialization, no need for log
      if (sampleCond == 0) { // sampled value rejected, switch to Gibbs sampler
        runPb = invalidSampler_;
        return;
      }
    }
    else { // use Gibbs sampling
      p_composer_->sStepCheck(); // checkSampleCondition is performed at each sampling, hence no need to call p_composer_->checkSampleCondition()
      p_composer_->samplingStepCheck();
    }

    p_composer_->mStep(unBiased_);

    if (runType == run_) {
      p_composer_->storeSEMRun(iter,
                               nbIter_ - 1);
    }
  }

  runPb = noProblem_;
}

void SEMAlgo::runNoCheck(RunType runType,
                         int group,
                         int groupMax) {
  Timer myTimer;

  if (runType == burnIn_) {
    myTimer.setName("SEM: burn-in");
  }
  else if (runType == run_) {
    myTimer.setName("SEM: run");
  }

  for (int iter = 0; iter < nbIter_; ++iter) {
    myTimer.iteration(iter, nbIter_ - 1);
    writeProgress(group,
                  groupMax,
                  iter,
                  nbIter_ - 1);

    p_composer_->eStep();

    p_composer_->sStepNoCheck(); // no checkSampleCondition performed, to increase speed of sampling
    p_composer_->samplingStepNoCheck(doNotPerformInitialization_);

    p_composer_->mStep(biased_);

    if (runType == run_) {
      p_composer_->storeSEMRun(iter,
                               nbIter_ - 1);
    }
  }
}

} // namespace mixt
