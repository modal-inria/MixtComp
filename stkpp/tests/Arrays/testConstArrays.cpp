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
 * Project: stkpp::Arrays
 * Purpose:  test program for testing Arrays classes.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testConstArrays.cpp
 *  @brief In this file we test the Const arrays classes.
 **/

#include "STKernel.h"
#include "Arrays.h"

using namespace STK;

/* main print. */
template<class CArray >
void printHead(CArray const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")    << A.isRef()  << _T("\n");
  stk_cout << name << _T(".rows() =")   << A.rows() << _T("\n");
  stk_cout << name << _T(".cols() =")   << A.cols() << _T("\n");
  stk_cout << name << _T(".sizeRows() =")   << A.sizeRows() << _T("\n");
  stk_cout << name << _T(".sizeCols() =")   << A.sizeCols() << _T("\n");
  stk_cout << name << _T("=\n");
}

/* main print.*/
template<class CArray >
void print(CArray const& A, String const& name)
{
  printHead(A, name);
  stk_cout << A;
}

template <typename Lhs, typename Rhs>
Real MultByConstantArray(Lhs const& lhs, Rhs const& rhs)
{ return ((lhs * rhs)- lhs).abs().maxElt();}
template <typename Lhs, typename Rhs>
Real Add2Identity(Lhs const& lhs, Rhs const& rhs)
{ return ((lhs + 2 * rhs)- lhs).abs().maxElt();}
template <typename Lhs, typename Rhs>
Real diff2Identity(Lhs const& lhs, Rhs const& rhs)
{ return ((lhs - 2 * rhs)- lhs).abs().maxElt();}
template <typename Lhs, typename Rhs>
Real div2Identity(Lhs const& lhs, Rhs const& rhs)
{ return ((lhs / (2 * rhs))- lhs).abs().maxElt();}

/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test all constant arrays                            +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  CArray<Real, 3, 4> A;
  CArraySquare<Real, 4> S;
  A << 1, 2, 3, 4
     , 2, 1, 3, 4
     , 2, 3, 1, 4;
  S << 1, 2, 3, 4
     , 2, 1, 3, 4
     , 2, 3, 1, 4
     , 2, 3, 4, 1;

  Const::Identity<Real, 4> I4;
  CArray<Real> Res;
  stk_cout << _T("(A*I4 - A).maxElt() = ") << MultByConstantArray(A, I4) << _T("\n");
  stk_cout << _T("S+2*I4 - S =\n") << (Res=(S + 2 * I4) - S) << _T("\n");
  stk_cout << _T("S-2*I4 - S =\n") << (Res=(S - 2 * I4) - S) << _T("\n");
  stk_cout << _T("S/(2*I4) - S =\n") << (Res=(S / (2 * I4)) - S) << _T("\n");

  Const::Square<Real, 4> S4;
  stk_cout << _T("(A*S4 - A).maxElt() = ") << MultByConstantArray(A, S4) << _T("\n");
  stk_cout << _T("S+2*S4 - A =\n") << (Res=(S + (2 * S4)) - S) << _T("\n");
  stk_cout << _T("S-2*S4 - A =\n") << (Res=(S - (2 * S4)) - S) << _T("\n");
  stk_cout << _T("S/(2*S4) - A =\n") << (Res=(S / (2 * S4)) - S) << _T("\n");

  Const::UpperTriangular<Real, 4, 4> U4;
  stk_cout << _T("(A*U4 - A).maxElt() = ") << MultByConstantArray(A, U4) << _T("\n");
  stk_cout << _T("S+2*U4 - A =\n") << (Res=(S + (2 * U4)) - S) << _T("\n");
  stk_cout << _T("S-2*U4 - A =\n") << (Res=(S - (2 * U4)) - S) << _T("\n");
  stk_cout << _T("S/(2*U4) - A =\n") << (Res=(S / (2 * U4)) - S) << _T("\n");

  Const::LowerTriangular<Real, 4, 4> L4;
  stk_cout << _T("(A*L4 - A).maxElt() = ") << MultByConstantArray(A, L4) << _T("\n");
  stk_cout << _T("S+2*L4 - A =\n") << (Res=(S + (2 * L4)) - S) << _T("\n");
  stk_cout << _T("S-2*L4 - A =\n") << (Res=(S - (2 * L4)) - S) << _T("\n");
  stk_cout << _T("S/(2*L4) - A =\n") << (Res=(S / (2 * L4)) - S) << _T("\n");


  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for constant arrays+\n");
  stk_cout << _T("+ No errors detected.                                 +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  return 0;
}
