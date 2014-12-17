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
 * Project:  stkpp::regress
 * created on: 31 juil. 2010
 * Purpose:  implementation of the BSplineRegression class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_BSplineRegression.cpp
 *  @brief In this file we implement the BSplineRegression class.
 **/


#include "Algebra/include/STK_GinvSymmetric.h"

#include "../include/STK_BSplineRegression.h"

namespace STK
{

BSplineRegression::BSplineRegression( ArrayXX const* p_y
                                    , VectorX const* p_x
                                    , int const& nbControlPoints
                                    , int const& degree
                                    , const _Kposition& position
                                    )
                                    : IRegression<ArrayXX, Vector, Vector>(p_y, p_x)
                                    , nbControlPoints_(nbControlPoints)
                                    , degree_(degree)
                                    , position_(position)
                                    , coefs_(*p_x, nbControlPoints_, degree_, position_)
                                    , controlPoints_()
{ }

BSplineRegression::~BSplineRegression()
{}

/* compute the regression function. */
void BSplineRegression::regressionStep()
{
  // compute X'X
  ArraySquareX prod;
  prod.move(multLeftTranspose(coefs_.coefficients()));

  // compute (X'X)^{-1}
  GinvSymmetric inv;
  inv(prod);

  // compute (X'X)^{-1}X'Y
  controlPoints_ = prod * (coefs_.coefficients().transpose() * p_y_->asDerived());
}

/* compute the regression function. */
void BSplineRegression::regression(Vector const& weights)
{
  // compute X'X
  ArraySquareX prod;
  prod.move(weightedMultLeftTranspose(coefs_.coefficients(), weights));

  // compute (X'X)^{-1}
  GinvSymmetric inv;
  inv(prod);

  // compute X'Y
  ArrayXX temp;
  temp.move(wmultLeftTranspose(coefs_.coefficients(), p_y_->asDerived(), weights));
  // compute (X'X)^{-1}X'Y
  controlPoints_.move(mult(prod, temp));
}

/* Compute the predicted outputs by the regression function. */
void BSplineRegression::predictionStep()
{
  // compute predictions
  predicted_.move(mult(coefs_.coefficients(), controlPoints_));
}

/* @brief Extrapolate the values @c y from the value @c x.
 *  Given the data set @c x will compute the values \f$ y = x.\hat{\beta} \f$.
 *  The coefficients @c coefs_ have to be estimated previously.
 *  @param x the input data set
 *  @param y the output (extrapolated) data set
 */
ArrayXX BSplineRegression::extrapolate( Vector const& x) const
{ return mult(coefs_.extrapolate(x), controlPoints_);}

} // namespace STK
