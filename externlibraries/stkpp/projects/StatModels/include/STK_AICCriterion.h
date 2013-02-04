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
 * Purpose: define the AIC criterion.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_AICCriterion.h
 *  @brief In this file we define the AICCriterion class.
 **/

#ifndef STK_AICCRITERION_H
#define STK_AICCRITERION_H

#include "STK_ICriterion.h"

namespace STK
{
/** @ingroup Model
 *  @brief Derived class of Criterion for computing the AIC Criterion
 *  The AIC criteria is a penalization of the likelihood given by the formula
 *  \f[
 *  -2 \cdot \ln{p(x|k)} \approx \mathrm{AIC} = {-2 \cdot \ln{L} +2 D }
 *  \f]
 *  where \f$ L \f$ represents the likelihood of the observations and \f$ D \f$
 *  the number of free parameter of the model.
 **/
class AICCriterion : public ICriterion
{
  public:
    /** Constructor.
     *  @param model the model to evaluate the criterion
     **/
    AICCriterion( IModelBase const& model);

    /** virtual destructor. */
    virtual ~AICCriterion();
  
    /** implementation of the virtual method run */
    virtual bool run();
};

} // namespace STK

#endif
