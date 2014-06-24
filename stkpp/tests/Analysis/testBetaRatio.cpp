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

#include "Analysis.h"
#include "STatistiK.h"
#include "Arrays.h"
#include "STKernel.h"

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
    bool lower_tail = true;
    if (argc >=4)
    {
      a = atof(argv[1]);
      b = atof(argv[2]);
      x = atof(argv[3]);
      if (argc >=5) lower_tail = atoi(argv[4]);
    }
    else
    {
      Law::Uniform lawb(0, 10000);
      a = lawb.rand();
      b = lawb.rand();
      x = std::abs(Law::Normal::rand(a/(a+b), 0.001));
    }
    // write results
    std::cout << "a = " << a << "; b = " << b << "; x = " << x << std::endl;

    Real res = Funct::betaRatio(a,b,x,lower_tail);
    stk_cout << _T("Res =") << res << _T("\n");

    Real res_se = Funct::betaRatio_se(a-1,b-1,x, false, true);
    stk_cout << _T("Res_se =") << res_se << _T("\n");

    Real res_ae;
    res_ae = (a<b) ? Funct::betaRatio_ae(a, b, x, false, true)
                   : Funct::betaRatio_ae(b, b, x, true, false);
    stk_cout << _T("Res_ae =") << res_ae << _T("\n");

    Real res2 = pbeta(x, a, b, lower_tail, false);
    stk_cout << _T("Res_R =") << res2 << _T("\n");

    stk_cout << _T("diff =") << std::abs(res2-res) << _T("\n");
    stk_cout << _T("diff se=") << std::abs(res_se-res2) << _T("\n");
    stk_cout << _T("diff ae=") << std::abs(res_ae-res2) << _T("\n");
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



