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
 * created on: 25 juin 2010
 * Purpose:  Compute the coefficients of an additive B-Spline manifold.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_AdditiveBSplineCoefficients.h
 *  @brief In this file we define the AdditiveBSplineCoefficients class.
 **/

#ifndef STK_ADDITIVEBSPLINECOEFFICIENTS_H
#define STK_ADDITIVEBSPLINECOEFFICIENTS_H

#include "../../Sdk/include/STK_IRunner.h"
#include "STK_BSplineCoefficients.h"

namespace STK
{

/** @ingroup Regress
 *  @brief Compute the regression splines coefficients of an additive model.
 *
 * The method is described in @ref BSplineCoefficients documentation class and
 * repeated for each variables of the model. The number of control points, the
 * degree and the position of the knots are the same for all variables.
 *
 * If the input data set is a matrix of size (n,p) then the output matrix of
 * the coefficients @c Coefficients() is a matrix of size
 * (n, p*nbControlPoints) where p is the number of variables.
 */
class AdditiveBSplineCoefficients : public IRunnerBase
{
  public:
    /** Constructor : initialize the data members. The number of knots is given
     *  by the formula nbKnots = nbControlPoints + degree +1.
     *  @param p_data a pointer on the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves
     *  @param position method to use for positioning the knots
     **/
    AdditiveBSplineCoefficients( Matrix const* p_data = 0
                               , int const& nbControlPoints = 1
                               , int const& degree = 3
                               , BSplineCoefficients::KnotsPosition const& position = BSplineCoefficients::uniform_
                               );
    /** Constructor : initialize the data members. The number of knots is given
     *  by the formula nbKnots = nbControlPoints + degree +1.
     *  @param data the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves
     *  @param position method to use for positioning the knots
     **/
    AdditiveBSplineCoefficients( Matrix const& data
                               , int const& nbControlPoints
                               , int const& degree = 3
                               , BSplineCoefficients::KnotsPosition const& position = BSplineCoefficients::uniform_
                               );
    /** Copy constructor.
     *  @param coefs the coefficients to copy
     **/
    AdditiveBSplineCoefficients( AdditiveBSplineCoefficients const& coefs);
    /** Destructor. */
    virtual ~AdditiveBSplineCoefficients();
    /** clone pattern implementation */
    AdditiveBSplineCoefficients* clone() const { return new AdditiveBSplineCoefficients(*this);}
    /** Compute the coefficients of the B-Spline curve for the given values.
     *  @param p_data the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves
     *  @param position method to use for positioning the knots
     **/
    void setData( Matrix const* p_data
                , int const& nbControlPoints
                , int const& degree = 3
                , BSplineCoefficients::KnotsPosition const& position = BSplineCoefficients::uniform_
                );

    /** run the computations. */
    bool run();

    /** give the degree of the B-Spline curves. */
    inline int const& degree() const { return degree_;}
    /** give the number of knots of the B-Spline curves. */
    inline int const& nbKnots() const { return nbKnots_;}
    /** give the number of control points of the B-Spline curves. */
    inline int const& nbControlPoints() const { return nbControlPoints_;}
    /** give the computed coefficients of the B-Spline curves.
     * This is a matrix of size (p_data_->range(), 0:lastControlPoints).
     **/
    inline Matrix const& coefficients() const { return coefficients_;}
    /** @return the extrapolated coefficients for the x matrix.
     *  @param x the values to extrapolate
     **/
    Matrix extrapolate(Matrix const& x) const;

  protected:
    /** the input data set */
    Matrix const* p_data_;
    /** number of knots of the B-Spline curves.*/
    int nbKnots_;
    /** number of control points of the B-Spline curves.*/
    int nbControlPoints_;
    /** degree of the B-splines curves. */
    int degree_;
    /** Method used in order to position the knots. */
    BSplineCoefficients::KnotsPosition position_;
    /** Array with the knots and coefficients in each dimension. */
    Array1D<BSplineCoefficients> coefs_;
    /** Matrix of the coefficients */
    Matrix coefficients_;
};

} // namespace STK

#endif /* STK_BSPLINECOEFFICIENTS_H */
