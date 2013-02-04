/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::STKernel
 * created on: 19 oct. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file testFunctors.cpp
 *  @brief In this file we test the Functors.
 **/

#include "../include/STKpp.h"

using namespace STK;


/* main.   */
int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Functors                                      +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
    Real x=10, y=20, z=18;
    int a=8, b=18;

    stk_cout << _T("MinOp<int, Real>()(") << a << _T(",") << x <<_T(") = ")
             << MinOp<int, Real>()(a,x) << _T("\n");
    stk_cout << _T("QuotientOp<Real>(") << z << _T(")(") << y <<_T(") = ")
             << QuotientOp<Real>(z)(y) << _T("\n");
    stk_cout << _T("QuotientOp<int>(") << a << _T(")(") << b <<_T(") = ")
             << QuotientOp<int>(a)(b) << _T("\n");
    stk_cout << _T("QuotientOp<int>(") << b << _T(")(") << a <<_T(") = ")
                 << QuotientOp<int>(b)(a) << _T("\n");
    stk_cout << _T("MultipleOp<Real>(") << x << _T(")(") << y <<_T(") = ")
                 << MultipleOp<Real>(x)(y) << _T("\n");
    stk_cout << _T("DivOp<Real, Real>(") << x << _T(")(") << y <<_T(") = ")
             << DivOp<Real, Real>()(x,y) << _T("\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for Functors      +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << "\n";
  }
  return 0;
}





