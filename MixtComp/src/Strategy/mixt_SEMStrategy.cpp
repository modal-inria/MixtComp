/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sept 3, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff, serge.iovleff@stkpp.org
 **/

#include "mixt_SEMStrategy.h"
#include "../Various/mixt_Timer.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Various.h"
#include "../Various/mixt_Constants.h"

namespace mixt {

/** default constructor */
SemStrategy::SemStrategy(
    MixtureComposer* p_composer,
    const StrategyParam& param) :
        p_composer_(p_composer),
        param_(param),
        burnInAlgo_(p_composer, param.nbBurnInIter_),
        runAlgo_(p_composer, param.nbIter_) {}

std::string SemStrategy::run() {
  std::string warnLog;

  switch(param_.bias_) {
    case unBiased_: {
      warnLog += initSEMCheck();
      if (warnLog.size() > 0) { // if no initialization is possible, for example if it is not possible to have individuals in all classes, on enough variability in classes
        return warnLog;
      }

      RunProblemType runProb = runSEMCheck(rejectSampler_);
      if (runProb == invalidSampler_) {
        std::cout << "SemStrategy::run, switch to Gibbs sampler" << std::endl;

        initSEMCheck();
        runSEMCheck(GibbsSampler_);
      }
    }
    break;

    case biased_: {
      initSEMNoCheck();
      runSEMNoCheck();
    }
    break;
  }

  runGibbs();

  return warnLog;
}

std::string SemStrategy::initSEMCheck() {
  std::string warnLog;

  for (int n = 0; n < nbSamplingAttempts; ++n) { // multiple initialization attempts
    p_composer_->initData(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
    p_composer_->initParam(); // initialize Markov Chain for model parameters which need it

    if (n < nbSamplingAttempts - 1) {
      if (p_composer_->checkSampleCondition() == 1) { // log is not generated, since further trials are expected
//        p_composer_->mStep(unBiased_); // this mStep call ensure that all variable have a correct initialization of parameter, usable in the subsequent call to eStep
        break;
      }
    }
    else { // last trial, will generate a descriptive log if not successful
      std::string sWarn;
      if (p_composer_->checkSampleCondition(&sWarn) == 1) { // log is generated only during last trial
//        p_composer_->mStep(unBiased_); // this mStep call ensure that all variable have a correct initialization of parameter, usable in the subsequent call to eStep
      }
      else {
        std::stringstream sstm;
        sstm << "SemStrategy initializations " << nbSamplingAttempts << " trials have failed. The error log from the last initialization "
             << "trial is: " << std::endl
             << sWarn;
        warnLog += sstm.str();
      }
    }
  }

  return warnLog;
}

void SemStrategy::initSEMNoCheck() {
  p_composer_->initData(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
  p_composer_->initParam(); // initialize Markov Chain for model parameters which need it

//  p_composer_->mStep(biased_); // this mStep call ensure that all variable have a correct initialization of parameter, usable in the subsequent call to eStep
}

RunProblemType SemStrategy::runSEMCheck(SamplerType sampler) {
  RunProblemType prob = noProblem_;
  burnInAlgo_.runCheck(
      burnIn_,
      prob,
      sampler,
      0, // group
      3); // groupMax

  if (prob == invalidSampler_) { // no run is performed if there is an error during the burn-in
    return prob;
  }

  runAlgo_.runCheck(
      run_,
      prob,
      sampler,
      1, // group
      3); // groupMax

  return prob;
}

void SemStrategy::runSEMNoCheck() {
  burnInAlgo_.runNoCheck(
      burnIn_,
      0, // group
      3); // groupMax

  runAlgo_.runNoCheck(
      run_,
      1, // group
      3); // groupMax
}

void SemStrategy::runGibbs() {

  // note that initData is not called. Since parameters are known, using them result in a more efficient initialization

  p_composer_->gibbsSampling(
      callInitDataIfMarkovChain_,
      doNotSampleData_,
      param_.nbGibbsBurnInIter_,
      2, // group
      3); // groupMax

  p_composer_->gibbsSampling(
      doNotCallInitData_,
      sampleData_,
      param_.nbGibbsIter_,
      3, // group
      3); // groupMax
}

} // namespace mixt
