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
 * created on: 6 oct. 2013
 * Author:   iovleff,  S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testBetaRatio.cpp
 *  @brief In this file we test the betaRatio function.
 **/

#include "../../include/Analysis.h"
#include "../../include/STatistiK.h"
#include "../../include/Arrays.h"
#include "../../include/STKernel.h"

#include <iomanip>
#include <Rmath.h>

using namespace STK;

const int nbTry = 1000;

static void printBetaRatio( CArrayVector<double, nbTry> stkRes
                          , CArrayVector<double, nbTry> rRes
                          , CArrayVector<double, nbTry> a
                          , CArrayVector<double, nbTry> b
                          , CArrayVector<double, nbTry> x
                          , Real rTime
                          , Real stkTime
                          )
{
  int k; Real max = (stkRes - rRes).abs().maxElt(k);
  stk_cout << _T("max =") << max << _T(" With a =")
           << a[k]  << _T(", b =") << b[k] << _T(" and x =") << x[k]
           << _T("\n");
//  stk_cout << _T("mean =") << (stkRes - rRes).abs().mean()  << _T("\n");
//  stk_cout << _T("R Elapsed time =") << rTime  << _T(" and ");
//  stk_cout << _T("stk++ Elapsed time =") << stkTime  << _T("\n");
//  stk_cout << _T("\n");
}


/**test the beta Ratio function */
static void testFunctBetaRatio()
{
  const double abmax = 200.;
  CArrayVector<double, nbTry> stkRes, rRes, a, b, x;
  RandBase generator;

  try
  {
    for (double ainf = 0., asup =4.61 ; ainf<abmax; ainf+=4.61, asup+=4.61)
    {
      Law::Uniform lawa(ainf, asup);
      for (double binf = 0., bsup =4.61 ; binf<abmax; binf+=4.61, bsup+=4.61)
      {
        Law::Uniform lawb(binf, bsup);
        for (int iTry = 0, i = rRes.firstIdx(); iTry < nbTry; iTry++, i++)
        {
          a[i] = lawa.rand();
          b[i] = lawb.rand();
          x[i] = generator.randUnif();
        }
        // start R computation
        Chrono::start();
        for (int iTry = 0, i = rRes.firstIdx(); iTry < nbTry; iTry++, i++)
        { rRes[i] = pbeta(x[i], a[i], b[i], true, false);}
        Real rTime = Chrono::elapsed();
        // start computation
        Chrono::start();
        for (int iTry = 0, i = rRes.firstIdx(); iTry < nbTry; iTry++, i++)
        { stkRes[i] = Funct::betaRatio(a[i],b[i],x[i],true);}
        Real stkTime = Chrono::elapsed();

        printBetaRatio(stkRes, rRes, a, b, x, rTime, stkTime);
      }
    }
  }
  catch (Exception & error)
  {
    std::cerr << _T("An error occured : ") << error.error() << _T("\n");
    return;
  }
  return;
}

// Main
int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test betaRatio function                        +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

    testFunctBetaRatio();

    stk_cout << _T("\n\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing betaRatio.    +\n");
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





