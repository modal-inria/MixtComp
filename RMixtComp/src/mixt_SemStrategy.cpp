/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff, Vincent KUBICKI

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  MixtComp
 * created on: 3 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org, kubicki, vincent.kubicki@inria.fr
 **/

/** @file mixt_MixtureStrategy.cpp
 *  @brief In this file we implement the strategies for estimating mixture model.
 **/

#include "stkpp/projects/Clustering/include/STK_IMixtureComposerBase.h"
#include "stkpp/projects/Clustering/include/STK_MixtureAlgo.h"
#include "stkpp/projects/Clustering/include/STK_MixtureInit.h"
#include "mixt_SemStrategy.h"

namespace mixt
{

/** default constructor */
SemStrategy::SemStrategy(STK::IMixtureComposerBase*& p_composer,
                         STK::Clust::initType init,
                         int nbTrialInInit,
                         int nbBurnInIter,
                         int nbIter)
        : p_composer_(p_composer),
        nbTry_(1)
{
  p_init_ = STK::Clust::createInit(init,
                                   nbTrialInInit,
                                   STK::Clust::semAlgo_,
                                   0,
                                   0.);
  p_burnInAlgo_ = STK::Clust::createAlgo(STK::Clust::semAlgo_, nbBurnInIter, 0.);
  p_longAlgo_   = STK::Clust::createAlgo(STK::Clust::semAlgo_, nbIter      , 0.);
}

/** copy constructor */
SemStrategy::SemStrategy(SemStrategy const& strategy)
        : p_composer_(strategy.p_composer_),
        p_init_(strategy.p_init_->clone()),
        nbTry_(0),
        p_burnInAlgo_(0),
        p_longAlgo_(0)
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
  p_composer_->initializeModel();

  for (int iTry = 0; iTry < nbTry_; ++iTry)
  {
    // initialize current model
    p_init_->setModel(p_composer_);
    p_composer_->setState(STK::Clust::modelInitialized_);
    if (p_init_->run())
    {
      p_burnInAlgo_->setModel(p_composer_);
      p_burnInAlgo_->run();
      p_composer_->setState(STK::Clust::shortRun_);
    }
    
    p_longAlgo_->setModel(p_composer_);
    if (p_longAlgo_->run())
    {
      p_composer_->setState(STK::Clust::longRun_);
    }
  }
  
  p_composer_->finalizeStep();
    
  return true;
}

} // namespace mixt
