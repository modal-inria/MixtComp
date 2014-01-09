/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::Clustering
 * created on: 4 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_MixtureFacade.cpp
 *  @brief In this file we implement the StrategyFacade class.
 **/

#include "../include/STK_MixtureFacade.h"
#include "../include/STK_MixtureStrategy.h"
#include "../include/STK_MixtureInit.h"
#include "../include/STK_IMixtureModelBase.h"

namespace STK
{

StrategyFacade::~StrategyFacade() { if (p_strategy_) delete p_strategy_;}

void StrategyFacade::createSimpleStrategy( Clust::initType init, int nbTrialInInit, Clust::algoType initAlgo, int nbInitIter, Real initEpsilon
                                        , int nbTry, Clust::algoType algo, int nbIter, Real epsilon)
{
  SimpleStrategyParam* p_param = new SimpleStrategyParam();
  p_param->nbTry_ = nbTry;
  p_param->p_algo_ = Clust::createAlgo(algo, nbIter, epsilon);

  SimpleStrategy* p_strategy = new SimpleStrategy(p_model_);
  p_strategy->setParam(p_param);
  p_strategy_ = p_strategy;
  p_strategy_->setMixtureInit(Clust::createInit(init, nbTrialInInit, initAlgo, nbInitIter, initEpsilon));
}
void StrategyFacade::createXemStrategy( Clust::initType init, int nbTrialInInit, Clust::algoType initAlgo, int nbInitIter, Real initEpsilon
                                     , int nbTry, int nbShortRun
                                     , Clust::algoType shortAlgo, int nbShortIter, Real shortEpsilon
                                     , Clust::algoType longAlgo, int nblongIter, Real longEpsilon)
{
  XemStrategyParam* p_param = new XemStrategyParam();
  p_param->nbTry_ = nbTry;
  p_param->nbShortRun_ = nbShortRun;
  p_param->p_shortAlgo_ = Clust::createAlgo(shortAlgo, nbShortIter, shortEpsilon);
  p_param->p_longAlgo_ = Clust::createAlgo(longAlgo, nblongIter, longEpsilon);

  XemStrategy* p_strategy = new XemStrategy(p_model_);
  p_strategy->setParam(p_param);
  p_strategy_ = p_strategy;
  p_strategy_->setMixtureInit(Clust::createInit(init, nbTrialInInit, initAlgo, nbInitIter, initEpsilon));
}

void StrategyFacade::createSemStrategy( Clust::initType init, int nbTrialInInit
                                        , int nbBurnInIter
                                        , int nbIter )
{
  SemStrategyParam* p_param = new SemStrategyParam();
  p_param->nbTry_ = 1;
  p_param->p_burnInAlgo_ = Clust::createAlgo(Clust::semAlgo_, nbBurnInIter, 0.);
  p_param->p_longAlgo_   = Clust::createAlgo(Clust::semAlgo_, nbIter      , 0.);

  SemStrategy* p_strategy= new SemStrategy(p_model_);
  p_strategy->setParam(p_param);
  p_strategy_ = p_strategy;
  p_strategy_->setMixtureInit(Clust::createInit(init, nbTrialInInit, Clust::semAlgo_, 0, 0.));
}

void StrategyFacade::run()
{
  if (p_strategy_)
  {
    p_model_->initializeModel();
    p_strategy_->run();
  }
}


}  // namespace STK




