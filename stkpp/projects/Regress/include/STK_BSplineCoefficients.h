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

#include "Sdk/include/STK_IRunner.h"
#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DVector.h"
#include "STK_Regress_Util.h"

#include "../include/STK_BSplineCoefficients.h"
#include "DManager/include/STK_HeapSort.h"
#include "STKernel/include/STK_String.h"

#ifdef STK_REGRESS_VERBOSE
#include "Arrays/include/STK_Display.h"
#endif

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
template<class Vector>
class BSplineCoefficients : public IRunnerBase
{
  public:
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
                       , int nbControlPoints =1
                       , int degree = 3
                       , Regress::KnotsPosition position = Regress::uniform_
                       );
    /** Constructor : initialize the data members. The number of knots is given
     *  by the formula nbKnots = nbControlPoints + degree +1.
     *  @param data the input data values
     *  @param nbControlPoints number of control points
     *  @param degree degree of the B-Spline curves
     *  @param position method to use for positioning the knots
     **/
    BSplineCoefficients( Vector const& data
                       , int nbControlPoints
                       , int degree = 3
                       , Regress::KnotsPosition position = Regress::uniform_
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
                , int nbControlPoints
                , int degree = 3
                , Regress::KnotsPosition position = Regress::uniform_
                );

    /** give the degree of the B-Spline curves.
     * @return the degree of the B-Splines
     **/
    inline int degree() const { return degree_;}
    /** give the number of knots of the B-Spline curves.
     * @return the number of knots of the B-Spline curve
     **/
    inline int nbKnots() const { return nbKnots_;}
    /** give the number of control points of the B-Spline curves.
     * @return the number of control points of the curve
     **/
    inline int nbControlPoints() const { return nbControlPoints_;}
    /** give the knots of the B-Spline curves.
     * @return the vector of knots of the B-Spline curve
     **/
    inline Array2DVector<Real> const& knots() const { return knots_;}
    /** The computed coefficients of the B-Spline curves.
     *  This is a matrix of size (p_data_->range(), 0:lastControlPoints).
     *  @return the matrix with the coefficients of the B-Spline curve.
     **/
    inline Array2D<Real> const& coefficients() const { return coefficients_;}
    /** @return the extrapolated matrix of coefficients for a given set of x-values.
     *  @param x the values to extrapolate
     **/
    template<class OtherVector>
    Array2D<Real> extrapolate(OtherVector const& x) const;

  protected:
    /** the input data set */
    Vector const* p_data_;
    /** number of control points of the B-Spline curves.*/
    int nbControlPoints_;
    /** degree of the B-splines curves. */
    int degree_;
    /** Method used in order to position the knots. */
    Regress::KnotsPosition position_;
    /** number of knots of the B-Spline curves.*/
    int nbKnots_;
    /** Index of the last knots of the B-Spline curves. This is nbKnots_ - 1.*/
    int lastKnot_;
    /** Index of the last control point of the B-Spline curves.
     *  This is nbControlPoints_ - 1.
     **/
    int lastControlPoint_;
    /** Vector of the knots */
    Array2DVector<Real> knots_;
    /** Array2D<Real> of the coefficients */
    Array2D<Real> coefficients_;

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
    void computeCoefficientsRow(int irow, Real const& value);
};

/* constructor */
template<class Vector>
BSplineCoefficients<Vector>::BSplineCoefficients( Vector const* p_data
                                        , int nbControlPoints
                                        , int degree
                                        , Regress::KnotsPosition position
                                        )
                                        : IRunnerBase()
                                        , p_data_(p_data)
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
template<class Vector>
BSplineCoefficients<Vector>::BSplineCoefficients( Vector const& data
                                        , int nbControlPoints
                                        , int degree
                                        , Regress::KnotsPosition position
                                        )
                                        : IRunnerBase()
                                        , p_data_(&data)
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
/* copy constructor.
 *  @param coefs the coefficients to copy
 **/
