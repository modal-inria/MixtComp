/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
SemStrategy::SemStrategy(MixtureComposer* p_composer,
                         const StrategyParam& param) :
    p_composer_(p_composer),
    param_(param) {
  p_burnInAlgo_ = new SEMAlgo(p_composer,
                              param.nbBurnInIter_);
  p_runAlgo_    = new SEMAlgo(p_composer,
                              param.nbIter_);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy) :
    p_composer_(strategy.p_composer_),
    param_(strategy.param_) {
  p_burnInAlgo_ = new SEMAlgo(p_composer_,
                              param_.nbBurnInIter_);
  p_runAlgo_    = new SEMAlgo(p_composer_,
                              param_.nbIter_);
}

/** destructor */
SemStrategy::~SemStrategy() {
  if (p_burnInAlgo_) delete p_burnInAlgo_;
  if (p_runAlgo_   ) delete p_runAlgo_   ;
}

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
    #ifdef MC_VERBOSE
          std::cout << "SemStrategy::run, switch to Gibbs sampler" << std::endl;
    #endif

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

  initGibbs();
  runGibbs();

  return warnLog;
}

std::string SemStrategy::initSEMCheck() {
  std::string warnLog;

  for (int n = 0; n < nbSamplingAttempts; ++n) { // multiple initialization attempts
    p_composer_->initializeProp(); // reset prop
    p_composer_->removeMissing(initParam_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet

    if (n < nbSamplingAttempts - 1) {
      if (p_composer_->checkSampleCondition() == 1) { // log is not generated, since further trials are expected
        p_composer_->mStep(unBiased_); // this mStep call ensure that all variable have a correct initialization of parameter, usable in the subsequent call to eStep
        break;
      }
    }
    else { // last trial, will generate a descriptive log if not successful
      std::string sWarn;
      if (p_composer_->checkSampleCondition(&sWarn) == 1) { // log is generated only during last trial
        p_composer_->mStep(unBiased_); // this mStep call ensure that all variable have a correct initialization of parameter, usable in the subsequent call to eStep
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
  p_composer_->initializeProp(); // reset prop
  p_composer_->removeMissing(initParam_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
  p_composer_->mStep(biased_); // this mStep call ensure that all variable have a correct initialization of parameter, usable in the subsequent call to eStep
}

RunProblemType SemStrategy::runSEMCheck(SamplerType sampler) {
  RunProblemType prob = noProblem_;
  p_burnInAlgo_->runCheck(burnIn_,
                          prob,
                          sampler,
                          0, // group
                          3); // groupMax

  if (prob == invalidSampler_) { // no run is performed if there is an error during the burn-in
    return prob;
  }

  p_runAlgo_->runCheck(run_,
                       prob,
                       sampler,
                       1, // group
                       3); // groupMax

  return prob;
}

void SemStrategy::runSEMNoCheck() {
  p_burnInAlgo_->runNoCheck(burnIn_,
                            0, // group
                            3); // groupMax

  p_runAlgo_->runNoCheck(run_,
                         1, // group
                         3); // groupMax
}

void SemStrategy::initGibbs() {
  p_composer_->removeMissing(keepParam_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
}

void SemStrategy::runGibbs() {
  Timer myTimer;
  myTimer.setName("Gibbs burn-in");
  for (int it = 0; it < param_.nbGibbsBurnInIter_; ++it) {
    myTimer.iteration(it,
                      param_.nbGibbsBurnInIter_ - 1);
    writeProgress(2,
                  3,
                  it,
                  param_.nbGibbsBurnInIter_ - 1);

    p_composer_->eStep();
    p_composer_->sStepNoCheck(); // during Gibbs no check is performed, as there is no parameter estimation. Note the following samplingStepNoCheck().
    p_composer_->samplingStepNoCheck();

  }

  p_composer_->gibbsSampling(param_.nbGibbsIter_,
                             3, // group
                             3); // groupMax
}

} // namespace mixt
