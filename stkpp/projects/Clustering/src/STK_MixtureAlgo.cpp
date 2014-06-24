/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 16 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureAlgo.cpp
 *  @brief In this file we implement the run method of the mixture algorithms.
 **/

#include "STKernel/include/STK_Exceptions.h"
#include "../include/STK_MixtureAlgo.h"
#include "../include/STK_IMixtureComposerBase.h"

namespace STK
{

/* run the CEM allgorithm */
bool CEMAlgo::run()
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering CEMAlgo::run() with:\n")
           << _T("nbIterMax_ = ") << nbIterMax_ << _T("\n")
           << _T("epsilon_ = ") << epsilon_ << _T("\n");
#endif
  try
  {
    Real currentLnLikelihood =  p_model_->lnLikelihood();
    for (int iter = 0; iter < nbIterMax_; iter++)
    {
      if (!p_model_->cStep())
      {
        msg_error_ = STKERROR_NO_ARG(CEMAlgo::run,No more individuals in a class);
        return false;
      }
      p_model_->pStep();
      p_model_->imputationStep();
      p_model_->mStep();
      p_model_->eStep();
      Real lnLikelihood = p_model_->lnLikelihood();
      if (std::abs(lnLikelihood - currentLnLikelihood) < epsilon_)
      {
#ifdef STK_MIXTURE_VERY_VERBOSE
        stk_cout << _T("Terminating CEMAlgo::run() with:\n")
                 << _T("iter = ") << iter << _T("\n")
                 << _T("delta = ") << lnLikelihood - currentLnLikelihood << _T("\n");
#endif
        break;
      }
      currentLnLikelihood = lnLikelihood;
    }
  }
  catch (Clust::exceptions const& error)
  {
    msg_error_ = Clust::exceptionToString(error);
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("An error occur in CEM algorithm: ") << msg_error_ << _T("\n");
#endif
    return false;
  }
  return true;
}

bool EMAlgo::run()
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering EMAlgo::run() with:\n")
           << _T("nbIterMax_ = ") << nbIterMax_ << _T("\n")
           << _T("epsilon_ = ") << epsilon_ << _T("\n");
#endif

  try
  {
    Real currentLnLikelihood = p_model_->lnLikelihood();
    for (int iter = 0; iter < nbIterMax_; iter++)
    {
      p_model_->pStep();
      p_model_->imputationStep();
      p_model_->mStep();
      p_model_->eStep();
      Real lnLikelihood = p_model_->lnLikelihood();
      // no abs as the likelihood should increase
      if ( (lnLikelihood - currentLnLikelihood) < epsilon_)
      {
#ifdef STK_MIXTURE_VERY_VERBOSE
        stk_cout << _T("Terminating EMAlgo::run() with:\n")
                 << _T("iter = ") << iter << _T("\n")
                 << _T("delta = ") << lnLikelihood - currentLnLikelihood << _T("\n");
#endif
        break;
      }
      currentLnLikelihood = lnLikelihood;
    }
  }
  catch (Clust::exceptions const& error)
  {
    msg_error_ = Clust::exceptionToString(error);
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("An error occur in EM algorithm: ") << msg_error_ << _T("\n");
#endif
    return false;
  }
  return true;
}

bool SEMAlgo::run()
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering SEMAlgo::run() with:\n")
           << _T("nbIterMax_ = ") << nbIterMax_ << _T("\n")
           << _T("epsilon_ = ") << epsilon_ << _T("\n");
#endif
  try
  {
    for (int iter = 0; iter < this->nbIterMax_; ++iter)
    {
      if (!p_model_->sStep())
      {
        msg_error_ = STKERROR_NO_ARG(SEMAlgo::run,No more individuals in a class);
        return false;
      }
      p_model_->pStep();
      p_model_->samplingStep();
      p_model_->mStep();
      p_model_->eStep();
    }
  }
  catch (Clust::exceptions const& error)
  {
    msg_error_ = Clust::exceptionToString(error);
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("An error occur in SEM algorithm: ") << msg_error_ << _T("\n");
#endif
    return false;
  }
  return true;
}

} // namespace STK
