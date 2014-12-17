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

namespace mixt
{

/** default constructor */
SemStrategy::SemStrategy(mixt::MixtureComposer* p_composer,
                         int nbTry,
                         int nbBurnInIter,
                         int nbIter,
                         int nbGibbsBurnInIter,
                         int nbGibbsIter,
                         int nbSamplingAttempts) :
    p_composer_(p_composer),
    nbTry_(nbTry),
    nbGibbsBurnInIter_(nbGibbsBurnInIter),
    nbGibbsIter_(nbGibbsIter)
{
  p_burnInAlgo_ = new SEMAlgo(nbBurnInIter, nbSamplingAttempts);
  p_longAlgo_   = new SEMAlgo(nbIter      , nbSamplingAttempts);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy) :
    p_composer_(strategy.p_composer_),
    nbTry_(strategy.nbTry_),
    p_burnInAlgo_(strategy.p_burnInAlgo_),
    p_longAlgo_(strategy.p_longAlgo_)
{}

/** destructor */
SemStrategy::~SemStrategy()
{
  if (p_burnInAlgo_) delete p_burnInAlgo_;
  if (p_longAlgo_) delete p_longAlgo_;
}

std::string SemStrategy::run()
{
  std::string currWarn; // collect warning strings from all the steps
  std::string tempWarn; // warning for each run
  for (int iTry = 0; iTry < nbTry_; ++iTry)
  {
    // Random initialization
    p_composer_->randomClassInit();
#ifdef MC_DEBUG_NEW
  std::cout << "SemStrategy::run(), after randomClassInit()" << std::endl;
  std::cout << "*p_composer_->p_zi()" << std::endl;
  std::cout << *p_composer_->p_zi() << std::endl;
#endif
    p_composer_->initializeStep(); // optionnal step for mixtures that need it
    tempWarn = p_composer_->mStep();
    if (tempWarn != std::string())
    {
#ifdef MC_DEBUG_NEW
      std::cout << "\ttempWarn: " << tempWarn << std::endl;
#endif
      currWarn += tempWarn; // append warning to global warning
      continue; // make another try
    }

    // short run
#ifdef MC_DEBUG_NEW
    std::cout << "SemStrategy::run, short run" << std::endl;
#endif
    p_composer_->setState(burnIn_);
    p_burnInAlgo_->setModel(p_composer_);
    tempWarn = p_burnInAlgo_->run();
    if (tempWarn != std::string()) // an empty string means a successful run
    {
      currWarn += tempWarn; // append warning to global warning
      continue; // make another try
    }

    // long run
#ifdef MC_DEBUG_NEW
    std::cout << "SemStrategy::run, long run" << std::endl;
#endif
    p_composer_->setState(longRun_);
    p_longAlgo_->setModel(p_composer_);
    tempWarn = p_longAlgo_->run();
    if (tempWarn == std::string()) // an empty string means a successful run
    {
#ifdef MC_DEBUG_NEW
    std::cout << "\tp_longAlgo_->run() OK" << std::endl;
#endif
    }
    else
    {
      currWarn += tempWarn; // append warning to global warning
      continue; // make another try
    }
  
    Timer myTimer;
    myTimer.setName("Gibbs burn-in");
    for (int iterBurnInGibbs = 0; iterBurnInGibbs < nbGibbsBurnInIter_; ++iterBurnInGibbs)
    {
      myTimer.iteration(iterBurnInGibbs, nbGibbsBurnInIter_);
  #ifdef MC_DEBUG_NEW
      std::cout << "SemStrategy::run(), iterBurnInGibbs: " << iterBurnInGibbs << std::endl;
  #endif
      p_composer_->sStep();
      p_composer_->samplingStep();
      p_composer_->eStep();
    }

    p_composer_->gibbsSampling(nbGibbsIter_);

    p_composer_->finalizeStep();

    return currWarn;
  }

  currWarn += "Number of initialization attempts exhausted. Try again with more initializations or other parameters.\n";
  return currWarn;
}

} // namespace mixt
