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

/** @file STK_AdditiveBSplineRegression.h
 *  @brief In this file we define the AdditiveBSplineRegression class.
 **/

#ifndef STK_ADDITIVEBSPLINEREGRESSION_H
#define STK_ADDITIVEBSPLINEREGRESSION_H

#include "STK_IRegression.h"
#include "STK_AdditiveBSplineCoefficients.h"

namespace STK
{

/** @ingroup Regress
 *  @brief Compute an additive BSpline, multivalued, regression function using
 *  BSpline basis.
 */
class AdditiveBSplineRegression : public IRegression<ArrayXX, ArrayXX, Vector>
{
  public:
    //
    typedef Regress::KnotsPosition KnotsPosition;

    /** Constructor.
     * @param p_y p-dimensional array of output to fit
     * @param p_x d-dimensional array of predictor
     * @param nbControlPoints number of control points of the spline
     * @param degree degree of the BSpline basis
     * @param position position of the knots to used
     **/
    AdditiveBSplineRegression( ArrayXX const* p_y
                             , ArrayXX const* p_x
                             , int const& nbControlPoints
                             , int const& degree = 3
                             , KnotsPosition const& position = Regress::uniform_
                             );

    /** virtual destructor. */
    virtual ~AdditiveBSplineRegression();

    /** give the degree of the B-Spline curves.
     *  @return the degree of the B-Spline curves
     * */
    inline int degree() const { return degree_;}
    /** give the number of control points of the B-Spline curves.
     *  @return the number of control points of the B-Spline curves
     **/
    inline int nbControlPoints() const { return nbControlPoints_;}
    /** give the control points.
     *  @return the control points of the B-Spline curves
     **/
    inline ArrayXX const& controlPoints() const { return controlPoints_; }
    /** give the computed coefficients of the B-Spline curves.
     *   This is a matrix of size (p_x_->range(), 0:lastControlPoints).
     *  @return the coefficients of the B-Spline curve
     **/
    inline ArrayXX const& coefficients() const { return coefs_.coefficients();}

    /** @return The extrapolated values of y from the value @c x.
     *  Given the data set @c x will compute the values \f$ y = \psi(x) \hat{\beta} \f$
     *  where \f$ \psi \f$ represents the B-spline basis functions and \f$ \hat{beta} \f$
     *  the estimated coefficients.
     *  @param x the input data set
     **/
    virtual ArrayXX extrapolate( ArrayXX const& x) const;

  protected:
    /** number of control points of the B-Spline curve. */
    int nbControlPoints_;
    /** degree of the B_Spline curve */
    int degree_;
    /** method of position of the knots of the B-Spline curve */
    KnotsPosition position_;
    /** Coefficients of the regression matrix */
    AdditiveBSplineCoefficients<ArrayXX> coefs_;
    /** Estimated control points of the B-Spline curve */
    ArrayXX controlPoints_;

    /** compute the coefficients of the BSpline basis. This method will be
     * called in the base class @c IRegression::run()
     **/
    virtual void initializeStep();

    /** compute the regression function. */
    virtual void regressionStep();
    /** compute the regression function.
     * @param weights weights of the samples
     **/
    virtual void regression(VectorX const& weights);
    /** Compute the predicted outputs by the regression function. */
    virtual void predictionStep();
    /** Compute the number of parameter of the regression function.
     * @return the number of parameter of the regression function
     **/
    inline virtual int computeNbFreeParameter() const
    { return controlPoints_.sizeCols() * controlPoints_.sizeRows(); }
};

} // namespace STK

#endif /* STK_ADDITIVEBSPLINEREGRESSION_H */
