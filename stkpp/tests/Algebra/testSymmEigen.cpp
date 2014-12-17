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
 * Project:  Algebra
 * Purpose:  test program for testing SymEigen class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testSymmEigen.cpp
 *  @brief In this file we test the SymEigen class.
 **/

#include "STKpp.h"

using namespace STK;

/* main print method. */
template< class Container2D >
void print(Container2D const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n");
  stk_cout << name << _T(".p_data() =") << A.allocator().p_data()  << _T("\n");
  stk_cout << name << _T("=\n")   << A << _T("\n\n");
}


void writeResult( SymEigen const& s)
{
  stk_cout << "s.rank()=" << s.rank()<< _T("\n");
  stk_cout << "s.det()=" << s.det()<< _T("\n");
  stk_cout << "s.norm()=" << s.norm()<< _T("\n");
  // D
  print(s.eigenValues(), _T("D"));
  // P
  print(s.rotation(), _T("P"));
  // PP'
  ArraySquareX R;
  R = s.rotation().transpose() * s.rotation();
  print(R, _T("P'P"));
  // PP'
  R =  s.rotation() * s.rotation().transpose();
  print(R, _T("PP'"));
  // PDP'
  R =  s.rotation() * s.eigenValues().diag() * s.rotation().transpose();
  print(R, _T("PDP'"));
}


/* test_svd.                                                                                                                                */
void test_diagsym(int N)
{
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test SymEigen                         +\n");
  stk_cout << _T("\n\n");
  if (N<2)
  {
    stk_cout << _T("N =") << N << _T("\n");
    stk_cout << _T("Nothing to do\n");
    stk_cout << _T("\n\n");
    return;
  }
  ArraySquareX A(N), id2;

  stk_cout << _T("+++++++++++++\n");
  stk_cout << _T("+ First test+\n");
  stk_cout << _T("+++++++++++++\n");
  A = 0.0;
  for (int i=A.begin(); i<=A.lastIdx(); i++) { A(i,i)  = 1.0;}
  for (int i=A.begin(); i<A.lastIdx(); i++) { A(i,i+1) = 2.0; A(i+1, i) = 2.0;}
  if (A.range().isContaining(2)) A(2,2) = 0.0;
  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");
  // run test
  SymEigen* eigen = new SymEigen(A);
  eigen->run();
  writeResult(*eigen);
  delete eigen;

  stk_cout << _T("+++++++++++++\n");
  stk_cout << _T("+Second test+\n");
  stk_cout << _T("+++++++++++++\n");
  for (int i=A.begin(), k=1; i<=A.lastIdx(); i++)
    for (int j=i; j<=A.lastIdx(); j++)
    { A(i,j) = N*N-k++; A(j,i) = A(i,j);}

  for (int i=A.begin(); i<=A.lastIdx(); i++) { A(i,i) = 1.0;}
  for (int i=A.begin(); i<A.lastIdx(); i++) { A(i,i+1) = 2.0; A(i+1, i) = 2.0;}
  if (A.range().isContaining(2)) A(2,2) = 0.0;

  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");

  eigen = new SymEigen(A);
  eigen->run();
  writeResult(*eigen);
  delete eigen;

  stk_cout << _T("+++++++++++++++++++++++++\n");
  stk_cout << _T("+Third test : A singular+\n");
  stk_cout << _T("+++++++++++++++++++++++++\n");
  for (int i=A.begin(), k=1; i<=A.lastIdx(); i++)
    for (int j=i; j<=A.lastIdx(); j++)
    { A(i,j) = N*N-k++;
      A(j,i) = A(i,j);
    }
  for (int i=A.begin(); i<=A.lastIdx(); i++) { A(i,i) = 1.0;}
  for (int i=A.begin(); i<A.lastIdx(); i++) { A(i,i+1) = 2.0; A(i+1, i) = 2.0;}
  if (A.range().isContaining(2)) A(2,2) = 0.0;
  A.col(A.begin()+1) = 0.0;
  A.row(A.begin()+1) = 0.0;

  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");
  print(A, _T("A"));

  eigen = new SymEigen(A);
  eigen->run();
  writeResult(*eigen);
  delete eigen;

  // ---------------- Fourth test --------------------
  stk_cout << _T("+++++++++++++++++++++++++++\n");
  stk_cout << _T("+Fourth test : A.shift(-1)+\n");
  stk_cout << _T("+++++++++++++++++++++++++++\n");
  A.shift(-1);
  print(A, _T("A"));
  eigen = new SymEigen(A);
  eigen->run();
  writeResult(*eigen);
  delete eigen;
}

// Main
int main(int argc, char *argv[])
{
  int N;

  if (argc < 2)
  {
    N=8;
  }
  else
  {
    N = atoi(argv[1]);
  }

  try
  {
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::SymmEigen                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T(" N = ") << N;

    stk_cout << _T("\n\n");

    test_diagsym(N); 

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::SymmEigen   +\n");
    stk_cout << _T("+ No errors detected.                                   +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << _T("An error occured : ") << error.error() << _T("\n");
  }
  return 0;
}
