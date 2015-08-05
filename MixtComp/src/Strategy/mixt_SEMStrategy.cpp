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
                         int nbGibbsIter,
                         int nbSamplingAttempts) :
    p_composer_(p_composer),
    nbGibbsBurnInIter_(nbGibbsBurnInIter),
    nbGibbsIter_(nbGibbsIter),
    nbSamplingAttempts_(nbSamplingAttempts)
{
  p_burnInAlgo_ = new SEMAlgo(p_composer,
                              nbBurnInIter,
                              nbSamplingAttempts);
  p_runAlgo_    = new SEMAlgo(p_composer,
                              nbIter,
                              nbSamplingAttempts);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy) :
    p_composer_        (strategy.p_composer_),
    nbGibbsBurnInIter_ (strategy.nbGibbsBurnInIter_),
    nbGibbsIter_       (strategy.nbGibbsIter_),
    nbSamplingAttempts_(strategy.nbSamplingAttempts_)
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

std::string SemStrategy::run()
{
  for (int n = 0; n < nbSamplingAttempts_; ++n) // multiple initialization attempts
  {
    p_composer_->intializeMixtureParameters(); // reset prop_, tik_ and zi_.data_
    p_composer_->sStep(false); // initialization is done by reject sampling, no need for checkSampleCondition flag
    p_composer_->removeMissing(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
    std::string mWarn = p_composer_->mStep(); // first estimation of parameters, based on completions by p_composer_->sStep() and p_composer_->removeMissing(). Warnlog is updated to trigger resample in case of soft degeneracy.

    std::string sWarn;
    Real proba = p_composer_->checkSampleCondition(&sWarn);

    if (mWarn.size() == 0 || proba == 1.) // correct sampling is not rejected
    {
      break; // no need for further sampling
    }
    else if ((mWarn.size() > 0 || proba == 0.) && n == nbSamplingAttempts - 1) // error in mStep or conditions on sample at the last sampling
    {
      std::stringstream sstm;
      sstm << "SemStrategy initializations " << nbSamplingAttempts_ << " trials have failed. The error log from the last initialization "
           << "trial is: " << std::endl
           << sWarn;
      return sstm.str();
    }
  }

  std::string allWarn; // collect warning strings from all the trials, only returned if either maxLightDegTry or maxStrongDegTry are exhausted, whichever condition come first

  int nbDegeneracy = 0; // number of "soft" degeneracies, for example with pi = 0 for the Ordinal model
  while(nbDegeneracy < maxSoftDegTry)
  {
#ifdef MC_VERBOSE
    std::cout << "SemStrategy::run, nbDegeneracy: " << nbDegeneracy << ", maxSoftDegTry: " << maxSoftDegTry << std::endl;
#endif

    std::string tryWarn; // warning for each trial

#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, SEM burn-in" << std::endl;
#endif

    tryWarn = p_burnInAlgo_->run(burnIn_,
                                 0, // group
                                 3); // groupMax
    if (tryWarn.size() > 0)
    {
      ++nbDegeneracy;
      std::stringstream sstm;
      sstm << "SemStrategy error, SEM burn-in, nbDegeneracy: " << nbDegeneracy << std::endl
           << tryWarn;
      allWarn += sstm.str(); // append warning to global warning

#ifdef MC_DEBUG
        std::cout << sstm.str() << std::endl;
#endif

      continue; // make another try
    }

#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, SEM run" << std::endl;
#endif

    tryWarn = p_runAlgo_->run(run_,
                              1, // group
                              3); // groupMax
    if (tryWarn.size() > 0)
    {
      ++nbDegeneracy;
      std::stringstream sstm;
      sstm << "SemStrategy error, SEM run, nbDegeneracy: " << nbDegeneracy << std::endl
           << tryWarn;
      allWarn += sstm.str(); // append warning to global warning

#ifdef MC_DEBUG
        std::cout << sstm.str() << std::endl;
#endif

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
      p_composer_->sStep(false);
      p_composer_->samplingStep(false);
      p_composer_->eStep(false);
    }

#ifdef MC_DEBUG
    std::cout << "SemStrategy::run, Gibbs run" << std::endl;
#endif

    p_composer_->gibbsSampling(nbGibbsIter_,
                               3, // group
                               3); // groupMax

    return std::string(); // if the last attempt is a success, ignore all warnings in allWarn and return an empty string
  }

  allWarn += "Number of initialization attempts exhausted.\n";
  return allWarn;
}

} // namespace mixt
