/*--------------------------------------------------------------------*/
/*  Copyright (C) 2004-2007  Serge Iovleff
    
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
 Cephes Math Library Release 2.2:  June, 1992
 Copyright 1984, 1987, 1988, 1992 by Stephen L. Moshier
 Direct inquiries to 30 Frost Street, Cambridge, MA 02140
 */

/*
 * Project:  Analysis
 * Purpose:  Implementation of the erf and erfc functions
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Funct_erf_raw.cpp
 *  @brief In this file we implement the erf and erfc functions
 *  using the cephes Math Library.
 **/

#include <cmath>

// for int and Real
#include "STKernel/include/STK_Misc.h"

#include "../include/STK_Const_Math.h"

#include "../include/STK_Funct_raw.h"

namespace STK
{
namespace Funct
{

static Real P[9] =
{ 2.46196981473530512524E-10,
  5.64189564831068821977E-1,
  7.46321056442269912687E0,
  4.86371970985681366614E1,
  1.96520832956077098242E2,
  5.26445194995477358631E2,
  9.34528527171957607540E2,
  1.02755188689515710272E3,
  5.57535335369399327526E2
};

static Real Q[8] =
{
  /* 1.00000000000000000000E0,*/
  1.32281951154744992508E1,
  8.67072140885989742329E1,
  3.54937778887819891062E2,
  9.75708501743205489753E2,
  1.82390916687909736289E3,
  2.24633760818710981792E3,
  1.65666309194161350182E3,
  5.57535340817727675546E2
};

static Real R[6] =
{ 5.64189583547755073984E-1,
  1.27536670759978104416E0,
  5.01905042251180477414E0,
  6.16021097993053585195E0,
  7.40974269950448939160E0,
  2.97886665372100240670E0
};

static Real S[6] =
{
  /* 1.00000000000000000000E0,*/
  2.26052863220117276590E0,
  9.39603524938001434673E0,
  1.20489539808096656605E1,
  1.70814450747565897222E1,
  9.60896809063285878198E0,
  3.36907645100081516050E0
};

static Real T[5] =
{ 9.60497373987051638749E0,
  9.00260197203842689217E1,
  2.23200534594684319226E3,
  7.00332514112805075473E3,
  5.55923013010394962768E4
};

static Real U[5] =
{
  /* 1.00000000000000000000E0,*/
  3.35617141647503099647E1,
  5.21357949780152679795E2,
  4.59432382970980127987E3,
  2.26290000613890934246E4,
  4.92673942608635921086E4
};

/* inlined poly eval functions */
static inline Real polevl(Real const& x, Real coef[], int const& N)
{
  Real *p = coef;

  int i = N;
  Real sum = *p++;
  do
    sum = sum * x + *p++;
  while ( --i);

  return ( sum );
}

static inline Real p1evl(Real const& x, Real coef[], int const& N)
{
  Real *p = coef;

  int i = N-1;
  Real sum = x + *p++;
  do
    sum = sum * x + *p++;
  while ( --i);
  
  return ( sum );
}

/** @ingroup Analysis
 * 
 *  Compute the function
 *   \f[
 *   \mathrm{erfc}(a) = \frac{2}{\sqrt{\pi}} \int_a^{+\infty} e^{-t^2} dt
 *  \f]
 *  @param[in] a value to evaluate the function
 */
Real erfc_raw(Real const& a)
{
  Real p, q, y;

  Real x = std::abs(a);
  if ( x < 1.0)  return ( 1.0 - erf_raw(a) );

  Real z = exp(-a * a);

  if ( x < 8.0)
  {
    p = polevl( x, P, 8);
    q = p1evl( x, Q, 8);
  }
  else
  {
    p = polevl( x, R, 5);
    q = p1evl( x, S, 6);
  }
  y = (z * p)/q;

  if ( a < 0)  y = 2.0 - y;
  // result
  return (y);
}

/** @ingroup Analysis
 * 
 *  Compute the function
 *   \f[
 *   \mathrm{erf}(a) = \frac{2}{\sqrt{\pi}} \int_0^{a} e^{-t^2} dt
 *  \f]
 *  @param[in] a value to evaluate the function
 */

Real erf_raw(Real const& a)
{
  if ( std::abs(a)> 1.0) return ( 1.0 - erfc(a) );
  Real z = a * a;
  // result
  return ( a * polevl( z, T, 4)/ p1evl( z, U, 5 ));
}

} // namespace Funct
  
} // namespace STK
