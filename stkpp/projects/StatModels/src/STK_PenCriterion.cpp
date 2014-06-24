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
 *  @brief In this file we implement the penalization criterion class.
 **/

#include "../include/STK_PenCriterion.h"

#include "STKernel/include/STK_Exceptions.h"


namespace STK
{
//* Compute AIC Criterion */
bool AICCriterion::run()
{
  try
  {
    // AIC criteria
    value_  = 2.*(-p_model_->lnLikelihood()+p_model_->nbFreeParameter());
  }
  catch( Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}

//* Compute BIC Criterion */
bool BICCriterion::run()
{
  try
  {
    // BIC criteria
    value_  = (-2.*p_model_->lnLikelihood())+(p_model_->nbFreeParameter()*p_model_->lnNbSample());
  }
  catch( Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}


} // namespace STK



