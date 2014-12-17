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
 * Project:  stkpp::STatistiK::StatDesc
 * Purpose:  test program for testing Analysis classes and methods.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testStatFunctors.cpp
 *  @brief In this file we test the statistical functors classes.
 **/

#include "STKpp.h"

using namespace STK;
using namespace STK::Stat;


/* main.*/
int main(int argc, char *argv[])
{

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+   Test StatFunctors classes                       +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  CArray<Real> A(100, 5);
  CArrayVector<Real> w(100);
  Law::Normal law(1,2);
  Law::Exponential exp(1);
  law.randArray(A);
  exp.randArray(w);

  stk_cout << _T("min(A.col(1) =") << min(A.col(1)) << _T("\n");
  stk_cout << _T("min(A) =\n") << min(A);
  stk_cout << _T("max(A) =\n") << max(A);
  stk_cout << _T("max(A.abs()) =\n") << max(A.abs());
  stk_cout << _T("mean(A) =\n") << mean(A);
  stk_cout << _T("mean(A,w) =\n") << mean(A,w);
  stk_cout << _T("variance(A) =\n") << variance(A);
  stk_cout << _T("variance(A,w) =\n") << variance(A,w);
  stk_cout << _T("sd(A) =\n") << variance(A).sqrt();

  stk_cout << _T("\n\n");
  stk_cout << _T("Centering A.\n");
  Array2DPoint<Real> m = mean(A);
  A -= Const::Vector<Real>(100) * m;

  stk_cout << _T("min(A) =\n") << min(A);
  stk_cout << _T("max(A) =\n") << max(A);
  stk_cout << _T("max(A.abs()) =\n") << max(A.abs());
  stk_cout << _T("mean(A) =\n") << mean(A);
  stk_cout << _T("varianceWithFixedMean(A,m) =\n")   << varianceWithFixedMean(A,m);
  stk_cout << _T("varianceWithFixedMean(A,w,m) =\n") << varianceWithFixedMean(A,w,m);

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for StatFunctors +\n");
  stk_cout << _T("+ No errors detected.                               +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  return 0;
}
