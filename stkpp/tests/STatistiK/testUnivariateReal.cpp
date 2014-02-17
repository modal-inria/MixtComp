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

/** @file testUnivariateReal.cpp
 *  @brief In this file we test the Univariate class.
 **/


#include "../../include/STKpp.h"

using namespace STK;
using namespace STK::Stat;

void writeStatInit(const Univariate< Variable<Real> >& S)
{
  stk_cout << _T("Univariate (init)\n");
  stk_cout << _T("nb samples = ")      << S.nbSamples()
           << _T(", nb available samples = ") << S.nbAvailableSamples()
           << _T(", nbMissing samples = ") << S.nbMissingSamples()
           << _T("\n");
  stk_cout << _T("min = ")    << S.min()
           << _T(", max = ")  << S.max()
           << _T("\n");
  stk_cout << _T("\n\n");

}

void writeStatOrder(const Univariate< Variable<Real> >& S)
{
  stk_cout << _T("Univariate (Order Statistics)\n");
  stk_cout << _T("median = ") << S.median() << _T("\n";);
  stk_cout << _T("Quartiles = \n") <<  S.quartiles() << _T("\n");
  stk_cout << _T("Deciles = \n") <<  S.deciles() << _T("\n\n");
}

void writeStatComp(const Univariate< Variable<Real> >& S)
{
  stk_cout << _T("Univariate (Statistics)\n");

  stk_cout << _T("mean = ") << S.mean()
           << _T(", mad = ") << S.mad()
           << _T(", variance = ") << S.variance()
           << _T(", unbiased variance = ") << S.unbiasedVariance()
           << _T(", std = ") << S.std()
           << _T(", unbiased std = ") << S.unbiasedStd()
           << _T(", skewness = ") << S.skewness()
           << _T(", kurtosis = ") << S.kurtosis() << _T("\n\n");
}


/* main.                                                              */
int main(int argc, char *argv[])
{
  int M;
  if (argc < 2)
  {
    M= 10;
  }
  else
    M = int(atoi(argv[1]));
  //
  int i;
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+   Test UnivariateReal class                       +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  Variable<Real> X(Range(0,M),String(_T("X")));
  for(i=0; i< M; i++) { X[i] = (Real)i;}

  // first test
  stk_cout << _T("First Test : S(V)\n");
  stk_cout << X << _T("\n\n");

  Univariate< Variable<Real> > S2(X);
  writeStatInit(S2);
  writeStatOrder(S2);
  writeStatComp(S2);

  // second test
  Variable<Real> W(Range(0,M));
  for(i=0; i< M; i++) { W[i] = (Real)i/(Real)(M);}

  stk_cout << _T("Second Test : R(X, W, false)\n");
  stk_cout << X << _T("\n\n");
  stk_cout << _T("W =") << W << _T("\n\n");

  Univariate< Variable<Real> > R2(X, W, false);
  writeStatInit(R2);
  writeStatOrder(R2);
  writeStatComp(R2);

  // third test
  stk_cout << _T("\n\n");
  stk_cout << _T("Third Test : V[0] = . and V[V.size()] = .; Q(V)\n");

  X.front() = Arithmetic<Real>::NA();
  X.back()  = Arithmetic<Real>::NA();
  stk_cout << _T("X =\n") << X << _T("\n\n");
  stk_cout << _T("W =") << W << _T("\n\n");

  Univariate< Variable<Real> > Q2(X);
  writeStatInit(Q2);
  writeStatOrder(Q2);
  writeStatComp(Q2);

  // fourth test
  stk_cout << _T("\n\n");
  stk_cout << _T("Fourth Test : P(X, W)\n");
  stk_cout << _T("X =\n") << X << _T("\n\n");
  stk_cout << _T("W =") << W << _T("\n\n");

  Univariate<Variable<Real> > P2(X, W, false);
  writeStatInit(P2);
  writeStatOrder(P2);
  writeStatComp(P2);

  // fifth test
  stk_cout << _T("\n\n");
  stk_cout << _T("Fourth Test : P.setData(X, W)\n");
  stk_cout << _T("X =\n") << X << _T("\n\n");
  stk_cout << _T("W =") << W << _T("\n\n");

  P2.setData(X, W, false);
  writeStatInit(P2);
  writeStatOrder(P2);
  writeStatComp(P2);

  // fifth test
  stk_cout << _T("\n\n");
  stk_cout << _T("Fifth Test : S.setData(X, W)\n");
  stk_cout << _T("X =\n") << X << _T("\n\n");
  stk_cout << _T("W =") << W << _T("\n\n");

  S2.setData(X, W, false);
  writeStatInit(S2);
  writeStatOrder(S2);
  writeStatComp(S2);

  // sixth test
  stk_cout << _T("\n\n");
  stk_cout << _T("Sixth Test : P.setData(V)\n");
  stk_cout << _T("X =\n") << X << _T("\n\n");

  P2.setData(X, false);
  writeStatInit(P2);
  writeStatOrder(P2);
  writeStatComp(P2);

  stk_cout << _T("Test Univariate(X) ;\n");
  Univariate< Variable<Real> > ustat(X);
  for(i=0; i< M; i++) { W[i] = 1./(Real)(M);}
  stk_cout << _T("X =\n") << X << _T("\n");
  stk_cout << _T("W =")   << W << _T("\n\n");

  stk_cout << _T("mean = ") << ustat.mean() << _T("\n");
  stk_cout << _T("variance = ") << ustat.variance() << _T("\n");
  stk_cout << _T("std = ") << ustat.std() << _T("\n");
  stk_cout << _T("unbiasedVariance = ") << ustat.unbiasedVariance() << _T("\n");
  stk_cout << _T("unbiasedStd = ") << ustat.unbiasedStd() << _T("\n");

  stk_cout << _T("\n\n");
  stk_cout << _T("setData(X,W), with W = 1/M\n");
  ustat.setData(X, W);
  stk_cout << _T("mean = ") << ustat.mean() << _T("\n");
  stk_cout << _T("variance = ") << ustat.variance() << _T("\n");
  stk_cout << _T("std = ") << ustat.std() << _T("\n");
  stk_cout << _T("unbiasedVariance = ") << ustat.unbiasedVariance() << _T("\n");
  stk_cout << _T("unbiasedStd = ") << ustat.unbiasedStd() << _T("\n");

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for UnivariateReal+\n");
  stk_cout << _T("+ No errors detected.                               +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  return 0;
}
