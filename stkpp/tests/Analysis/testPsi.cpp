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
    
    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::Analysis
 * Purpose:  test program for testing zero finding algorithm.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testPsi.cpp
 *  @brief In this file we test the the zero finding algorithm.
 **/

#include "Analysis.h"
#include "STatistiK.h"


using namespace STK;
using namespace STK::Algo;

#define EUL 0.57721566490153286061

double polevl( double x, double coef[], int N )
{
double ans;
int i;
double *p;

p = coef;
ans = *p++;
i = N;

do
  ans = ans * x  +  *p++;
while( --i );

return( ans );
}

static double A[] = {
 8.33333333333333333333E-2,
-2.10927960927960927961E-2,
 7.57575757575757575758E-3,
-4.16666666666666666667E-3,
 3.96825396825396825397E-3,
-8.33333333333333333333E-3,
 8.33333333333333333333E-2
};

double psi(double x)
{
double p, q, nz, s, w, y, z;
int i, n, negative;

negative = 0;
nz = 0.0;

if( x <= 0.0 )
  {
  negative = 1;
  q = x;
  p = floor(q);
  if( p == q )
    {
    stk_cout << _T("Error: negative integer in psi (cephes)\n");
    return( STK::Arithmetic<double>::NA() );
    }
/* Remove the zeros of tan(PI x)
 * by subtracting the nearest integer from x
 */
  nz = q - p;
  if( nz != 0.5 )
    {
    if( nz > 0.5 )
      {
      p += 1.0;
      nz = q - p;
      }
    nz = Const::_PI_/std::tan(Const::_PI_*nz);
    }
  else
    {
    nz = 0.0;
    }
  x = 1.0 - x;
  }

/* check for positive integer up to 10 */
if( (x <= 10.0) && (x == std::floor(x)) )
  {
  y = 0.0;
  n = x;
  for( i=1; i<n; i++ )
    {
    w = i;
    y += 1.0/w;
    }
  y -= EUL;
  goto done;
  }

s = x;
w = 0.0;
while( s < 10.0 )
  {
  w += 1.0/s;
  s += 1.0;
  }

if( s < 1.0e17 )
  {
  z = 1.0/(s * s);
  y = z * polevl( z, A, 6 );
  }
else
  y = 0.0;

y = log(s)  -  (0.5/s)  -  y  -  w;

done:

if( negative )
  {
  y -= nz;
  }

return(y);
}
// Main
int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Funct::psi_raw.                      +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
    for(int i=0; i<=100; i++)
    {
      Real x = STK::Law::Normal::rand(1, 300);
      // test evalPolynomial
      Real diff = polevl(double(x),A,6) - Funct::evalPolynomial<6>(x, A);
      if (std::abs(diff) > Arithmetic<Real>::epsilon())
      {
        stk_cout << _T("x=") << x
                 << _T(", polevl(x)=") << polevl(double(x),A,6)
                 << _T(", evalPolynomial<6>(x, A)=") << Funct::evalPolynomial<6>(x, A)
                 << _T("\n");
      }
      // test psi_raxw
      diff = psi(double(x)) - Funct::psi_raw(x);
      if (std::abs(diff) > Arithmetic<Real>::epsilon())
      {
        stk_cout << _T("x=") << x
                 << _T(", psi(x)=") << psi(double(x))
                 << _T(", psi_raw(x)=") << Funct::psi_raw(x)
                 << _T("\n");
      }
    }
    stk_cout << _T("\n\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for           +\n");
    stk_cout << _T("+ STK::Funct::psi_raw.                           +\n");
    stk_cout << _T("+ No errors detected.                            +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    stk_cerr << _T("An error occured : ") << error.error() << _T("\n");
  }
  return 0;
}

