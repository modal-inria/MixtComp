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

/** @file testFindZero.cpp
 *  @brief In this file we test the the zero finding algorithm.
 **/

#include "../../include/Analysis.h"


using namespace STK;
using namespace STK::Algo;

struct F1 : public IFunction<F1>
{ inline Real fImpl(Real x) const { return std::cos(x) - x*x*x;}};

struct F2 : public IFunction<F2>
{ inline Real fImpl(Real x) const { return x*x*x + 2.;}};

struct F3 : public IFunction<F3>
{ inline Real fImpl(Real x) const { return((x+3)*(x-1)*(x-1));}};

// Main
int main(int argc, char *argv[])
{
  Real res;
  try
  {
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Algo::FindZero.                      +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
    F1 f1;
    res = findZero(f1, 0, 1);
    stk_cout << _T("Result of cos(x) - x^3 = 0; res =") << res << _T("\n");
    F2 f2;
    res = findZero(f2, 10., 20.);
    stk_cout << _T("Result of x^3+2 = 0 with x_0=10, x_1 = 20\nres =")
             << res << _T("\n");
    res = findZero(f2, -10., -20.);
    stk_cout << _T("Result of x^3+2 = 0 with x_0=-10, x_1 = -20\nres =")
             << res << _T("\n");
    res = findZero(f2, -10., 20.);
    stk_cout << _T("Result of x^3+2 = 0 with x_0=-10, x_1 = 20\nres =")
             << res << _T("\n");
    F3 f3;
    res = findZero(f3, -4., 4./3.);
    stk_cout << _T("Result of f3(x) = 0 with x_0=-4, x_1 = 4/3\nres =")
             << res << _T("\n");
    //res = findZero(f3, -4., 4./3.);

    stk_cout << _T("\n\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for           +\n");
    stk_cout << _T("+ STK::Algo::FindZero.                           +\n");
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

