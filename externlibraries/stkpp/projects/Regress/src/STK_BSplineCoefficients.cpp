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

 Contact : Serge.Iovleff@stkpp.org
 */

/*
 * Project:  stkpp::Regress
 * created on: 25 juin 2010
 * Purpose:  implement the BSplineCoefficients class.
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_BSplineCoefficients.cpp
 *  @brief In this file we implement the BSplineCoefficients class.
 **/

#include "../include/STK_BSplineCoefficients.h"
#include "../../DManager/include/STK_HeapSort.h"
#include "../../STKernel/include/STK_String_Util.h"

#ifdef STK_VERBOSE
#include "../../Arrays/include/STK_Display.h"
#endif


namespace STK
{
/* convert a String to a TypeReduction.
 *  @param type the type of reduction we want to define
 *  @return the TypeReduction represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
BSplineCoefficients::KnotsPosition BSplineCoefficients::StringToKnotsPosition( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("uniform")))  return uniform_;
  if (toUpperString(type) == toUpperString(_T("periodic"))) return periodic_;
  if (toUpperString(type) == toUpperString(_T("density"))) return density_;
  return unknown_;
}

/* convert a TypeReduction to a String.
 *  @param type the type of reduction we want to convert
 *  @return the string associated to this type.
 **/
String BSplineCoefficients::KnotsPositionToString( KnotsPosition const& type)
{
  if (type == uniform_)  return String(_T("uniform"));
  if (type == periodic_) return String(_T("periodic"));
  if (type == density_) return String(_T("density"));
  return String(_T("unknown"));
}

/* constructor */
BSplineCoefficients::BSplineCoefficients( Vector const* p_data
                                        , int const& nbControlPoints
                                        , int const& degree
                                        , const KnotsPosition& position
                                        )
                                        : p_data_(p_data)
                                        , nbControlPoints_(nbControlPoints)
                                        , degree_(degree)
                                        , position_(position)
                                        , nbKnots_(nbControlPoints + degree +1)
                                        , lastKnot_(nbKnots_-1)
                                        , lastControlPoint_(nbControlPoints_-1)
                                        , knots_()
                                        , coefficients_()
                                        , minValue_( Arithmetic<Real>::max())
                                        , maxValue_(-Arithmetic<Real>::max())
{ }

/* constructor */
BSplineCoefficients::BSplineCoefficients( Vector const& data
                                        , int const& nbControlPoints
                                        , int const& degree
                                        , const KnotsPosition& position
                                        )
                                        : p_data_(&data)
                                        , nbControlPoints_(nbControlPoints)
                                        , degree_(degree)
                                        , position_(position)
                                        , nbKnots_(nbControlPoints + degree +1)
                                        , lastKnot_(nbKnots_-1)
                                        , lastControlPoint_(nbControlPoints_-1)
                                        , knots_()
                                        , coefficients_()
                                        , minValue_( Arithmetic<Real>::max())
                                        , maxValue_(-Arithmetic<Real>::max())
{}

// destructor
BSplineCoefficients::~BSplineCoefficients() {}

/* run the computations. */
bool BSplineCoefficients::run()
{
  // check if data exists
  if (!p_data_)
  {
   msg_error_ = _T("Error in BSplineCoefficients::run():\nWhat: p_data_ is not set.");
   return false;
  }
  // compute the coefficients
  try
  {
    // resize knots
    knots_.resize(Range(0,lastKnot_, 0));
    // resize coeficients
    coefficients_.resize(p_data_->range(), Range(0, lastControlPoint_, 0));
    // initialize array of coefficient
    coefficients_ = 0.0;
    // if there is more than one knot
    if (computeKnots()) { computeCoefficients();}
 }
 catch(Exception const& e)
 {
   msg_error_ = e.error();
   return false;
 }
 return true;
}


/*
 *  run the computations for the given value.
 *  @param p_data the input data values
 **/
void BSplineCoefficients::setData( Vector const& data
                                 , int const& nbControlPoints
                                 , int const& degree
                                 , KnotsPosition const& position
                                 )
{ // set data
  p_data_ = &data;
  nbControlPoints_ = nbControlPoints;
  degree_ = degree;
  position_ = position;
  nbKnots_ = nbControlPoints_ + degree_ +1;
  lastKnot_ = nbKnots_-1;
  lastControlPoint_ = nbControlPoints_-1;
  knots_.resize(Range(0,lastKnot_, 0));
  minValue_ =  Arithmetic<Real>::max();
  maxValue_ = -Arithmetic<Real>::max();
}

/* Extrapolate the matrix of coefficients for a given set of x-values.
 *  @param x the values to extrapolate
 *  @param coefs the matrix of coefficients for each values.
 **/
Matrix BSplineCoefficients::extrapolate(Vector const& x) const
{
  // check if knots exists
  if (knots_.empty())
  { STKRUNTIME_ERROR_NO_ARG(BSplineCoefficients::run(x),There is no knots);}
  // resize coeficients
  Matrix coefs(x.range(), Range(0, lastControlPoint_, 0), 0.0);
  // check if the original data set was not reduced to a single point
  if (minValue_ == maxValue_) return coefs;
  // compute the coefficients
  for (int irow=x.firstIdx(); irow<= x.lastIdx(); irow++)
  {
    const Real value = x[irow];
    // value outside the range of the knots case
    if (value <= minValue_)
    {
      coefs(irow, 0) = 1.0;
      continue;
    }
    if (value >= maxValue_)
    {
      coefs(irow, lastControlPoint_) = 1.0;
      continue;
    }
    // find interval
    int k, k1;
    for (k=0, k1=1; k<lastControlPoint_; k++, k1++)
    {
      if (value < knots_[k1]) break;
    }
    // begin recursion
    coefs(irow, k) = 1.0;
    for (int d=1; d<=degree_; d++)
    {
      // right (south-west corner) term only
      coefs(irow, k-d) = ( (knots_[k1] - value)/(knots_[k1] - knots_[k1-d]) ) * coefs(irow, k1-d);
      // compute internal terms
      for (int i = k1-d; i<k; i++)
      {
        const Real knots_i = knots_[i], knots_id1 = knots_[i+d+1];
        coefs(irow, i) = ( (value - knots_i)/(knots_[i+d] - knots_i) ) * coefs(irow, i)
                       + ( (knots_id1 - value)/(knots_id1 - knots_[i+1]) ) * coefs(irow, i+1);
      }
      // left (north-west corner) term only
      coefs(irow, k) *= (value - knots_[k])/(knots_[k+d] - knots_[k]);
    }
  }
  return coefs;
}

/* compute the knots of the B-Spline curves.*/
bool BSplineCoefficients::computeKnots()
{
  // get dimensions
  int first = p_data_->firstIdx(), last = p_data_->lastIdx();
  // compute min value
  for (int i=first; i<= last; i++)
  {
    minValue_ = std::min(minValue_, (*p_data_)[i]);
    maxValue_ = std::max(maxValue_, (*p_data_)[i]);
  }
  // if all value are equals, all the knots are equals to this value
  if (minValue_ == maxValue_)
  {
    knots_ = minValue_;
    return false;
  }
  // set knots values
  switch (position_)
  {
    // uniform position
    case uniform_:
      computeUniformKnots();
      break;
    // periodic position
    case periodic_:
      computePeriodicKnots();
      break;
    // density position
    case density_:
      computeDensityKnots();
      break;
      // periodic position
    case unknown_:
      // check if there exists data
      throw runtime_error("Error In BSplineCoefficients::computeKnots():"
                               " unknowns positions");
      break;
  }
  // shift knots
  Real range = (maxValue_ - minValue_);
  for (int k = 0; k <= lastKnot_; k++)
    knots_[k] = minValue_ + range * knots_[k];
  return true;
}

/* compute the position of the uniform knots.*/
void BSplineCoefficients::computeUniformKnots()
{
  // compute step
  Real step = 1.0/(nbControlPoints_ - degree_);
  // set internal knots
  const int first = degree_ + 1, last = lastControlPoint_;
  for (int k = first, j = 1; k <= last; j++, k++)
    knots_[k] = j * step;
  // set external knots
  for ( int k=0, j = last+1; k < first; j++, k++)
  {
    knots_[k] = 0;
    knots_[j] = 1;
  }
}
/* compute the position of the periodic knots.*/
void BSplineCoefficients::computePeriodicKnots()
{
  // compute step
  Real step = 1.0/(nbControlPoints_ - degree_);
  // set knots
  for (int k = 0, j = -degree_; k <= lastKnot_; j++, k++)
    knots_[k] = j * step;
;
}
/* compute the position of the density knots. */
void BSplineCoefficients::computeDensityKnots()
{
  // sorted data
  Vector xtri;
  // sort the data
  heapSort< Vector>(*p_data_, xtri);

  // compute step
  Real step = xtri.size()/(Real)lastKnot_;
  int first = xtri.firstIdx(), last = xtri.lastIdx();
  // set knots
  for (int k = 0; k < lastKnot_; k++)
  {
    int cell = first + int(k* step);
    knots_[k] = (xtri[cell] + xtri[cell+1])/2.;
  }
  // set last knots
  knots_[lastKnot_] = (xtri[last-1] + xtri[last])/2.;
}

/* Compute the coefficients of the B-Spline curves.*/
void BSplineCoefficients::computeCoefficients()
{
#ifdef STK_VERBOSE
  stk_cout << _T("BSplineCoefficients::computeCoefficients()\n");
#endif
  // get dimensions
  int first = p_data_->firstIdx(), last = p_data_->lastIdx();
  // compute the coefficients
  for (int i=first; i<= last; i++)
  {
    computeCoefficientsRow(i, (*p_data_)[i]);
  }
#ifdef STK_VERBOSE
  stk_cout << _T("BSplineCoefficients::computeCoefficients() done\n");
#endif
}

/* Compute a row of the coefficients
 * @param irow index of the row
 **/
void BSplineCoefficients::computeCoefficientsRow(int const& irow, Real const& value)
{
  // value outside the range of the knots case
  if (value <= minValue_)
  {
    coefficients_(irow, 0) = 1.0;
    return;
  }
  if (value >= maxValue_)
  {
    coefficients_(irow, lastControlPoint_) = 1.0;
    return;
  }
  // find interval
  int k, k1;
  for (k=0, k1=1; k<lastControlPoint_; k++, k1++)
  {
    if (value < knots_[k1]) break;
  }
  // begin recursion
  coefficients_(irow, k) = 1.0;
  for (int d=1; d<=degree_; d++)
  {
    // right (south-west corner) term only
    coefficients_(irow, k-d) = ( (knots_[k1] - value)/(knots_[k1] - knots_[k1-d]) )
                               * coefficients_(irow, k1-d);
    // compute internal terms
    for (int i = k1-d; i<k; i++)
    {
      const Real knots_i = knots_[i], knots_id1 = knots_[i+d+1];
      coefficients_(irow, i) = ( (value - knots_i)/(knots_[i+d] - knots_i) )
                               * coefficients_(irow, i)
                             + ( (knots_id1 - value)/(knots_id1 - knots_[i+1]) )
                               * coefficients_(irow, i+1);
    }
    // left (north-west corner) term only
    coefficients_(irow, k) *= (value - knots_[k])/(knots_[k+d] - knots_[k]);
  }
}

} // namespace STK
