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
 * Purpose:  Compute the coefficient of a B-Spline curves.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_BSplineCoefficients.h
 *  @brief In this file we define the BSplineCoefficients class.
 **/

#ifndef STK_BSPLINECOEFFICIENTS_H
#define STK_BSPLINECOEFFICIENTS_H

#include "../../Sdk/include/STK_IRunner.h"
#include "../../Arrays/include/STK_Array2D.h"

namespace STK
{


/** @brief Compute the regression splines coefficients.
 * The BSplineCoefficients class compute the coefficients of a B-Spline curve
 * using the de Boor's algorithm. The knots can be uniform (the default) or
 * periodic. TODO: implment the density knots like in the R-script.
 *
 * The input data set is a vector of size n and the output matrix of the
 * coefficients @c Coefficients() is a matrix of size (n, nbControlPoints).
 */
class BSplineCoefficients : public IRunnerBase
{
  public:
    /** Method to use for positioning the knots. */
    enum KnotsPosition
    {
      uniform_  ///< uniform knots
    , periodic_ ///< periodic knots
    , density_  ///< knots using density of the data
    , unknown_  ///< unknown method
    };

    /** convert a String to a KnotsPosition.
     *  @param type the type of KnotsPosition in a string
     *  @return the KnotsPosition represented by the String @c type. If the string
     *  does not match any known name, the @c unknown_ type is returned.
     **/
    static KnotsPosition StringToKnotsPosition( String const& type);

    /** convert a KnotsPosition to a String.
     *  @param type the KnotsPosition we want to convert to a string
     *  @return the string associated to this KnotsPosition
     **/
    static String KnotsPositionToString( KnotsPosition const& type);

    /** @brief Default constructor : initialize the data members with default
     *  values.
     *  The number of knots is given by the formula
     *  nbKnots = nbControlPoints + degree +1.
     *  @param p_data the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves
     *  @param position method to use for positioning the knots
     **/
    BSplineCoefficients( Vector const* p_data =0
                       , int const& nbControlPoints =1
                       , int const& degree = 3
                       , KnotsPosition const& position = uniform_
                       );
    /** Constructor : initialize the data members. The number of knots is given
     *  by the formula nbKnots = nbControlPoints + degree +1.
     *  @param data the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves
     *  @param position method to use for positioning the knots
     **/
    BSplineCoefficients( Vector const& data
                       , int const& nbControlPoints
                       , int const& degree = 3
                       , KnotsPosition const& position = uniform_
                       );
    /** copy constructor.
     *  @param coefs the coefficients to copy
     **/
    BSplineCoefficients( BSplineCoefficients const& coefs);
    /** Destructor. */
    virtual ~BSplineCoefficients();
    /** clone pattern implementation */
    BSplineCoefficients* clone() const { return new BSplineCoefficients(*this);}

    /** run the computations. */
    virtual bool run();

    /** Compute the coefficients of the B-Spline curve for the given values.
     *  @param data the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves (default is 3)
     *  @param position method to use for positioning the knots (default is uniform)
     **/
    void setData( Vector const& data
                , int const& nbControlPoints
                , int const& degree = 3
                , KnotsPosition const& position = uniform_
                );

    /** give the degree of the B-Spline curves.
     * @return the degree of the B-Splines
     **/
    inline int const& degree() const { return degree_;}
    /** give the number of knots of the B-Spline curves.
     * @return the number of knots of the B-Spline curve
     **/
    inline int const& nbKnots() const { return nbKnots_;}
    /** give the number of control points of the B-Spline curves.
     * @return the number of control points of the curve
     **/
    inline int const& nbControlPoints() const { return nbControlPoints_;}
    /** give the knots of the B-Spline curves.
     * @return the vector of knots of the B-Spline curve
     **/
    inline Vector const& knots() const { return knots_;}
    /** The computed coefficients of the B-Spline curves.
     *  This is a matrix of size (p_data_->range(), 0:lastControlPoints).
     *  @return the matrix with the coefficients of the B-Spline curve.
     **/
    inline Matrix const& coefficients() const { return coefficients_;}
    /** @return the extrapolated matrix of coefficients for a given set of x-values.
     *  @param x the values to extrapolate
     **/
    Matrix extrapolate(Vector const& x) const;

  protected:
    /** the input data set */
    Vector const* p_data_;
    /** number of control points of the B-Spline curves.*/
    int nbControlPoints_;
    /** degree of the B-splines curves. */
    int degree_;
    /** Method used in order to position the knots. */
    KnotsPosition position_;
    /** number of knots of the B-Spline curves.*/
    int nbKnots_;
    /** Index of the last knots of the B-Spline curves. This is nbKnots_ - 1.*/
    int lastKnot_;
    /** Index of the last control point of the B-Spline curves.
     *  This is nbControlPoints_ - 1.
     **/
    int lastControlPoint_;
    /** Vector of the knots */
    Vector knots_;
    /** Matrix of the coefficients */
    Matrix coefficients_;

  private:
    /** Minimal value of the knots */
    Real minValue_;
    /** Maximal value of the knots */
    Real maxValue_;
    /** compute the position of the knots of the B-Spline curves.*/
    bool computeKnots();
    /** compute the position of the uniform knots.*/
    void computeUniformKnots();
    /** compute the position of the periodic knots.*/
    void computePeriodicKnots();
    /** compute the position of the density knots.*/
    void computeDensityKnots();
    /** Compute the coefficients of the B-Spline curves.*/
    void computeCoefficients();
    /** Compute a row of the coefficients matrix for a given value.
     * @param irow index of the row
     * @param value the value to which we want to compute the row
     **/
    void computeCoefficientsRow(int const& irow, Real const& value);
};

}

#endif /* STK_BSPLINECOEFFICIENTS_H */
