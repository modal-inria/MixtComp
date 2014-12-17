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
#include "../include/STK_IMixtureComposer.h"

namespace STK
{

StrategyFacade::~StrategyFacade() { if (p_strategy_) delete p_strategy_;}

void StrategyFacade::createSimpleStrategy( Clust::initType init, int nbTrialInInit, Clust::algoType initAlgo, int nbInitIter, Real initEpsilon
                                        , int nbTry, Clust::algoType algo, int nbIter, Real epsilon)
{
  SimpleStrategyParam* p_param = new SimpleStrategyParam();
  p_param->p_algo_ = Clust::createAlgo(algo, nbIter, epsilon);

  SimpleStrategy* p_strategy = new SimpleStrategy(p_model_);
  p_strategy->setNbTry(nbTry);
  p_strategy->setParam(p_param);
  p_strategy_ = p_strategy;
  p_strategy_->setMixtureInit(Clust::createInit(init, nbTrialInInit, initAlgo, nbInitIter, initEpsilon));
}

/* create a FullStrategy */
void StrategyFacade::createFullStrategy( Clust::initType init, int nbTryInInit, Clust::algoType initAlgo, int nbInitIter, Real initEpsilon
                       , int nbTry, int nbInitRun, int nbShortRun
                       , Clust::algoType shortAlgo, int nbShortIter, Real shortEpsilon
                       , Clust::algoType longAlgo, int nblongIter, Real longEpsilon)
{
  IMixtureAlgo* p_shortAlgo = Clust::createAlgo(shortAlgo, nbShortIter, shortEpsilon);
  IMixtureAlgo* p_longAlgo = Clust::createAlgo(longAlgo, nblongIter, longEpsilon);
  IMixtureInit* p_init = Clust::createInit(init, nbTryInInit, initAlgo, nbInitIter, initEpsilon);

  p_strategy_ = Clust::createFullStrategy(p_model_, nbTry, nbInitRun, p_init, nbShortRun, p_shortAlgo, p_longAlgo);
}

bool StrategyFacade::run()
{
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << _T("------------------------------\n")
           << _T("Entering StrategyFacade::run()\n");
#endif
  bool flag = false;
  if (p_strategy_)
  {
    // just check if the model is fresh or is already
    if (p_strategy_->run()) { flag = true;}
    else
    {
      msg_error_ += STKERROR_NO_ARG(StrategyFacade::run,strategy failed\n);
      msg_error_ += p_strategy_->error();
#ifdef STK_MIXTURE_VERBOSE
      stk_cout << _T("StrategyFacade:run() terminated without success.\n")
               << msg_error_ << _T("\n")
               << _T("------------------------------------------------\n");
#endif
    }
    p_model_->imputationStep();
    p_model_->finalizeStep();
  }
  else
  { msg_error_ = STKERROR_NO_ARG(MixtureFacade::run(),strategy is not set);}
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << _T("StrategyFacade:run() terminated.\np_model->lnLikelihood() =")
           << p_model_->lnLikelihood() << _T("\n")
           << _T("--------------------------------\n");
#endif
  return flag;
}


}  // namespace STK




