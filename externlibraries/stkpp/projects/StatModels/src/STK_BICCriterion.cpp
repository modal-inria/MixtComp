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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: implement the BIC criterion.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_BICCriterion.h
 *  @brief In this file we implement the BICCriterion class.
 **/
#include "../../STKernel/include/STK_Exceptions.h"

#include "../include/STK_BICCriterion.h"

namespace STK
{
//Constructor
BICCriterion::BICCriterion( IModelBase const& model)
                          : ICriterion(model)
{}



//Destructor
BICCriterion::~BICCriterion(){}

//* Compute BIC Criterion */
bool BICCriterion::run()
{
  try
  {
    Real loglikelihood    = p_model_->lnLikelihood();
    int freeParameter = p_model_->nbFreeParameter();
    Real logN             = p_model_->lnNbSample();
    // BIC criteria
    value_  = (-2.*loglikelihood)+(freeParameter*logN);
  }
  catch(const Exception& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}


} // namespace STK



