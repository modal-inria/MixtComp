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
 * Project:  stkpp::aam
 * created on: 27 sept. 2010
 * Purpose:  Define the AdditiveBSplineRegressionPage.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_AdditiveBSplineRegressionPage.h
 *  @brief In this file we define the AdditiveBSplineRegressionPage class.
 **/

#ifndef STK_ADDITIVEBSPLINEREGRESSIONPAGE_H
#define STK_ADDITIVEBSPLINEREGRESSIONPAGE_H

#include "DManager/include/STK_IPage.h"
#include "STK_BSplineCoefficients.h"

namespace STK
{
/** @brief In this Page, we manage the options for the computation of the
 * Additive BSpline regression.
 * The AdditiveBSplineRegression page have the form
 * @code
 *   # AdditiveBSplineRegression options
 *  [AdditiveBSplineRegression]
 *    # number of control point
 *    nbControlPoints = 7
 *    # degree of the BSpline
 *    degree = 3
 *    # disposition of the knots uniform/periodic
 *    knotsPositions = uniform
 * @endcode
 * */
class AdditiveBSplineRegressionPage: public IPage
{
  public:
    /** constructor.
     *  @param level the level of the option page
     **/
    AdditiveBSplineRegressionPage(int const& level);
    /** copy constructor.
     * @param page the page to copy
     **/
    AdditiveBSplineRegressionPage( AdditiveBSplineRegressionPage const& page);
    /** destructor. */
    virtual ~AdditiveBSplineRegressionPage();

    /** number of control points
     * @return the number of control point
     **/
    inline int nbControlPoints() const { return nbControlPoints_;}
    /** degree of the BSpline
     * @return the degree of the spline
     **/
    inline int degree() const { return degree_;}
    /** get the method for positioning the knots.
     *  @return the method for positioning the knots
     */
    inline Regress::KnotsPosition const& knotsPositions() const
    { return position_;}

    /** validate the options. Check if the values are coherent. */
    virtual bool validate();

    /** Create a copy of the AdditiveBSplineRegressionPage.
     *  @return a pointer on the clone of this
     **/
    inline virtual AdditiveBSplineRegressionPage* clone() const
    { return new AdditiveBSplineRegressionPage(*this);}

  protected:
    /** Number of control points */
    int nbControlPoints_;
    /** degree of the splines */
    int degree_;
    /** positions of the knots. */
    Regress::KnotsPosition position_;
};

} // namespace STK


#endif /* STK_ADDITIVEBSPLINEREGRESSIONPAGE_H */
