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
    
    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  Analysis
 * Purpose:  test program for testing Analysis classes and methods.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 */

/** @file testAnalysis.h
 *  @brief In this file we test the Analysis classes (special functions).
 **/

#include "../include/STKpp.h"
#include "../include/Algebra.h"
#include "../include/STatistiK.h"

//extern "C"
//{
//#include "../include/Rmath.h"
//}

//#include <cmath>
#include <iomanip>
#include <Rmath.h>

using namespace STK;

/*
 * Choice of a test Constant
 */
static void testConst()
{
  try
  {
    stk_cout << _T("test Const::pi() and (pi()-Const::_PI_)/Const::_PI_ : \n");
    stk_cout << std::setw(18)
             << Const::pi()
             << _T(" ")
             << (Const::pi()-Const::_PI_)/Const::_PI_
             << _T("\n");

    stk_cout << _T("test Const::euler() : \n");
    stk_cout << std::setw(18)
             << Const::euler()
             << _T(" ")
             << (Const::euler()-Const::_EULER_)/Const::_EULER_
             << _T("\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured in testConst : \n"
              << error.error()
              << "\n";
  }
  return;
}

static void printGammaRatio( Real const& amin, Real const& amax
                           , Real const& xmin, Real const& xmax
                           )
{
  stk_cout << _T("\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ ") << amin << _T(" <= a <= ") << amax << _T("\n")
           << _T("+ ") << xmin << _T(" <= x <= ") << xmax << _T("\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n");
  stk_cout << _T("|  a           ")
           << _T("|  x           ")
           << _T("| gammaRatioP(a, x)")
           << _T("| R::pgamma( a, x) ")
           << _T("| P relative error ")
           << _T("| Time diff        ")
           << _T("| gammaRatioQ(a, x)")
           << _T("| R::qgamma( a, x) ")
           << _T("| Q relative error ")
           << _T("| Time diff        ")
           << _T("|\n");
}

/**
 * test the Gamma function
 **/
static void testFunctGamma(Real const& Emax)
{
  //
  Real x;
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test Funct::Gamma.                               +\n");
  stk_cout << _T("\n\n");

  try
  {
    stk_cout << _T("|\n\n");
    stk_cout << _T("\n Test gamma\n");
    stk_cout << _T("|           x")
              << _T("|     gammafn")
              << _T("|     gamma  ")
              << _T("|     error  ")
              << _T("|\n");

    for (x = -170.001; x<=170.001; x+=0.01)
    {
      Real error = ( gammafn(x)-Funct::gamma(x) )/ gammafn(x);
      if (std::abs(error)>Emax)
      {
        stk_cout << _T("|") << std::setw(12) << x
                 << _T("|") << std::setw(12) << gammafn(x)
                 << _T("|") << std::setw(12) << Funct::gamma(x)
                 << _T("|") << std::setw(12) << error
                 << _T("|\n");
      }
    }
    stk_cout << _T("|\n\n");
    stk_cout << _T("\n Test gammaLn\n");
    stk_cout << _T("|           x")
             << _T("|    lgammafn")
             << _T("|    gammaLn ")
             << _T("|     error  ")
             << _T("|\n");
    for (x = -1000.001; x<=1000.001; x+=0.01)
    {
      Real error = ( lgammafn(x)-Funct::gammaLn(x) )/ lgammafn(x);
      if (std::abs(error)>Emax)
      {
        stk_cout << _T("|") << std::setw(12) << x
        << _T("|") << std::setw(12) << lgammafn(x)
        << _T("|") << std::setw(12) << Funct::gammaLn(x)
        << _T("|") << std::setw(12) << error
        << _T("|\n");
      }
    }
  }
  catch (Exception & error)
  {
    std::cerr << _T("An error occured : ") << error.error() << _T("\n");
  }
  return;
}

/** Evaluation of the STK gamma Ratio function, comparison with the R
 *  function and output results.
 **/
static void evalGammaRatio( Real const& a, Real const& x
                          , Real const& Emax, const bool & flag
                          )
{
  // lower_tail computation
  Chrono::start();
  Real Pcur = Funct::gammaRatio(a, x, true);
  Real Ptime = Chrono::elapsed();
  
  Chrono::start();
  Real Rpg = pgamma(x, a, 1., true, false);
  Real Rptime = Chrono::elapsed();
  
  Real Errp = (std::abs(Pcur-Rpg)<Emax) ? Pcur-Rpg : (Pcur-Rpg)/Rpg;

  // upper_tail computation
  Chrono::start();
  Real Qcur = Funct::gammaRatio(a, x, false);
  Real Qtime = Chrono::elapsed();
  
  Chrono::start();
  Real Rqg = pgamma(x, a, 1., false, false);
  Real Rqtime = Chrono::elapsed();

  Real Errq = (std::abs(Qcur-Rqg)<Emax) ? Qcur-Rqg : (Qcur-Rqg)/Rqg;

  // write output if needed
  if ( (std::abs(Errp) > Emax)|(std::abs(Errq) > Emax)
     |(Arithmetic<Real>::isNA(Pcur))|(Arithmetic<Real>::isNA(Qcur))
     |(Arithmetic<Real>::isNA(Rpg))|(Arithmetic<Real>::isNA(Rqg))
     |flag
     )
  {
    stk_cout << _T("\n")
              << _T("|") << std::setw(14) << a
              << _T("|") << std::setw(14) << x
              << _T("|") << std::setw(18) << Pcur
              << _T("|") << std::setw(18) << Rpg
              << _T("|") << std::setw(18) << Errp
              << _T("|") << std::setw(18) << Ptime-Rptime
              << _T("|") << std::setw(18) << Qcur
              << _T("|") << std::setw(18) << Rqg
              << _T("|") << std::setw(18) << Errq
              << _T("|") << std::setw(18) << Qtime-Rqtime
              << _T("|");
  }
}

/**
 * test the Gamma Ratio function
 **/
static void testFunctGammaRatio(Real const& Emax)
{
  //
  Real amin, amax, xmin, xmax = 1.;
  Real a, x;
  int count = 0;
  stk_cout << _T("\n\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test Funct::gammaRatio.                          +\n");
  try
  {
    // region a<= 1, b >=40
    amin = 1/(4294967296.*4294967296.); amax = 4294967296.*4294967296.;
    xmin = 1/(4294967296.*4294967296.); xmax = 4294967296.*4294967296.;
    count = 0;
    // print header
    printGammaRatio(amin, amax, xmin, xmax);
    // test values
    for ( a = amin; a<=amax; a*=1.0625)
        for ( x = xmin; x<=xmax; x*=1.0625 )
    {
      count++;
      evalGammaRatio(a, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << "\n";
    return;
  }
  stk_cout << _T("\n");
  stk_cout << _T("+ Test Funct::gammaRatio succesfull.               +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return;
}

/**
 * test the Poisson function
 **/
static void testFunctPoisson(Real const& Emax)
{
  //
  Real l,x;
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test Funct poisson.                         +\n");
  stk_cout << _T("\n\n");

  try
  {
    stk_cout << _T("|\n\n");
    stk_cout << _T("\n Test poisson\n");
    stk_cout << _T("|     lambda ")
              << _T("|          x ")
              << _T("| poisson    ")
              << _T("|R::ppois(l, x)")
              << _T("| error      ")
              << _T("|\n");

    for (l = 1/(4294967296.); l<=(4294967296.); l*=1.0625)
      for (x = 1/(4294967296.); x<=(4294967296.); x*=1.0625)
      {
        Real error = ( dgamma(l, x+1, 1, false)-Funct::poisson_pdf_raw(x, l) )
                     / dgamma(l, x+1, 1, false);
        if (std::abs(error)>Emax)
        {
          stk_cout << _T("|") << std::setw(12) << l
                   << _T("|") << std::setw(12) << x
                   << _T("|") << std::setw(12) << Funct::poisson_pdf_raw(x, l)
                   << _T("|") << std::setw(14) << dgamma(l, x+1, 1, false)
                   << _T("|") << std::setw(12) << error
                   << _T("|\n");
        }
    }
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << "\n";
  }
  return;
}

static void printBetaRatio( Real const& amin, const Real amax
                          , Real const& bmin, const Real bmax
                          , Real const& xmin, const Real xmax
                          )
{
  stk_cout << _T("\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ ") << amin << _T(" <= a <= ") << amax << _T("\n")
            << _T("+ ") << bmin << _T(" <= b <= ") << bmax << _T("\n" )
            << _T("+ ") << xmin << _T(" <= x <= ") << xmax << _T("\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n");
  stk_cout << _T("|  a           ")
            << _T("|  b           ")
            << _T("|  x           ")
            << _T("| betaRatioP( a, x)")
            << _T("|R::pbeta( a, b, x)")
            << _T("| P relative error ")
            << _T("| Time diff        ")
            << _T("| betaRatioQ( a, x)")
            << _T("|R::qbeta( a, b, x)")
            << _T("| Q relative error ")
            << _T("| Time diff        ")
            << _T("|");
  stk_cout << _T("\n");
}

/** Evaluation of the STK beta Ratio function, comparison with the R
 *  function and output results.
 **/
static void evalBetaRatio( Real const& a, Real const& b, Real const& x
                         , Real const& Emax, const bool & flag
                         )
{
  // lower_tail computation
  Chrono::start();
  Real Pcur = Funct::betaRatio(a, b, x, true);
  Real Ptime = Chrono::elapsed();
  
  Chrono::start();
  Real Rpg = pbeta(x, a, b, true, false);
  Real Rptime = Chrono::elapsed();
  
  Real Errp = (std::abs(Pcur-Rpg)<Emax) ? Pcur-Rpg : (Pcur-Rpg)/Rpg;

  // upper_tail computation
  Chrono::start();
  Real Qcur = Funct::betaRatio(a, b, x, false);
  Real Qtime = Chrono::elapsed();
  
  Chrono::start();
  Real Rqg = pbeta(x, a, b, false, false);
  Real Rqtime = Chrono::elapsed();

  Real Errq = (std::abs(Qcur-Rqg)<Emax) ? Qcur-Rqg : (Qcur-Rqg)/Rqg;

  // write output if needed
  if ( (std::abs(Errp) > Emax)|(std::abs(Errq) > Emax)
     |(Arithmetic<Real>::isNA(Pcur))|(Arithmetic<Real>::isNA(Qcur))
     |(Arithmetic<Real>::isNA(Rpg))|(Arithmetic<Real>::isNA(Rqg))
     |flag
     )
  {
    stk_cout << _T("|")  << std::setw(14) << a
              << _T("|") << std::setw(14) << b
              << _T("|") << std::setw(14) << x
              << _T("|") << std::setw(18) << Pcur
              << _T("|") << std::setw(18) << Rpg
              << _T("|") << std::setw(18) << Errp
              << _T("|") << std::setw(18) << Ptime-Rptime
              << _T("|") << std::setw(18) << Qcur
              << _T("|") << std::setw(18) << Rqg
              << _T("|") << std::setw(18) << Errq
              << _T("|") << std::setw(18) << Qtime-Rqtime
              << _T("|\n");
  }
}

/**
 * test the beta Ratio function
 **/
static void testFunctBetaRatio(Real const& Emax)
{
  Real amin, amax, bmin, bmax, xmin, xmax;
  Real a, b, x;
  int count = 0;
  // 
  stk_cout << _T("\n\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test Beta Ratio Function.                        +\n");
  try
  {
    // region 15<= a,b
    amin = 15;              amax = 4294967296.*4294967296.;
    bmin = 15.;             bmax = 4294967296.*4294967296.;
    xmin = 1/(4294967296.*4294967296.);  xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.0625)
      for (b = bmin; b<=bmax; b*=1.0625 )
      {
        for (x = xmin; x<=xmax; x*=1.25 )
        {
          count++;
          evalBetaRatio(a, b, x, Emax, count==100000);
          if (count == 100000) count = 0;
        }
    }
    // region 1<= a,b <=15
    amin = 1.; amax = 15.;
    bmin = 1.; bmax = 15.;
    xmin = 1/(4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.0625)
      for (b = bmin; b<=bmax; b*=1.0625)
        for (x = xmin; x<=xmax; x*=1.25)
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region a<=1 and b<=1
    amin = 1/( 4294967296.*4294967296.); amax = 1.;
    bmin = 1/( 4294967296.*4294967296.); bmax = 1.;
    xmin = 1/( 4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.25)
      for (b = bmin; b<=bmax; b*=1.25)
        for (x = xmin; x<=xmax; x*=1.25)
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region a<=1 and 1<=b<=15
    amin = 1/( 4294967296.*4294967296.); amax = 1.;
    bmin = 1.; bmax = 15.;
    xmin = 1/(4294967296.*4294967296.);  xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.25)
      for (b = bmin; b<=bmax; b*=1.0625 )
        for (x = xmin; x<=xmax; x*=1.25 )
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region 1<=a<=15 and b<=1
    amin = 1.; amax = 15.;
    bmin = 1/( 4294967296.*4294967296.); bmax = 1.;
    xmin = 1/( 4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.0625)
      for (b = bmin; b<=bmax; b*=1.25)
        for (x = xmin; x<=xmax; x*=1.25)
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region 15<= a, 0<= b <=1
    amin = 15.; amax = 4294967296.*4294967296.;
    bmin = 1/(4294967296.*4294967296.); bmax = 1;
    xmin = 1/(4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.0625)
      for (b = bmin; b<=bmax; b*=1.25)
        for (x = xmin; x<=xmax; x*=1.25)
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region O<= a <=1, 15<= b
    amin = 1./(4294967296.*4294967296.); amax = 1;
    bmin = 15.; bmax = 4294967296.*4294967296.;
    xmin = 1./(4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for (a = amin; a<=amax; a*=1.25)
      for (b = bmin; b<=bmax; b*=1.0625)
        for (x = xmin; x<=xmax; x*=1.25)
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region 1<= a <=15 b >=15
    amin = 1.0;             amax = 15.;
    bmin = 15.;             bmax = 4294967296.*4294967296.;
    xmin = 1/(4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for ( a = amin; a<=amax; a*=1.0625)
      for ( b = bmin; b<=bmax; b*=1.0625 )
        for ( x = xmin; x<=xmax; x*=1.25 )
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // region a >=15, 1<= b <=15
    amin = 15.;             amax = 4294967296.*4294967296.;
    bmin = 1.0;             bmax = 15.;
    xmin = 1/(4294967296.*4294967296.); xmax = 1.;
    count = 99999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for ( a = amin; a<=amax; a*=1.0625)
      for ( b = bmin; b<=bmax; b*=1.0625 )
        for ( x = xmin; x<=xmax; x*=1.25 )
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==100000);
      if (count == 100000) count = 0;
    }
    // int values
    amin = 1;           amax = 1000.;
    bmin = 1;           bmax = 1000.;
    xmin = 1/(4294967296.*4294967296.); xmax = 1.;
    count = 999999;
    // print header
    printBetaRatio(amin, amax, bmin, bmax, xmin, xmax);
    // test values
    for ( a = amin; a<=amax; a+=1)
      for ( b = bmin; b<=bmax; b+=1)
        for ( x = xmin; x<=xmax; x*=1.25 )
    {
      count++;
      evalBetaRatio(a, b, x, Emax, count==1000000);
      if (count == 1000000) count = 0;
    }
  }
  catch (Exception & error)
  {
    std::cerr << _T("An error occured : ") << error.error() << _T("\n");
    return;
  }
  stk_cout << _T("\n");
  stk_cout << _T("+ Test Funct BetaRatio succesfull.                 +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return;
}

// Main
int main(int argc, char *argv[])
{
  Real Emax = 1e-13;
  try
  {
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Analysis.                            +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

    Char td;
    while(1)
    {
      stk_cout << _T("Test Functions:\n");
      stk_cout << _T(" 1 Const\n");
      stk_cout << _T(" 2 Poisson\n");
      stk_cout << _T(" 3 Gamma\n");
      stk_cout << _T(" 4 GammaRatio\n");
      stk_cout << _T(" 5 BetaRatio\n");
      stk_cout << _T(" 0 Exit\n");
      stk_cout << _T("Selection? ");
      stk_cin >> td;
      switch (td)
      {
        case _T('1'): testConst(); break;
        case _T('2'): testFunctPoisson(Emax); break;
        case _T('3'): testFunctGamma(Emax); break;
        case _T('4'): testFunctGammaRatio(Emax); break;
        case _T('5'): testFunctBetaRatio(Emax); break;
        default: break;
      }
      if (td==_T('0')) break;
    }
    stk_cout << _T("\n\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for Analysis  +\n");
    stk_cout << _T("+               STK Analysis                     +\n");
    stk_cout << _T("+ No errors detected.                            +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << "\n";
  }
  return 0;
}

