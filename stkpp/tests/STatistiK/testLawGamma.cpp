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
 * Project:  Analysis
 * Purpose:  test program for testing gamma class and static methods.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testLawGamma.cpp
 *  @brief In this file we test the distribution law Gamma
 **/

#include "STKpp.h"

//#include <cmath>
#include <iomanip>
#define MATHLIB_STANDALONE 1
#include <Rmath.h>

using namespace STK;

/**
 * test the Gamma function
 **/
static void testGamma( Real a, Real b)
{
  //
  Real x;
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("Test Law Gamma with\n")
           << _T("a=") << a << _T("\n")
           << _T("b=") << b << _T("\n");
  Law::Gamma law(a,b);
  Real max = 0., lmax =0.;
  try
  {
    for (x = 1/(4294967296.); x<=(4294967296.); x*=1.0625)
    {
      Real error = std::abs( dgamma(x, a, b, false)-law.pdf(x) );
//                    / dgamma(x, a, b, false);
      max = std::max(error, max);
      error = std::abs( dgamma(x, a, b, true)-law.lpdf(x) );
//                    / dgamma(x, a, b, true);
      lmax = std::max(error, lmax);
    }
  }
  catch (Exception & error)
  { stk_cerr << "An error occured : " << error.error() << _T("\n";);}
  stk_cout << _T("Errors\n")
           << _T("pdf max=") << max << _T("\n")
           << _T("lpdf max=") << lmax << _T("\n");
}


// Main
int main(int argc, char *argv[])
{
  Real a, b;
  if (argc < 3)
  {
    a= 1; b=1.;
  }
  else
  {
    a = std::atof(argv[1]);
    b = std::atof(argv[2]);
  }
   stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Law::Gamma.                          +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
    testGamma(a, b);

    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for           +\n");
    stk_cout << _T("+               STK::Law::Gamma                  +\n");
    stk_cout << _T("+ No errors detected.                            +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  return 0;
}

