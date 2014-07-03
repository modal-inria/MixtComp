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

namespace mixt
{

/** default constructor */
SemStrategy::SemStrategy(mixt::MixtureComposer*& p_composer,
                         STK::Clust::initType init,
                         int nbTry,
                         int nbTrialInInit,
                         int nbBurnInIter,
                         int nbIter,
                         int nbGibbsIter,
                         int zMin,
                         int nbSamplingAttempts) :
    p_composer_(p_composer),
    nbTry_(nbTry),
    nbGibbsIter_(nbGibbsIter)
{
  p_init_ = STK::Clust::createInit(init,
                                   nbTrialInInit,
                                   STK::Clust::semAlgo_,
                                   0,
                                   0.);
  p_burnInAlgo_ = new SEMAlgo(nbBurnInIter, zMin, nbSamplingAttempts);
  p_longAlgo_   = new SEMAlgo(nbIter      , zMin, nbSamplingAttempts);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy) :
    p_composer_(strategy.p_composer_),
    p_init_(strategy.p_init_->clone()),
    nbTry_(strategy.nbTry_),
    p_burnInAlgo_(strategy.p_burnInAlgo_),
    p_longAlgo_(strategy.p_longAlgo_)
{}

/** destructor */
SemStrategy::~SemStrategy()
{
  if (p_init_) delete p_init_;
  if (p_burnInAlgo_) delete p_burnInAlgo_;
  if (p_longAlgo_) delete p_longAlgo_;
}

bool SemStrategy::run()
{
  for (int iTry = 0; iTry < nbTry_; ++iTry)
  {
    // initialize current model
    p_init_->setModel(p_composer_);
    p_composer_->setState(STK::Clust::modelInitialized_);
    if (!p_init_->run())
      continue; // make another try

    // short run
    p_burnInAlgo_->setModel(p_composer_);
    if (!p_burnInAlgo_->run())
      continue;
    p_composer_->setState(STK::Clust::shortRun_);

    // long run
    p_longAlgo_->setModel(p_composer_);
    if (!p_longAlgo_->run())
      continue;
    else
    {
      p_composer_->setState(STK::Clust::longRun_);
      break; // no need to further try if a run is complete
    }
  }
  
  for (int iterGibbs = 0; iterGibbs < nbGibbsIter_; ++iterGibbs)
  {
      p_composer_->sStep();
      p_composer_->samplingStep();
      p_composer_->eStep();
      p_composer_->storeData();
  }

  p_composer_->finalizeStep();

  return true;
}

} // namespace mixt
