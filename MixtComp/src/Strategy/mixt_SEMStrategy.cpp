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
  if (warnLog.size() > 0) {
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
  for (int n = 0; n < nbSamplingAttempts; ++n) // multiple initialization attempts
  {
    p_composer_->intializeMixtureParameters(); // reset prop_ and tik_
    p_composer_->sStepNoCheck(); // initialization is done by reject sampling, no need for checkSampleCondition flag
    p_composer_->removeMissing(SEM_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet

    std::string sWarn;
    int proba = p_composer_->checkSampleCondition(&sWarn);

    if (proba == 1) // correct sampling is not rejected
    {
      /** first estimation of parameters, based on completions by p_composer_->sStep() and p_composer_->removeMissing(). The parameter
       * true indicates that this is the first call to mStep, and therefore eventual Markov chains (as it is the case for the Rank model for example)
       * should be initialized . */
      p_composer_->mStep(true);
      break;
    }
    else if (n == nbSamplingAttempts - 1) // proba == 0 in during last initialization attempt
    {
      std::stringstream sstm;
      sstm << "SemStrategy initializations " << nbSamplingAttempts << " trials have failed. The error log from the last initialization "
           << "trial is: " << std::endl
           << sWarn;
      warnLog += sstm.str();
    }
  }
  return warnLog;
}

RunProblemType SemStrategy::runSEM(SamplerType sampler) {
    RunProblemType prob = noProblem_;
    p_burnInAlgo_->run(burnIn_,
                       prob,
                       sampler,
                       0, // group
                       3); // groupMax

    if (prob == invalidSampler_) {
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
  p_composer_->sStepNoCheck(); // initialization is done by reject sampling, no need for checkSampleCondition flag
  p_composer_->removeMissing(Gibbs_); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
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

    p_composer_->sStepNoCheck();
    p_composer_->samplingStepNoCheck();
    p_composer_->eStep();
  }

  p_composer_->gibbsSampling(nbGibbsIter_,
                             3, // group
                             3); // groupMax
}

} // namespace mixt
