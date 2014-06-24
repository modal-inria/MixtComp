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
 * created on: 31 juil. 2010
 * Purpose: definition of the AdditiveBSplineRegression class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_AdditiveBSplineRegression.cpp
 *  @brief In this file we implement the AdditiveBSplineRegression class.
 **/

#include "../include/STK_AdditiveBSplineRegression.h"

#include "Arrays/include/STK_Array2D_Functors.h"
#include "Algebra/include/STK_LinAlgebra2D.h"
//#include "Algebra/include/STK_LinAlgebra3D.h"
#include "Algebra/include/STK_GinvSymmetric.h"

namespace STK
{

/* Constructor.
 * @param p_y p-dimensional array of output to fit
 * @param p_x d-dimensional array of predictor
 * @param nbControlPoints number of control points of the spline
 * @param degree degree of the BSpline basis
 * @param position position of the knots to used
 **/
AdditiveBSplineRegression::AdditiveBSplineRegression( Matrix const* p_y
                                                    , Matrix const* p_x
                                                    , int const& nbControlPoints
                                                    , int const& degree
                                                    , KnotsPosition const& position
                                                    )
                                                    : IRegression<Matrix, Matrix, Vector>(p_y, p_x)
                                                    , nbControlPoints_(nbControlPoints)
                                                    , degree_(degree)
                                                    , position_(position)
                                                    , coefs_(p_x, nbControlPoints_, degree_, position_)
                                                    , controlPoints_()
{ }

/* virtual destructor. */
AdditiveBSplineRegression::~AdditiveBSplineRegression()
{}


/* compute the coefficients of the BSpline basis. This method willl be
 * called in the base class @c IRegression::run()
 **/
void AdditiveBSplineRegression::preRun()
{
  coefs_.setData(p_x_, nbControlPoints_, degree_, position_);
  if (!coefs_.run())
  { throw runtime_error(coefs_.error());}
}

/* compute the regression function. */
void AdditiveBSplineRegression::regression()
{
  // compute X'X
  MatrixSquare prod;
  prod.move(multLeftTranspose(coefs_.coefficients()));

  // compute (X'X)^{-1}
  GinvSymmetric inv;
  inv(prod);
  // compute (X'X)^{-1}X'Y
  controlPoints_ = prod * (coefs_.coefficients().transpose() *p_y_->asDerived()) ;
}



/* compute the regression function. */
void AdditiveBSplineRegression::regression(Vector const& weights)
{
  // compute X'X
  MatrixSquare prod;
  prod.move(weightedMultLeftTranspose(coefs_.coefficients(), weights));

  // compute (X'X)^{-1}
  GinvSymmetric inv;
  inv(prod);

  // compute X'Y
  Matrix temp;
  temp.move(wmultLeftTranspose(coefs_.coefficients(), p_y_->asDerived(), weights));

  // compute (X'X)^{-1}X'Y
  controlPoints_ = prod * temp;
}

/* Compute the predicted outputs by the regression function. */
void AdditiveBSplineRegression::prediction()
{
  // remove existing predictions if any (should not be the case)
  if (!p_predicted_) p_predicted_ = new Matrix;
  // compute predictions
  *p_predicted_ = coefs_.coefficients() * controlPoints_;
}


/* @brief Extrapolate the values @c y from the value @c x.
 *  Given the data set @c x will compute the values \f$ y = \psi(x) \hat{\beta} \f$
 *  where \f$ \psi \f$ represents the B-spline basis functions and \f$ \hat{beta} \f$
 *  the estimated coefficients.
 */
Matrix AdditiveBSplineRegression::extrapolate( Matrix const& x) const
{ return mult(coefs_.extrapolate(x), controlPoints_);}

} // namespace STK

