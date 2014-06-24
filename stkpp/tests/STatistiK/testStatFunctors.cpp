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

  CArray<Real> A(100, 20);
  Law::Normal law(1,2);
  law.randArray(A);

  stk_cout << _T("min(A.col(1) =") << min(A.col(1)) << _T("\n");
  stk_cout << _T("min(A) =") << min(A) << _T("\n");
  stk_cout << _T("max(A) =") << max(A) << _T("\n");
  stk_cout << _T("max(A.abs()) =") << max(A.abs()) << _T("\n");
  stk_cout << _T("mean(A) =") << mean(A) << _T("\n");

  A -= Const::Vector<Real>(100) * mean(A);

  stk_cout << _T("min(A) =") << min(A) << _T("\n");
  stk_cout << _T("max(A) =") << max(A) << _T("\n");
  stk_cout << _T("max(A.abs()) =") << max(A.abs()) << _T("\n");
  stk_cout << _T("mean(A) =") << mean(A) << _T("\n");

  // test static methods
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+   Test static univariate methods                  +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  Law::Exponential exp(1);
  CArrayVector<Real> w(100);
  exp.randArray(w);
  stk_cout  << _T("\n\n");
  stk_cout << _T("Test static method : mean, var, varwithFixedMean ;\n");

  Array2DPoint<Real> mu, wmu;
  mu = mean(A); wmu = mean(A, w);
  stk_cout << _T("mean(A) = ") << mu << _T("\n");
  stk_cout << _T("mean(A, w) = ") << wmu << _T("\n");

  stk_cout << _T("var(A) = ")        << variance(A) << _T("\n");
  stk_cout << _T("var(A, true) = ")  << variance(A, true) << _T("\n");
  stk_cout << _T("var(A, w) = ")     << variance(A, w) << _T("\n");
  stk_cout << _T("var(A, w, true) = ") << variance(A, w, true) << _T("\n");

  stk_cout << _T("varianceWithFixedMean(A, mu) = ")
           << varianceWithFixedMean(A, mu) << _T("\n");
  stk_cout << _T("varianceWithFixedMean(A, mu, true) = ")
           << varianceWithFixedMean(A, mu, true) << _T("\n");
  stk_cout << _T("varianceWithFixedMean(A, w, wmu) = ")
           << varianceWithFixedMean(A, w, wmu) << _T("\n");
  stk_cout << _T("varianceWithFixedMean(A, w, wmu, true) = ")
           << varianceWithFixedMean(A, w, wmu, true) << _T("\n");

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for StatFunctors +\n");
  stk_cout << _T("+ No errors detected.                               +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  return 0;
}
