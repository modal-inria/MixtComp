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
 * Project:  stkpp::
 * created on: 3 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_MixtureStrategy.cpp
 *  @brief In this file we implement the strategies for estimating mixture model.
 **/

#include "STKernel/include/STK_Exceptions.h"
#include "../include/STK_MixtureStrategy.h"
#include "../include/STK_MixtureInit.h"
#include "../include/STK_MixtureAlgo.h"
#include "../include/STK_IMixtureComposerBase.h"

namespace STK
{

/* copy constructor
 *  @param strategy the strategy to copy
 **/
IMixtureStrategy::IMixtureStrategy( IMixtureStrategy const& strategy)
                                  : IRunnerBase(strategy), nbTry_(strategy.nbTry_)
                                  , p_model_(strategy.p_model_)
                                  , p_init_(strategy.p_init_->clone())
{}

/* destructor */
IMixtureStrategy::~IMixtureStrategy() { if (p_init_) delete p_init_;}

/* destructor */
SimpleStrategyParam::~SimpleStrategyParam()
{ if (p_algo_) delete p_algo_;}

/* destructor */
XemStrategyParam::~XemStrategyParam()
{
  if (p_shortAlgo_) delete p_shortAlgo_;
  if (p_longAlgo_) delete p_longAlgo_;
}

/* run the simple strategy */
bool SimpleStrategy::run()
{
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << _T("-----------------------------------------------\n");
  stk_cout << _T("Entering SimpleStrategy::run() with:\n")
           << _T("nbTry_ = ") << this->nbTry_ << _T("\n\n");
#endif
  // initialize bestLikelihood
  Real bestLikelihood = -STK::Arithmetic<Real>::max();
  try
  {
    IMixtureComposerBase* p_currentModel = p_model_->create();
    // find best of the shortModel and save it in p_currentmodellong
    for (int iTry = 0; iTry < this->nbTry_; ++iTry)
    {
      // intialize current model
      p_init_->setModel(p_currentModel);
      if (p_init_->run())
      {
#ifdef STK_MIXTURE_VERBOSE
        stk_cout << _T("iTry =") << iTry << _T(". In SimpleStrategy::run(), Init step terminated. Current model:\n");
        p_currentModel->writeParameters(stk_cout);
        stk_cout << _T("\n\n");
#endif
        // run the estimation algorithm
        p_param_->p_algo_->setModel(p_currentModel);
        if (p_param_->p_algo_->run())
        {
#ifdef STK_MIXTURE_VERBOSE
            stk_cout << _T("iTry =") << iTry << ". In SimpleStrategy::run(), Long run terminated. current model\n";
            p_currentModel->writeParameters(stk_cout);
            stk_cout << _T("\n\n");
#endif
          // Check if we get a better result
          if(bestLikelihood < p_currentModel->lnLikelihood())
          {
            std::swap(p_currentModel, p_model_);
            bestLikelihood = p_model_->lnLikelihood();
            bestLikelihood = p_currentModel->lnLikelihood();
#ifdef STK_MIXTURE_VERBOSE
            stk_cout << "In SimpleStrategy::run(), model overwritten. Model is:\n";
            stk_cout << "bestlikelihood =" << bestLikelihood << "\n";
            p_model_->writeParameters(stk_cout);
            stk_cout << _T("\n\n");
#endif
          }
        }  // algo step
      } // init step
    } // iTry
    delete p_currentModel;
  } catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << "SimpleStrategy::run() terminated. current model\n";
  p_model_->writeParameters(stk_cout);
  stk_cout << "-----------------------------------------------\n";
#endif
  if (bestLikelihood == -STK::Arithmetic<Real>::max())
  {
    msg_error_ = STKERROR_NO_ARG(In SimpleStrategy::run,All trials failed);
    return false;
  }
  return true;
}

/* run the xem strategy */
bool XemStrategy::run()
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering XemStrategy::run() with:\n")
           << _T("nbTry_ = ") << this->nbTry_ << _T("\n")
           << _T("nbShortRun_ = ") << p_param_->nbShortRun_ << _T("\n");
#endif
  // initialize bestModel and bestLikelihood
  Real bestLikelihood = - STK::Arithmetic<Real>::max();
  try
  {
    // the current model is used in the short runs
    IMixtureComposerBase* p_currentModel = p_model_->create();
    IMixtureComposerBase* p_currentBestModel = p_model_->create();
    for (int iTry = 0; iTry < this->nbTry_; ++iTry)
    {
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("-------------------------------\n")
           << _T("try number = ") << iTry << _T("\n")
           << _T("-------------------------------\n");
#endif
      Real currentBestlikelihood = - STK::Arithmetic<Real>::max();
      // find best of the shortModel and save it in p_currentBestModel
      for (int iShortRun = 0; iShortRun < p_param_->nbShortRun_; ++iShortRun)
      {
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("-------------------------------\n")
           << _T("short run number = ") << iShortRun << _T("\n");
#endif
        // initialize current model
        p_init_->setModel(p_currentModel);
        if (p_init_->run())
        {
          // perform short run on the current model
          p_param_->p_shortAlgo_->setModel(p_currentModel);
          p_param_->p_shortAlgo_->run();
#ifdef STK_MIXTURE_VERBOSE
        stk_cout << _T("iTry =") << iTry << ". In XemStrategy::run(), short run terminated. current model\n";
            p_currentModel->writeParameters(stk_cout);
            stk_cout << _T("\n\n");
#endif
          // if we get a better result, swap it with currentBestModel
          if( currentBestlikelihood<p_currentModel->lnLikelihood())
          {
            std::swap(p_currentModel, p_currentBestModel);
            currentBestlikelihood = p_currentBestModel->lnLikelihood();
          }
        } // initialization
#ifdef STK_MIXTURE_VERY_VERBOSE
      else
        stk_cout << _T("short run failed.") << _T("\n");
#endif
      } // iShortRun
      // in case there is no short runs successful or nbShortRun_==0
      // try to initialize bestCurrentModel, otherwise go to a next try
      if (currentBestlikelihood == - STK::Arithmetic<Real>::max())
      {
        // initialize current model
        p_init_->setModel(p_currentBestModel);
        if (!p_init_->run())
        {
#ifdef STK_MIXTURE_VERY_VERBOSE
        stk_cout << _T("In XemStrategy::run(), All Initialization and short runs failed. ")
                 << _T("iTry =")<< iTry << _T("\n");
#endif
          continue; // model not initialized, we go to the next trial
        }
      }
#ifdef STK_MIXTURE_VERBOSE
      stk_cout << _T("iTry =") << iTry
               << _T(". In XemStrategy::run(), short run terminated. best model:\n");
      p_currentBestModel->writeParameters(stk_cout);
      stk_cout << _T("\n\n");
#endif
      // start a long run with the better model
      p_param_->p_longAlgo_->setModel(p_currentBestModel);
      if (p_param_->p_longAlgo_->run())
      {
#ifdef STK_MIXTURE_VERBOSE
        stk_cout << _T("iTry =") << iTry
                 << _T(". In XemStrategy::run(), Long run terminated. Current model:\n");
        p_currentBestModel->writeParameters(stk_cout);
        stk_cout << _T("\n\n");
#endif
        if(bestLikelihood<p_currentBestModel->lnLikelihood())
        {
          std::swap(p_model_, p_currentBestModel);
          bestLikelihood = p_model_->lnLikelihood();
        }
      }
#ifdef STK_MIXTURE_VERBOSE
            stk_cout << "In XemStrategy::run(), Long run terminated. current model\n";
            p_model_->writeParameters(stk_cout);
            stk_cout << _T("\n\n");
#endif
    } // end iTry
    delete p_currentBestModel;
    delete p_currentModel;
  } catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << "XemStrategy::run() terminated. current model\n";
  p_model_->writeParameters(stk_cout);
  stk_cout << "-----------------------------------------------\n";
#endif
  if (bestLikelihood == -STK::Arithmetic<Real>::max())
  {
    msg_error_ = STKERROR_NO_ARG(In XemStrategy::run,All trials failed);
    return false;
  }
  return true;
}

} // namespace STK



