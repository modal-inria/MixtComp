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
 * Project:  stkpp::Regress
 * created on: 4 avr. 2011
 * Purpose:  cretae the matrix of coefficients for an additive regression model.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_AdditiveBSplineCoefficients.cpp
 *  @brief In this file we implement the AdditiveBSplineCoefficients class.
 **/


#include "../include/STK_AdditiveBSplineCoefficients.h"

#ifdef STK_VERBOSE
#include "../../Arrays/include/STK_Display.h"
#endif

namespace STK
{
AdditiveBSplineCoefficients::AdditiveBSplineCoefficients( Matrix const* p_data
                                                        , int const& nbControlPoints
                                                        , int const& degree
                                                        , BSplineCoefficients::KnotsPosition const& position
                                                        )
                                                        : IRunnerBase()
                                                        , p_data_(p_data)
                                                        , nbKnots_(nbControlPoints + degree +1)
                                                        , nbControlPoints_(nbControlPoints)
                                                        , degree_(degree)
                                                        , position_(position)
                                                        , coefficients_()
{ ; }

AdditiveBSplineCoefficients::AdditiveBSplineCoefficients( Matrix const& data
                                                        , int const& nbControlPoints
                                                        , int const& degree
                                                        , BSplineCoefficients::KnotsPosition const& position
                                                        )
                                                        : IRunnerBase()
                                                        , p_data_(&data)
                                                        , nbKnots_(nbControlPoints + degree +1)
                                                        , nbControlPoints_(nbControlPoints)
                                                        , degree_(degree)
                                                        , position_(position)
                                                        , coefficients_()
{ ; }

/* Destructor. */
AdditiveBSplineCoefficients::~AdditiveBSplineCoefficients()
{ ;}

/* run the computations. */
bool AdditiveBSplineCoefficients::run()
{
#ifdef STK_VERBOSE
  stk_cout << _T("in AdditiveBSplineCoefficients::run()\n");
#endif
  // check if there exists data
  if (!p_data_)
  {
    msg_error_ = _T("Error In AdditiveBSplineCoefficients::run()\nWhat: no data\n");
    return false;
  }
  try
  {
    // resize the Matrix with the marginal coefficients
    coefs_.clear(); // necesssary as coefficient_ is a reference at second call
    coefs_.resize(p_data_->cols());

    // resize the matrix of all coefficient
    coefficients_.resize(p_data_->rows(), Range());
    // get dimensions
    for (int i=p_data_->firstIdxCols(); i<=p_data_->lastIdxCols(); i++)
    {
      // create a reference on the ith column of the data
      Vector colData(p_data_->col(i), true);
      // set data to the i-th coefficient
      // WARNING: colData will be invalidate
      coefs_[i].setData(colData, nbControlPoints_, degree_, position_);
      // run computation for the i-th coefficient
      if (!coefs_[i].run())
      {
        msg_error_ =coefs_[i].error();
        return false;
      }
      // get coefficients
      coefficients_.merge(coefs_[i].coefficients());
    }
  }
  catch ( runtime_error const& e)
  {
    msg_error_ = e.what();
    return false;
  }
#ifdef STK_VERBOSE
  stk_cout << _T("AdditiveBSplineCoefficients::run() done\n");
#endif
  return true;
}

/* run the computations. */
Matrix AdditiveBSplineCoefficients::extrapolate(Matrix const& x) const
{
#ifdef STK_VERBOSE
  stk_cout << _T("in AdditiveBSplineCoefficients::extrapolate()\n");
#endif
  if (x.cols() != coefs_.range())
  { STKRUNTIME_ERROR_NO_ARG(AdditiveBSplineBSplineCoefficients::extrapolate(x),x.cols() != coefs_.range());}
    // resize the matrix of coefficient
    Matrix coefficients(x.rows(), Range());
    for (int j= coefs_.firstIdx(); j<=coefs_.lastIdx(); j++)
    { coefficients.merge(coefs_[j].extrapolate(x.col(j)));}
#ifdef STK_VERBOSE
  stk_cout << _T("AdditiveBSplineCoefficients::run() done\n");
#endif
  return coefficients;
}


/* Compute the coefficients of the B-Spline curve for the given values.
 *  @param p_data the input data values
 *  @param nbControlPoints number of control points
 *  @param degree degree of the B-Spline curves
 *  @param position method to use for positioning the knots
 **/
void AdditiveBSplineCoefficients::setData( Matrix const* p_data
                                         , int const& nbControlPoints
                                         , int const& degree
                                         , BSplineCoefficients::KnotsPosition const& position
                                         )
{
  p_data_ =p_data;
  nbKnots_ = nbControlPoints + degree +1;
  nbControlPoints_ = nbControlPoints;
  degree_ = degree;
  position_ = position;
}

} // namespace STK
