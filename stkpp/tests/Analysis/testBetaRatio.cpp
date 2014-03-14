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

int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test betaRatio function                        +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
    std::string rawInput;
    Real a, b, x;
    if (argc >=4)
    {
      a = atof(argv[1]);
      b = atof(argv[2]);
      x = atof(argv[3]);
    }
    else
    {
      Law::Uniform lawb(0, 10000);
      a = lawb.rand();
      b = lawb.rand();
      x = std::abs(Law::Normal::rand(a/(a+b), 0.001)),1.;
    }
    // write results
    std::cout << "a = " << a << "; b = " << b << "; x = " << x << std::endl;
    Real res = Funct::betaRatio(a,b,x,true);
    stk_cout << _T("Res =") << res << _T("\n");

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



