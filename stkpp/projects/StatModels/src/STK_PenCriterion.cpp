/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * Project:  stkpp::MStatodel
 * created on: 22 juil. 2011
 * Purpose: implement the penalized criterion.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_PenCriterion.h
 *  @brief In this file we implement the penalization criterion classes.
 **/

#include "../include/STK_PenCriterion.h"
#include "../../Sdk/include/STK_Macros.h"

namespace STK
{
/* @param p_model a pointer on the current model to set */
void ICriterion::setModel( IStatModelBase const* p_model) { p_model_ = p_model;}
/* @param model the current model to set */
void ICriterion::setModel( IStatModelBase const& model) { p_model_ = &model;}

//* Compute AIC Criterion */
bool AICCriterion::run()
{
  if (!p_model_)
  { msg_error_ = STKERROR_NO_ARG(AICCriterion::run,p_model_ is not set);
    return false;
  }
  value_ = p_model_->computeAIC();
  return true;
}

//* Compute BIC Criterion */
bool BICCriterion::run()
{
  if (!p_model_)
  { msg_error_ = STKERROR_NO_ARG(BICCriterion::run,p_model_ is not set);
    return false;
  }
  value_  = p_model_->computeBIC();
  return true;
}


} // namespace STK



