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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::STatistiK
 * Purpose:  Method implementing Algorithms.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Algo.h
 *  @brief In this file we give some templated generic algorithms.
 **/

#ifndef STK_ALGO_FINDZERO_H
#define STK_ALGO_FINDZERO_H

#include "Sdk/include/STK_Macros.h"

#include "STKernel/include/STK_Integer.h"
#include "STKernel/include/STK_Real.h"

#include "STK_IFunction.h"

#define MAX_ITER 1000

namespace STK
{

namespace Algo
{
// forward declaration
template <class Function>
Real BrentMethod( IFunction<Function> const& f, Real const& x0, Real const& x1, Real tol);

/** @ingroup Analysis
 *  @brief apply the secant method for finding the zero of a function.
 *
 * Compute iteratively
 *  \f[
 *  x_{n+1} = x_n - f(x_n)\frac{x_{n}-x_{n-1}}{f(x_{n}-f(x_{n-1})}
 *  \f]
 * in order to find the zero of the function f.
 *
 * Some details of the implementation :
 * - If at some step the root is bracketed, the method switch immediately
 * to the Brent's method.
 * - If at some step the method diverge, a small line search is performed.
 * - if no descent is found, the method return a NA number.
 *
 * @param f the function
 * @param x0 the first point of the algorithm
 * @param x1 the second point of the algorithm
 * @param tol the tolerance of the method
 * @return the zero of the function if the method converge, a NA value otherwise
**/
template <class Function>
Real SecantMethod( IFunction<Function> const& f, Real const& x0, Real const& x1, Real tol)
{
  Real a = x0, b = x1, fa = f(x0), fb = f(x1);
  // set low and high values
  if (std::abs(fa) < std::abs(fb))
  {
    std::swap(a, b);
    std::swap(fa, fb);
  }
  // trivial case
  if (std::abs(fb)<tol) return b;
  // if root is bracketed use Brent method
  if (fa * fb <0.) { return BrentMethod(f, a, b, tol);}
  // start secant iterations
  while( std::abs(b-a)>tol )
  {
    Real s = b - fb * (b - a) / (fb - fa);
    Real delta = b-s;
    // if we are out of bound, we try a desperate step
    if (s<f.xmin())
    { s = (b + std::max(f.xmin(), b - std::abs(b-a))/8 )/2.; delta = b-s;}
    if (s>f.xmax())
    { s = (b + std::min(f.xmax(), b + std::abs(b-a))/8 )/2.; delta = b-s;}
    // compute f(s)
    Real fs = f(s);
    // check if we can switch to Brent method
    if (fb * fs < 0) { return BrentMethod(f, b, s, tol); }
    // handle divergence
    if (std::abs(fs)>std::abs(fb))// divergence
    {
      bool dv = true;
      for (int i=0; i<16; i++)
      {
        delta /=2.;
        s = b - delta;
        fs = f(s);
        // check if we can switch to Brent method
        if (fb * fs < 0) { return BrentMethod(f, b, s, tol); }
        // ok !
        if (std::abs(fs)<std::abs(fb)) { dv = false; break ;}
      }
      // we were not lucky...
      if (dv) return Arithmetic<Real>::NA();
    }
    // update
    a =b; fa = fb;
    b =s; fb = fs;
    // set low and high values
    if (std::abs(fa) < std::abs(fb))
    {
      std::swap(a, b);
      std::swap(fa, fb);
    }
    // trivial case
    if (std::abs(fb) <tol) return b;
  }
  return b;
}

/** @ingroup Analysis
 *  @brief perform the brent's algorithm.
 *
 * @param f the function
 * @param x0 the first point of the algorithm
 * @param x1 the second point of the algorithm
 * @param tol the tolerance of the method
 * @return the new value
**/
template <class Function>
Real BrentMethod( IFunction<Function> const& f, Real const& x0, Real const& x1, Real tol)
{
  Real a = x0, b = x1, fa = f(x0), fb = f(x1);
  // set low and high values
  if (std::abs(fa) < std::abs(fb))
  {
    std::swap(a, b);
    std::swap(fa, fb);
  }
  // trivial case
  if (std::abs(fb)<tol) return b;
  // if root is not bracketed secant method have to be used
  if (fa * fb >0.) { return SecantMethod(f, a, b, tol);}
  Real bkm1 = a, fbkm1 = fa;
  bool mflag = true;
  Real bkm2 = bkm1; // value of bkm2 is not used at the first iteration as mflag = true

  int iter =0;
#ifdef STK_ANALYSIS_VERBOSE
  stk_cout << _T("iter = ") << iter <<_T(". bkm1, a, b = ") << bkm1 << _T(" ") << a << _T(" ") << b << _T("\n");
  stk_cout << _T("iter = ") << iter <<_T(". fbkm1, fak, fbk = ") << fbkm1 << _T(" ") << fa << _T(" ") << fb << _T("\n");
#endif
  // start Brent iterations
  while( std::abs(b-a)>tol )
  {
    iter++;
    Real s;
    if ((fa != fbkm1) && (fb != fbkm1)) // inverse quadratic approximation
    {
      Real dab = fa - fb, dac = fa -fbkm1, dbc = fb -fbkm1;
      s = a * fb * fbkm1 / dab / dac
        + b * fa * fbkm1 / -dab / dbc
        + bkm1 * fa * fb / dac / dbc;
    }
    else // secante method
    { s = b - fb * (b - a) / (fb - fa);}
    // check if we shall fall-back to dichotomy
    Real tmp = (3. * a + b) / 4., diff1 =std::abs(b - bkm1), diff2 = std::abs(bkm1-bkm2);
    if (!( ((s > tmp) && (s < b)) || ((s < tmp) && (s > b)) ) // s not between b and tmp
       ||( mflag &&( (std::abs(s-b) >= diff1 / 2.)||(diff1 < tol)))
       ||(!mflag &&( (std::abs(s-b) >= diff2 / 2.)||(diff2 < tol)))
       ||(s<=f.xmin())||(s>=f.xmax())
       )
    {
       s = (a + b) / 2.;
       mflag = true;
    }
    else { mflag = false;}

    Real fs = f(s);
    // save history
    bkm2 = bkm1; bkm1 = b; fbkm1 = fb;
    // check for bracketing the root
    if (fa * fs < 0) { b = s; fb = fs; }
    else             { a = s; fa = fs; }
    // if |f(a)| < |f(b)| then swap (a, b)
    if (std::abs(fa) < std::abs(fb))
    {
      std::swap(a, b);
      std::swap(fa, fb);
    }
#ifdef STK_ANALYSIS_VERBOSE
    stk_cout << _T("iter = ") << iter <<_T(". bkm1, ak, b = ") << bkm1 << _T(" ") << ak << _T(" ") << b << _T("\n");
    stk_cout << _T("iter = ") << iter <<_T(". fbkm1, fa, fb = ") << fbkm1 << _T(" ") << fa << _T(" ") << fb << _T("\n");
#endif
    if (std::abs(fb) < tol ) return b;
    if (iter > MAX_ITER) return Arithmetic<Real>::NA();
  }
  return b;
}

/** @ingroup Analysis
 *  @brief find the zero of a function.
 *  Check if the initial values are inside the domain of definition of the
 *  function and call the Brent's method.
 *
 * @param f the function
 * @param x0 the first starting point of the algorithm
 * @param x1 the second starting point of the algorithm
 * @param tol the tolerance to apply
 * @return the zero of the function if any, a NA value otherwise
**/
template <class Function>
Real findZero( IFunction<Function> const& f, Real const& x0, Real const& x1, Real tol = Arithmetic<Real>::epsilon())
{
  if (x0<f.xmin() || x0>f.xmax() || x1<f.xmin() || x1>f.xmax() || tol<=0 )
    return Arithmetic<Real>::NA();
  return BrentMethod(f, x0, x1, tol);
}

} // namesapce Law

} // namespace STK

#undef MAX_ITER
#endif /*STK_ALGO_FINDZERO_H*/
