/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1987, 1992, 2000 by Stephen L. Moshier
*/

/*
 * Project:  stkpp::Analysis
 * created on: 24 juin 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Funct_psi_raw.cpp
 *  @brief In this file we implement the psi function.
 **/

#include <cmath>
#include "../include/STK_Funct_raw.h"
#include "../include/STK_Funct_util.h"
#include "../include/STK_Const_Math.h"

/** The Bernouilli numbers divided by 2k */
static double A[] = {
 8.333333333333333333333333333333333333333333333333333333333333333333333E-2,
-2.10927960927960927961092796092796092796092796092796092796092796092796E-2,
 7.575757575757575757575757575757575757575757575757575757575757575757576E-3,
-4.166666666666666666666666666666666666666666666666666666666666666666667E-3,
 3.968253968253968253968253968253968253968253968253968253968253968253968E-3,
-8.333333333333333333333333333333333333333333333333333333333333333333333E-3,
 8.333333333333333333333333333333333333333333333333333333333333333333333E-2
};

namespace STK
{

namespace Funct
{
/** Compute the psi function
 * \f[
 *   \psi(x)  =\frac{d}{dx}(-\ln(\Gamma(x)))
 * \f]
 * the logarithmic derivative of the gamma function.
 *
 * For integer x, we use the formula
 * \f[
 * \psi(n) = -\gamma + \sum_{k=1}^{n-1}  \frac{1}{k}.
 * \f]
 * This formula is used for 0 < n <= 20.
 *
 * If x is negative, it is transformed to a positive argument by the reflection
 * formula \f$ \psi(1-x) = \psi(x) + \pi \cot(\pi x) \f$.
 *
 * For general positive x, the argument is made greater than 10
 * using the recurrence  \f$ \psi(x+1) = \psi(x) + 1/x \f$.
 * Then the following asymptotic expansion is applied:
 * \f[
 * \psi(x) = \log(x) - \frac{1}{2} x - \sum_{k=1}^{\infty} \frac{B_{2k}}{2k x^{2k}}
 * \f]
 * where the \f$ B_{2k} \f$ are Bernoulli numbers.
 **/

Real psi_raw( Real x)
{
  Real s, w, y, z;

  bool negative = false;
  Real nz = 0.0;

  if( x <= 0.0 ) // use reflection formula
  {
    negative = true;
    Real q = x;
    Real p = std::floor(x);
    // negative integer
    if( p == q ) { return( Arithmetic<Real>::infinity() ); }
    // Remove the zeros of tan(pi x) by subtracting the nearest integer from x
    nz = q - p;
    if( nz != 0.5 )
    {
      if( nz > 0.5 ) { nz = q - p - 1.;}
      nz = Const::_PI_/std::tan(Const::_PI_*nz);
    }
    else { nz = 0.0;}
    x = 1.0 - x;
  }
  if (x==1.) return -Const::_EULER_;
  // check for positive integer up to 20
  if( (x <= 20.0) && (x == std::floor(x)) )
  {
    y = 0.0;
    for( int i=int(x)-1; i>1; i-- ) { y += 1.0/(Real)i; }
    y += -Const::_EULER_ + 1.;
  }
  else // not integer or greater than 20
  {
    s = x;
    w = 0.0;
    while( s < 10.0 ) // shift to
    {
      w += 1.0/s;
      s += 1.0;
    }
    z = 1.0/(s * s);
    y = std::log(s) - (0.5/s) - z * evalPolynomial<6>( z, A) -  w;
  }
  return (negative) ? y - nz : y;
}
} // namespace Funct

} // namespace STK