template<class Vector>
BSplineCoefficients<Vector>::BSplineCoefficients( BSplineCoefficients const& coefs)
                                        : IRunnerBase()
                                        , p_data_(coefs.p_data_)
                                        , nbControlPoints_(coefs.nbControlPoints_)
                                        , degree_(coefs.degree_)
                                        , position_(coefs.position_)
                                        , nbKnots_(coefs.nbKnots_)
                                        , lastKnot_(coefs.lastKnot_)
                                        , lastControlPoint_(coefs.lastControlPoint_)
                                        , knots_(coefs.knots_)
                                        , coefficients_(coefs.coefficients_)
                                        , minValue_(coefs.minValue_)
                                        , maxValue_(coefs.maxValue_)
{}

// destructor
template<class Vector>
BSplineCoefficients<Vector>::~BSplineCoefficients() {}

/* run the computations. */
template<class Vector>
bool BSplineCoefficients<Vector>::run()
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
template<class Vector>
void BSplineCoefficients<Vector>::setData( Vector const& data
                                 , int nbControlPoints
                                 , int degree
                                 , Regress::KnotsPosition position
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
template<class Vector>
template<class OtherVector>
Array2D<Real> BSplineCoefficients<Vector>::extrapolate(OtherVector const& x) const
{
  // check if knots exists
  if (knots_.empty())
  { STKRUNTIME_ERROR_NO_ARG(BSplineCoefficients::run(x),There is no knots);}
  // resize coeficients
  Array2D<Real> coefs(x.range(), Range(0, lastControlPoint_, 0), 0.0);
  // check if the original data set was not reduced to a single point
  if (minValue_ == maxValue_) return coefs;
  // compute the coefficients
  for (int irow=x.begin(); irow< x.end(); irow++)
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
template<class Vector>
bool BSplineCoefficients<Vector>::computeKnots()
{
  // get dimensions
  int first = p_data_->begin(), last = p_data_->lastIdx();
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
    msg_error_ = STKERROR_NO_ARG(BSplineCoefficients::computeKnots(),All values are equal);
    return false;
  }
  // set knots values
  switch (position_)
  {
    // uniform position
    case Regress::uniform_:
      computeUniformKnots();
      break;
    // periodic position
    case Regress::periodic_:
      computePeriodicKnots();
      break;
    // density position
    case Regress::density_:
      computeDensityKnots();
      break;
      // periodic position
    case Regress::unknownKnotsPosition_:
      msg_error_ = STKERROR_NO_ARG(BSplineCoefficients::computeKnots(),unknownKnotsPosition reached);
      return false;
      break;
  }
  // shift knots
  Real range = (maxValue_ - minValue_);
  for (int k = 0; k <= lastKnot_; k++)
    knots_[k] = minValue_ + range * knots_[k];
  return true;
}

/* compute the position of the uniform knots.*/
template<class Vector>
void BSplineCoefficients<Vector>::computeUniformKnots()
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
template<class Vector>
void BSplineCoefficients<Vector>::computePeriodicKnots()
{
  // compute step
  Real step = 1.0/(nbControlPoints_ - degree_);
  // set knots
  for (int k = 0, j = -degree_; k <= lastKnot_; j++, k++)
    knots_[k] = j * step;
;
}
/* compute the position of the density knots. */
template<class Vector>
void BSplineCoefficients<Vector>::computeDensityKnots()
{
  // sorted data
  Array2DVector<Real> xtri;
  // sort the data
  heapSort< Array2DVector<Real> >(*p_data_, xtri);

  // compute step
  Real step = xtri.size()/(Real)lastKnot_;
  int first = xtri.begin(), last = xtri.lastIdx();
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
template<class Vector>
void BSplineCoefficients<Vector>::computeCoefficients()
{
#ifdef STK_REGRESS_VERBOSE
  stk_cout << _T("BSplineCoefficients::computeCoefficients()\n");
#endif
  // get dimensions
  int first = p_data_->begin(), last = p_data_->lastIdx();
  // compute the coefficients
  for (int i=first; i<= last; i++)
  {
    computeCoefficientsRow(i, (*p_data_)[i]);
  }
#ifdef STK_REGRESS_VERBOSE
  stk_cout << _T("BSplineCoefficients::computeCoefficients() done\n");
#endif
}

/* Compute a row of the coefficients
 * @param irow index of the row
 **/
template<class Vector>
void BSplineCoefficients<Vector>::computeCoefficientsRow(int irow, Real const& value)
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

#endif /* STK_BSPLINECOEFFICIENTS_H */
