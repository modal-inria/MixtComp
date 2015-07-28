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

/** @file mixt_MixtureStrategy.cpp
 *  @brief In this file we implement the strategies for estimating mixture model.
 **/

#include "mixt_SEMStrategy.h"
#include "../Various/mixt_Timer.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Various.h"

namespace mixt
{

/** default constructor */
SemStrategy::SemStrategy(MixtureComposer* p_composer,
                         int nbTrialInInit_,
                         int nbBurnInIter,
                         int nbIter,
                         int nbGibbsBurnInIter,
                         int nbGibbsIter,
                         int nbSamplingAttempts) :
    p_composer_(p_composer),
    nbTrialInInit_(nbTrialInInit_),
    nbGibbsBurnInIter_(nbGibbsBurnInIter),
    nbGibbsIter_(nbGibbsIter),
    nbSamplingAttempts_(nbSamplingAttempts)
{
  p_burnInAlgo_ = new SEMAlgo(p_composer,
                              nbBurnInIter,
                              nbSamplingAttempts);
  p_longAlgo_   = new SEMAlgo(p_composer,
                              nbIter,
                              nbSamplingAttempts);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy) :
    p_composer_        (strategy.p_composer_),
    nbTrialInInit_     (strategy.nbTrialInInit_),
    nbGibbsBurnInIter_ (strategy.nbGibbsBurnInIter_),
    nbGibbsIter_       (strategy.nbGibbsIter_),
    nbSamplingAttempts_(strategy.nbSamplingAttempts_)
{
  SEMAlgo& burnInAlgo = *strategy.p_burnInAlgo_;
  SEMAlgo& longAlgo   = *strategy.p_longAlgo_  ;
  p_burnInAlgo_ = new SEMAlgo(burnInAlgo);
  p_longAlgo_   = new SEMAlgo(longAlgo);
}

/** destructor */
SemStrategy::~SemStrategy()
{
  if (p_burnInAlgo_) delete p_burnInAlgo_;
  if (p_longAlgo_  ) delete p_longAlgo_  ;
}

std::string SemStrategy::run()
{
  std::string allWarn; // collect warning strings from all the trials
  DegeneracyType currDeg = noDeg_; // current type of degeneracy

  for (int iTry = 0; iTry < nbTrialInInit_; ++iTry) // trials on the whole SEM chain
  {
    std::string tryWarn; // warning for each trial
#ifdef MC_DEBUG
  std::cout << "SemStrategy::run(), after randomClassInit()" << std::endl;
  std::cout << "*p_composer_->p_zi()" << std::endl;
  std::cout << *p_composer_->p_zi() << std::endl;
#endif

    if (currDeg == noDeg_ || currDeg == strongDeg_) // only reset everything when needed. softDeg_ does not trigger such a reinitialization
    {
      p_composer_->intializeMixtureParameters(); // reset prop_, tik_ and zi_.data_

      tryWarn = p_composer_->sStepNbAttempts(nbSamplingAttempts_); // perform at max nbSamplingAttempts_ calls to p_composer_->sStep();
      if (tryWarn.size() > 0)
      {
        std::stringstream sstm;
        sstm << "SemStrategy error, initial partition, iTry: " << iTry << std::endl
             << tryWarn;
        allWarn += sstm.str(); // append warning to global warning
        continue; // make another try
      }

      p_composer_->removeMissing(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet

      tryWarn = p_composer_->mStep(); // first estimation of parameters, based on completions by p_composer_->sStep() and p_composer_->removeMissing()
      if (tryWarn.size() > 0)
      {
        std::stringstream sstm;
        sstm << "SemStrategy error, initial mStep, iTry: " << iTry << std::endl
             << tryWarn;
        allWarn += sstm.str(); // append warning to global warning
        continue; // make another try
      }
    }

#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, SEM burn-in" << std::endl;
#endif
    tryWarn = p_burnInAlgo_->run(burnIn_,
                                 currDeg,
                                 0, // group
                                 3); // groupMax
    if (tryWarn.size() > 0) // an empty string means a successful run
    {
      std::stringstream sstm;
      sstm << "SemStrategy error, SEM burn-in, iTry: " << iTry << std::endl
           << tryWarn;
      allWarn += sstm.str(); // append warning to global warning
      continue; // make another try
    }

#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, SEM run" << std::endl;
#endif
    tryWarn = p_longAlgo_->run(run_,
                               currDeg,
                               1, // group
                               3); // groupMax
    if (tryWarn.size() > 0) // an empty string means a successful run
    {
      std::stringstream sstm;
      sstm << "SemStrategy error, SEM run, iTry: " << iTry << std::endl
           << tryWarn;
      allWarn += sstm.str(); // append warning to global warning
      continue; // make another try
    }
  
#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, Gibbs burn-in" << std::endl;
#endif
    Timer myTimer;
    myTimer.setName("Gibbs burn-in");
    for (int iterBurnInGibbs = 0; iterBurnInGibbs < nbGibbsBurnInIter_; ++iterBurnInGibbs)
    {
      myTimer.iteration(iterBurnInGibbs, nbGibbsBurnInIter_ - 1);
      writeProgress(2,
                    3,
                    iterBurnInGibbs,
                    nbGibbsBurnInIter_ - 1);
  #ifdef MC_DEBUG
      std::cout << "SemStrategy::run(), iterBurnInGibbs: " << iterBurnInGibbs << std::endl;
  #endif
      p_composer_->sStep();
      p_composer_->samplingStep();
      p_composer_->eStep();
    }

#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, Gibbs run" << std::endl;
#endif
    p_composer_->gibbsSampling(nbGibbsIter_,
                               3, // group
                               3); // groupMax

    return std::string(); // if the last attempt is a success, ignore all warnings in allWarn and return an empty string
  }

  allWarn += "Number of initialization attempts exhausted. Try again with more initializations or other parameters.\n";
  return allWarn;
}

} // namespace mixt
