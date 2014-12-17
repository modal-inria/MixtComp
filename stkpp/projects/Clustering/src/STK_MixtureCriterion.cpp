/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * created on: 19 sept. 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_MixtureCriterion.cpp
 *  @brief In this file we implement the run method of the MixtureCriterion classes.
 **/

#include "../include/STK_MixtureCriterion.h"

namespace STK
{

//* Compute AIC Criterion */
bool AICMixtureCriterion::run()
{
  if (!p_composer_)
  { msg_error_ = STKERROR_NO_ARG(AICMixtureCriterion::run,p_composer_ is not set);
    return false;
  }
  value_  = p_composer_->computeAIC();
  return true;
}

//* Compute BIC Criterion */
bool BICMixtureCriterion::run()
{
  if (!p_composer_)
  { msg_error_ = STKERROR_NO_ARG(BICMixtureCriterion::run,p_composer_ is not set);
    return false;
  }
  value_  = p_composer_->computeBIC();
  return true;
}

/* implementation of the virtual method run */
bool ICLMixtureCriterion::run()
{
  if (!p_composer_)
  { msg_error_ = STKERROR_NO_ARG(ICLMixtureCriterion::run,p_composer_ is not set);
    return false;
  }
  value_  = p_composer_->computeICL();
  return true;
}

} // namespace STK


