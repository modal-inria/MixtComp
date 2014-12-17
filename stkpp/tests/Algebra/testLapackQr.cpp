/*--------------------------------------------------------------------*/
/*     Copyright (C) 2003-2007  Serge Iovleff

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
 * Purpose:  test program for testing Qr class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testLapackQr.cpp
 *  @brief In this file we test the lapack::Qr class.
 **/

#include "STKpp.h"
using namespace STK;

/* main print method. */
template< class Container2D >
void print(Container2D const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".capacityHo() =")   << A.capacityHo()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n\n");
  stk_cout << name << _T(".rangeCols().isRef() =")  << A.rangeCols().isRef() << _T("\n");
  stk_cout << name << _T(".rangeCols() =\n")  << A.rangeCols() << _T("\n");
  stk_cout << name << _T(".capacityCols().isRef() =") << A.capacityCols().isRef()  << _T("\n");
  stk_cout << name << _T(".capacityCols() =\n") << A.capacityCols()  << _T("\n");
  stk_cout << name << _T("=\n")   << A << _T("\n\n");
}


void writeResult( const lapack::Qr& q, bool verbose = false)
{
  if (verbose)
  {
    // write result
    print(q.Q(), "Q");
    print(q.R(), "R");
  }
  // compute QR
  ArrayXX QR;
  //q.compQ();
  stk_cout << _T("Q is computed ?") << q.isCompQ() << _T("\n");
  if (q.isCompQ())
  { // Q is formed, we can compute QR directly

    QR = q.Q() * q.R();
  }
  else
  { // apply leftHousolder transformation to R
    QR = q.R();
    leftArrayHouseholder(QR, q.Q());
  }
  // write result
  print(QR, "QR");
}

/* test_qr. */
void test_qr(int M, int N, bool verbose = false)
{
  int i,j,k;
  int size = std::min(M,N);
  Vector T3;

  stk_cout << _T("Test QR class with:\n");
  stk_cout << _T("M = ") << M << _T(", N = ") << N << _T("\n\n");
  
  stk_cout << _T("=========================\n");
  stk_cout << _T("Test1: qr1(A) with A(N,M)\n");
  ArrayXX A(N, M, 0.0);
  for (i=baseIdx; i<baseIdx+N; i++) { A(i,i+1) = 2; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;
  print(A, _T("A"));
  lapack::Qr qr1(A);
  qr1.run();
  writeResult(qr1, verbose);

  stk_cout << _T("---------------\n");
  stk_cout << _T("qr1.eraseCol(3)\n");
  qr1.eraseCol(3);
  writeResult(qr1, verbose);

  stk_cout << _T("-----------------------------------------------\n");
  stk_cout << _T("T3 = A.col(3); qr3.pushBackCol(T3);\n");
  T3 = A.col(3);
  stk_cout << _T("T3 =") << T3 << _T("\n");
  qr1.pushBackCol(T3);
  writeResult(qr1, verbose);

  stk_cout << _T("----------------------------------------------------\n");
  stk_cout << _T("T3 = 1.0; qr3.insertCol(T3, qr3.R().lastIdxCols() );\n");
  T3 = 1.0;
  stk_cout << _T("T3 =") << T3 << _T("\n");
  qr1.insertCol(T3, qr1.R().lastIdxCols() );
  writeResult(qr1, verbose);

  stk_cout << _T("==========================\n");
  stk_cout << _T("Test2: qr2(A) with A(M,N).\n");
  A.resize(M, N);
  A = 0.0;
  for (i=baseIdx; i<A.lastIdxCols(); i++) { A(i,i+1) = 2; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;
  print(A, _T("A"));
  lapack::Qr qr2(A);
  qr2.run();
  writeResult(qr2, verbose);

  // Third test
  stk_cout << _T("==========================\n");
  stk_cout << _T("Test3: qr3(A) with A(M,N).\n");
  for (i=A.lastIdxRows(), k=1; i>=A.beginRows(); i--)
    for (j=A.beginCols(); j<=A.lastIdxCols(); j++)
    { A(i,j) = M*N-k++;}
  for (i=A.beginCols(); i<A.lastIdxCols(); i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;
  print(A, _T("A"));
  lapack::Qr qr3(A);
  qr3.run();
  writeResult(qr3, verbose);

  stk_cout << _T("---------------\n");
  stk_cout << _T("qr3.eraseCol(3)\n");
  qr3.eraseCol(3);
  writeResult(qr3, verbose);

  stk_cout << _T("-----------------------------------------------\n");
  stk_cout << _T("T3 = A.col(A.lastIdxCols()); qr3.pushBackCol(T3);\n");
  T3 = A.col(A.lastIdxCols());
  stk_cout << _T("T3 =") << T3 << _T("\n");
  qr3.pushBackCol(T3);
  writeResult(qr3, verbose);

  stk_cout << _T("----------------------------------------------------\n");
  stk_cout << _T("T3 = 1.0; qr3.insertCol(T3, qr3.R().lastIdxCols() );\n");
  T3 = 1.0;
  stk_cout << _T("T3 =") << T3 << _T("\n");
  qr3.insertCol(T3, qr3.R().lastIdxCols() );
  writeResult(qr3, verbose);

  stk_cout << _T("==========================\n");
  stk_cout << _T("Test4: qr4(A) with A(N,M).\n");
  A.resize(N, M);
  for (i=A.lastIdxRows(), k= baseIdx; i>=A.beginRows(); i--)
    for (j=A.beginCols(); j<=A.lastIdxCols(); j++)
    { A(i,j) = M*N-k++;}
  for (i=1; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;
  print(A, _T("A"));
  lapack::Qr qr4(A);
  qr4.run();
  writeResult(qr4, verbose);

  stk_cout << _T("------------------\n");
  stk_cout << _T("qr4.popBackCols();\n");
  qr4.popBackCols();
  writeResult(qr4, verbose);

  stk_cout << _T("----------------\n");
  stk_cout << _T("qr4.eraseCol(3);\n");
  qr4.eraseCol(3);
  writeResult(qr4, verbose);

  stk_cout << _T("----------------------------------------------\n");
  stk_cout << _T("Vector T4(A.rows(), 1.0); qr4.pushBackCol(T4);\n");
  Vector T4(A.rows(), 1.0);
  stk_cout << _T("T4 =") << T4 << _T("\n");
  qr4.pushBackCol(T4);
  writeResult(qr4, verbose);

  stk_cout << _T("------------------------------------\n");
  stk_cout << _T("T4 = A.col(3); qr4.insertCol(T4, 3);\n");
  T4 = A.col(3);
  stk_cout << _T("T4 =") << T4 << _T("\n");
  qr4.insertCol(T4, 3);
  writeResult(qr4, verbose);
}

// Main
int main(int argc, char *argv[])
{ 
  int M=16, N =10;
  bool verbose = false;
  stk_cout << _T("Setting: M=16 and N=10\n");
  if (argc >= 2) { verbose = atoi(argv[1]);}

  try
  {
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++""\n");
    stk_cout << _T("+ Test STK::Algebra::Qr                               +""\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++""\n");
    stk_cout << _T("\n\n");

#ifdef STKUSELAPACK
    test_qr(M,N, verbose);
#else
    stk_cout << _T("Warning STK++ has been compiled without lapack\n");
#endif

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++""\n");
    stk_cout << _T("+ Successful completion of testing for STK::Algebra::Qr +""\n");
    stk_cout << _T("+ No errors detected.                                   +""\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++""\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << _T("\n";);
  }
  return 0;

}
