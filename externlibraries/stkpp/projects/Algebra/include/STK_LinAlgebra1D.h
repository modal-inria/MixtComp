/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  stkpp::Algebra
 * Purpose:  Define 1D Linear Algebra methods with Real Containers
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_LinAlgebra1D.h
 *  @brief In this file we implement Linear Algebra methods
 *  for Real one dimensional containers.
 **/
 
#ifndef STK_LINALGEBRA1D_H
#define STK_LINALGEBRA1D_H

#include "../../STKernel/include/STK_Real.h"
#include "../../STKernel/include/STK_Misc.h"

#include "../../Arrays/include/STK_ITContainer.h"

namespace STK
{

///** @ingroup Algebra
// *  @brief Sum the element of the container
// *
// *  Sum of the element of the Container1D @c x
// *  \f[ s= \sum_{i=1}^n x_i \f]
//
// *  @param[in] x vector to treat
// *  @return the sum of the element of the container
// **/
//template<class Container1D>
//Real sum( ITContainer< Container1D> const& x)
//{
//  typedef typename hidden::Traits<Container1D>::Type Type;
//  // get dimensions
//  const int first = x.firstIdx(), last = x.lastIdx();
//  // compute the sum
//  Type sum = 0.0;
//  for (int i=first; i<=last; i++)
//    sum += x[i];
//  return (sum);
//}
//
///** @ingroup Algebra
// *  @brief find the std::maximal element of the container
// *
// * maximal element of the Container1D @c x
// *  \f[ m = \max_{i=1}^n x_i \f]
// *  @param[in] x vector to treat
// *  @return the max of the element of the container
// **/
//template<class Container1D>
//Real max( ITContainer< Container1D> const& x)
//{
//  typedef typename hidden::Traits<Container1D>::Type Type;
//  // get dimensions
//  const int  last = x.lastIdx();
//  // compute the sum
//  Type maxValue = - Arithmetic<Type>::infinity();
//  for (int i=x.firstIdx(); i<=last; i++)
//  { if (x[i] > maxValue) maxValue = x[i];}
//  return (maxValue);
//}
//
///** @ingroup Algebra
// *  @brief find the std::maximal element of the container
// *
// * std::maximal element of the Container1D @c x
// *  \f[ m = \max_{i=1}^n x_i \f]
//
// *  @param[in] x vector to treat
// *  @return the std::max of the element of the container
// **/
//template<class Container1D>
//int maxElt( ITContainer< Container1D> const& x)
//{
//  typedef typename hidden::Traits<Container1D>::Type Type;
//  // get dimensions
//  const int  last = x.lastIdx();
//  // compute the sum
//  Type maxValue = - Arithmetic<Type>::infinity();
//  int imax = x.firstIdx();
//  for (int i=x.firstIdx(); i<=last; i++)
//  { if (x[i] > maxValue) { maxValue = x[i]; imax = i;}}
//  return (imax);
//}

/** @ingroup Algebra
 *  @brief Weighted sum of the element of the container
 *
 *  Sum of the weighted elements of the Container1D x
 *  \f[ s= \sum_{i=1}^n w_i x_i \f]

 *  @param[in] x vector to treat
 *  @param w the weights to apply
 *  @return the weighted sum of the container
 **/
template<class Container1D1, class Container1D2>
Real weightedSum( ITContainer< Container1D1> const& x
                , ITContainer< Container1D2> const& w
                )
{
#ifdef STK_DEBUG
    if (!x.range().isIn(w.range()))
      throw runtime_error("In weightedSum(x, w) "
                               "x.range() not include in w.range()");
#endif
  // dimensions
  const int first = x.firstIdx(), last = x.lastIdx();
  // compute the weighted sum
  Real sum = 0.0;
  for (int i=first; i<=last; i++)
    sum += w[i] * x[i];
  return (sum);
}

/** @ingroup Algebra
 *  @brief Compute the infinity norm of a 2D container
 *
 *  Return the maximal absolute value of the 2D container x
 *  \f[ s= \max_{i=1}^n |x_{ij}| \f]
 *
 *  @param[in] A matrix to treat.
 *  @return the infinity norm of A
 **/
template < class TContainer2D >
Real normInf( ITContainer<TContainer2D> const& A)
{
  // find maximal absolute value
  Real scale = 0.0;
  for (int j=A.firstIdxCols(); j<=A.lastIdxCols(); j++)
    for (int i=A.firstIdxRows(); i<=A.lastIdxRows(); i++)
      scale = std::max(scale, std::abs(A(i,j)));
  return (scale);
}


/** @ingroup Algebra
 *  @brief Compute the weighted infinite norm
 *
 *  Compute the std::maximal absolute weighted value of the container @c x
 *  \f[ s= \max_{i=1}^n |w_i x_i| \f]
 *  @param[in] A vector to treat
 *  @param w the weights to apply
 *  @return the weighted infinite norm
 **/
template<class Container, class Weights>
Real weightedNormInf( ITContainer<Container> const& A, ITContainer<Weights> const& w)
{
#ifdef STK_DEBUG
  if (!A.range().isIn(w.range()))
    STKRUNTIME_ERROR_NO_ARG(weightedNormInf(A,w),A.range() not include in w.range());
#endif
  Real scale = 0.0;
  for (int i=A.firstIdx(); i<=A.lastIdx(); i++)
    scale = std::max(scale, std::abs(w[i]*A[i]));
  return (scale);
}

/** @ingroup Algebra
 *  @brief compute the norm two
 *
 *  Compute the norm of the container @c x avoiding overflow
 *  \f[ \|x\| = \sqrt{\sum_{i=1}^n x^2_i } \f]
 *  @param[in] x vector to treat
 *  @return the norm two of the container
 **/
template<class Container1D>
Real normTwo( ITContainer< Container1D> const& x)
{
  // compute the std::maximal value of x
  Real scale =normInf(x), norm =0.0;
  if (scale)
  {
    for (int j=x.firstIdxCols(); j<=x.lastIdxCols(); j++)
      for (int i=x.firstIdxRows(); i<=x.lastIdxRows(); i++)
      {
        const Real aux = x(i,j)/scale;
        norm += aux * aux;
      }
  }
  // rescale sum
  return (Real(sqrt(double(norm)))*scale);
}

/** @ingroup Algebra
 *  @brief compute the weighted norm two
 * 
 *  Compute the weighted norm of the container @c x avoiding overflow
 *  \f[ \|x\| = \sqrt{\sum_{i=1}^n w_i x^2_i } \f]
 *  @param[in] x vector to treat
 *  @param w the weights to apply
 *  @return the weighted two norm
 **/
template<class Container1D1, class Container1D2>
Real weightedNormTwo( ITContainer< Container1D1> const& x
                    , ITContainer< Container1D2> const& w
                    )
{
#ifdef STK_DEBUG
    if (!x.range().isIn(w.range()))
      STKRUNTIME_ERROR_NO_ARG(weightedNormTwo(x,w),x.range() not include in w.range());
#endif
  // compute the std::maximal value of x
  Real scale = weightedNormInf(x, w), norm2 =0.0;
  if (scale)
  {
    // get dimensions
    const int first = x.firstIdx(), last = x.lastIdx();
    // compute norm2
    for (int i = first; i<=last; i++)
    {
      const Real aux = (w[i]*x[i])/scale;
      norm2 += aux * aux;
    }
  }
  // rescale sum
  return (Real(sqrt(double(norm2)))*scale);
}

/** @ingroup Algebra
 *  @brief Compute the squared norm two
 *
 *  Compute the square norm of the Container1D x avoiding overflow
 *  \f[ \|x\|^2 = \sum_{i=1}^n x^2_i \f]
 *
 *  @param[in] x vector to treat
 **/
template<class Container1D>
Real normTwo2( ITContainer< Container1D> const& x)
{
  Real scale =normInf(x), norm =0.0;
  if (scale)
  {
    for (int j=x.firstIdxCols(); j<=x.lastIdxCols(); j++)
      for (int i=x.firstIdxRows(); i<=x.lastIdxRows(); i++)
      {
        const Real aux = x(i,j)/scale;
        norm += aux * aux;
      }
  }
  // scale result
  return (norm*scale*scale);
}

/** @ingroup Algebra
 *  @brief Compute the squared norm two
 * 
 *  Compute the weighted square norm two of the container x avoiding overflow
 *  \f[ \|x\|^2 = \sum_{i=1}^n w_i x^2_i \f]
 * 
 *  @param[in] x vector to treat
 *  @param w the weights to apply
 *  @return the weighted square norm two
 **/
template<class Container1D1, class Container1D2>
Real weightedNormTwo2( ITContainer< Container1D1> const& x
                     , ITContainer< Container1D2> const& w
                     )
{
  if (!x.range().isIn(w.range()))
  { STKRUNTIME_ERROR_NO_ARG(In weightedNormTwo2(x, w),x.range() not include in w.range());}
  Real scale =weightedNormInf(x, w), norm =0.0;
  if (scale)
  {
    // compute norm2
    for (int i = x.firstIdx(); i <= x.lastIdx(); i++)
    {
      const Real aux = x[i]/scale;
      norm += w[i] * aux * aux;
    }
  }
  // scale result
  return (norm*scale*scale);
}


} // Namespace STK

#endif // STK_LINALGEBRA1D_H
