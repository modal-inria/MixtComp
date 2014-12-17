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
 * Purpose: definition of the BsplineRegression class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_BSplineRegression.h
 *  @brief In this file we define the BsplineRegression class.
 **/

#ifndef STK_BSPLINEREGRESSION_H
#define STK_BSPLINEREGRESSION_H

#include "Arrays/include/STK_Array2D.h"

#include "STK_BSplineCoefficients.h"
#include "STK_IRegression.h"

namespace STK
{

/** @brief Compute a BSpline, multivalued, regression function using BSpline
 *  basis.
 */
class BSplineRegression : public IRegression<ArrayXX, Vector, Vector>
{
  private:
    typedef Regress::KnotsPosition _Kposition;

  public:
    /** Constructor.
     * @param p_y d-dimensional array of output to fit
     * @param p_x uni-dimensional array of predictor
     * @param nbControlPoints number of control points of the spline
     * @param degree degree of the BSpline basis
     * @param position position of the knots to used
     **/
    BSplineRegression( ArrayXX const* p_y
                     , VectorX const* p_x
                     , int const& nbControlPoints
                     , int const& degree = 3
                     , const _Kposition& position = Regress::uniform_
                     );

    /** virtual destructor. */
    virtual ~BSplineRegression();

    /** give the degree of the B-Spline curve.
     *  @return the degree of the B-Spline curve
     * */
    inline int degree() const { return degree_;}
    /** give the number of control points of the B-Spline curves.
     *  @return the number of control points of the B-Spline curve
     **/
    inline int nbControlPoints() const { return nbControlPoints_;}
    /** give the control points.
     *  @return the control points of the B-Spline curve
     **/
    inline ArrayXX const& controlPoints() const { return controlPoints_; }
    /** give the knots.
     *  @return the knots of the B-Spline curve
     **/
    inline Vector const& knots() const { return coefs_.knots(); }
    /** give the computed coefficients of the B-Spline curves.
     *  This is a matrix of size (p_x_->range(), 0:lastControlPoints).
     *  @return the coefficients of the B-Spline curve
     **/
    inline ArrayXX const& coefficients() const { return coefs_.coefficients();}

    /** @return the Extrapolates values of y from the value @c x.
     *  Given the data set @c x will compute the values \f$ y = \psi(x) \hat{\beta} \f$
     *  where \f$ \psi \f$ represents the B-spline basis functions and \f$ \hat{beta} \f$
     *  the estimated coefficients.
     *  @param x the input data set
     */
    virtual ArrayXX extrapolate( Vector const& x) const;

  protected:
    /** number of control points of the B-Spline curve. */
    int nbControlPoints_;
    /** degree of the B_Spline curve */
    int degree_;
    /** method of position of the knots of the B-Spline curve */
    _Kposition position_;
    /** Coefficients of the regression matrix */
    BSplineCoefficients<Vector> coefs_;
    /** Estimated control points of the B-Spline curve */
    ArrayXX controlPoints_;

    /** compute the coefficients of the BSpline basis. This method will be
     *  called in the base class @c IRegression::run()
     **/
    inline virtual void initializeStep() {coefs_.run();}

    /** compute the regression function. This method will be
     *  called in the base class @c IRegression::run() after initializeStep()
     **/
    virtual void regressionStep();
    /** compute the regression function. This method will be
     *  called in the base class @c IRegression::run(weights) after initializeStep()
     *  @param weights the weights of the samples
     **/
    virtual void regression(VectorX const& weights);
    /** Compute the predicted outputs by the regression function. This method
     *  will be called in the base class @c IRegression::run() after initializeStep()
     **/
    virtual void predictionStep();
    /** Compute the number of parameter of the regression function.
     * @return the number of parameter of the regression function
     **/
    inline virtual int computeNbFreeParameter() const
    { return controlPoints_.sizeCols() * controlPoints_.sizeRows(); }

};

} // namespace STK

#endif /* STK_BSPLINEREGRESSION_H */
