/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::Regress
 * created on: 27 oct. 2010
 * Purpose: Definition of the class MultidimRegression .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_MultidimRegression.cpp
 *  @brief In this file we implement the class MultidimRegression.
 **/

#include "Algebra/include/STK_GinvSymmetric.h"

#include "../include/STK_MultidimRegression.h"


namespace STK
{

MultidimRegression::MultidimRegression( ArrayXX const* y, ArrayXX const* x)
                                      : IRegression<ArrayXX, ArrayXX, Vector>(y, x)
                                      , coefs_()
{ }

MultidimRegression::~MultidimRegression()
{ }

/* compute the regression function. */
void MultidimRegression::regressionStep()
{
  // compute X'X
  ArraySquareX prod;
  prod.move(multLeftTranspose(p_x_->asDerived()));

  // compute (X'X)^{-1}
  GinvSymmetric()(prod);

  // compute X'Y
  ArrayXX temp;
  temp.move(multLeftTranspose(p_x_->asDerived(), p_y_->asDerived()));

  // compute (X'X)^{-1}X'Y
  coefs_.move(mult(prod, temp));
}

/* compute the regression function. */
void MultidimRegression::regression(VectorX const& weights)
{
  // compute X'WX
  ArraySquareX prod;
  prod.move(weightedMultLeftTranspose(p_x_->asDerived(), weights));

  // compute (X'WX)^{-1}
  GinvSymmetric inv;
  inv(prod);

  // compute X'WY
  ArrayXX temp;
  temp.move(wmultLeftTranspose(p_x_->asDerived(), p_y_->asDerived(), weights));

  // compute (X'WX)^{-1}X'WY
  coefs_.move(mult(prod, temp));
}

/* Compute the predicted outputs by the regression function. */
void MultidimRegression::predictionStep()
{ predicted_.move(mult(*p_x_, coefs_));}

/* @brief Extrapolate the the values @c y from the value @c x.
 *  Given the data set @c x will compute the values \f$ y = \hat{f}(x) \f$.
 *  The regression function @e f have to be estimated previously.
 */
ArrayXX MultidimRegression::extrapolate( ArrayXX const& x) const
{ return(mult(x, coefs_));}

} // namespace STK
