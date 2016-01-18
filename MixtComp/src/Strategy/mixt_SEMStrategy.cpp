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

namespace mixt
{

/** default constructor */
SemStrategy::SemStrategy(MixtureComposer* p_composer,
                         int nbBurnInIter,
                         int nbIter,
                         int nbGibbsBurnInIter,
                         int nbGibbsIter) :
    p_composer_(p_composer),
    nbGibbsBurnInIter_(nbGibbsBurnInIter),
    nbGibbsIter_(nbGibbsIter)
{
  p_burnInAlgo_ = new SEMAlgo(p_composer,
                              nbBurnInIter);
  p_runAlgo_    = new SEMAlgo(p_composer,
                              nbIter);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy) :
    p_composer_        (strategy.p_composer_        ),
    nbGibbsBurnInIter_ (strategy.nbGibbsBurnInIter_ ),
    nbGibbsIter_       (strategy.nbGibbsIter_       )
{
  SEMAlgo& burnInAlgo = *strategy.p_burnInAlgo_;
  SEMAlgo& longAlgo   = *strategy.p_runAlgo_   ;
  p_burnInAlgo_ = new SEMAlgo(burnInAlgo);
  p_runAlgo_    = new SEMAlgo(longAlgo);
}

/** destructor */
SemStrategy::~SemStrategy()
{
  if (p_burnInAlgo_) delete p_burnInAlgo_;
  if (p_runAlgo_   ) delete p_runAlgo_   ;
}

std::string SemStrategy::run() {
  std::string warnLog;

  warnLog += initSEM();
  if (warnLog.size() > 0) { // if no initialization is possible, for example if it is not possible to have individuals in all classes, on enough variability in classes
    return warnLog;
  }

  RunProblemType runProb = runSEM(rejectSampler_);
  if (runProb == invalidSampler_) {
#ifdef MC_VERBOSE
      std::cout << "SemStrategy::run, switch to Gibbs sampler" << std::endl;
#endif

    initSEM();
    runSEM(GibbsSampler_);
  }

  initGibbs();
  runGibbs();

  return warnLog;
}

std::string SemStrategy::initSEM() {
  std::string warnLog;

  for (int n = 0; n < nbSamplingAttempts; ++n) { // multiple initialization attempts
    p_composer_->initializeProp(); // reset prop
    p_composer_->removeMissing(initParam_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet

    if (n < nbSamplingAttempts - 1) {
      if (p_composer_->checkSampleCondition() == 1) { // log is not generated, since further trials are expected
        p_composer_->mStep(true);
        break;
      }
    }
    else { // last trial
      std::string sWarn;
      if (p_composer_->checkSampleCondition(&sWarn) == 1) { // log is generated only during last trial
        p_composer_->mStep(true);
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

RunProblemType SemStrategy::runSEM(SamplerType sampler) {
#ifdef MC_DEBUG
  std::cout << "SemStrategy::runSEM" << std::endl;
#endif

    RunProblemType prob = noProblem_;
    p_burnInAlgo_->run(burnIn_,
                       prob,
                       sampler,
                       0, // group
                       3); // groupMax

    if (prob == invalidSampler_) { // no run is performed if there is an error during the burn-in
      return prob;
    }

    p_runAlgo_->run(run_,
                    prob,
                    sampler,
                    1, // group
                    3); // groupMax

    return prob;
}

void SemStrategy::initGibbs() {
  p_composer_->removeMissing(keepParam_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
}

void SemStrategy::runGibbs() {
  Timer myTimer;
  myTimer.setName("Gibbs burn-in");
  for (int iterBurnInGibbs = 0; iterBurnInGibbs < nbGibbsBurnInIter_; ++iterBurnInGibbs)
  {
    myTimer.iteration(iterBurnInGibbs, nbGibbsBurnInIter_ - 1);
    writeProgress(2,
                  3,
                  iterBurnInGibbs,
                  nbGibbsBurnInIter_ - 1);

    p_composer_->eStep();
    p_composer_->sStepNoCheck(); // during Gibbs no check is performed, as there is no parameter estimation. Note the following samplingStepNoCheck().
    p_composer_->samplingStepNoCheck();

  }

  p_composer_->gibbsSampling(nbGibbsIter_,
                             3, // group
                             3); // groupMax
}

} // namespace mixt
