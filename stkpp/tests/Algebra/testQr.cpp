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

/** @file testQr.cpp
 *  @brief In this file we test the Qr class.
 **/

//  Matrix QtQ, Qt;
//  transpose(qr3.Q(), Qt);
//  QtQ.resize(qr3.Q().cols(), qr3.Q().cols());
//  mult(QtQ, Qt, qr3.Q());
//  stk_cout << _T(" Q'Q= \n");
//  stk_cout << QtQ << _T("\n" ;
//
//  QtQ.resize(qr3.Q().rows(), qr3.Q().rows());
//  mult(QtQ, qr3.Q(), Qt);
//  stk_cout << _T(" QQ'= \n");
//  stk_cout << QtQ << _T("\n" ;

#include "../../include/STKpp.h"

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
  stk_cout << name << _T(".p_data() =") << A.allocator().p_data()  << _T("\n");
  stk_cout << name << _T("=\n")   << A << _T("\n\n");
}


void writeResult( const Qr& q, bool verbose = false)
{
  if (verbose)
  {
    // write result
    print(q.Q(), "Q");
    print(q.R(), "R");
  }
  // compute QR
  Matrix QxR;
  //q.compQ();
  if (q.isCompQ())
  { // Q is formed, we can compute QR directly
    QxR.move(mult(q.Q(), q.R()));
  }
  else
  { // apply leftHousolder transformation to R
    QxR = q.R();
    leftHouseholder(QxR, q.Q());
  }
  // write result
  print(QxR, "QxR");
}

/* test_qr. */
void test_qr(int M, int N)
{
  int i,j,k;
  int size = std::min(M,N);

  stk_cout << _T("Test qr with:\n");
  stk_cout << _T("M = ") << M << _T(", N = ") << N << _T("\n\n");
  
  stk_cout << _T("=========================\n");
  stk_cout << _T("Test1: qr1(A) with A(N,M)\n");
  Matrix A(Range(1,N),Range(1,M), 0.0);
  for (i=1; i<N; i++) { A(i,i+1) = 2; A(i,i) = 1.0;}
  A(2,2) = 0.0;
  print(A, _T("A"));
  Qr qr1(A);
  writeResult(qr1);
     
  stk_cout << _T("==========================\n");
  stk_cout << _T("Test2: qr2(A) with A(M,N).\n");
  A.resize(Range(1,M),Range(1,N));
  A = 0.0;
  for (i=1; i<N; i++) { A(i,i+1) = 2; A(i,i) = 1.0;}
  A(2,2) = 0.0;
  print(A, _T("A"));
  Qr qr2(A);
  writeResult(qr2);

  // Third test
  stk_cout << _T("==========================\n");
  stk_cout << _T("Test3: qr3(A) with A(M,N).\n");
  for (i=M, k=1; i>=1; i--)
    for (j=1; j<=N; j++)
    { A(i,j) = M*N-k++;}
  for (i=1; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(2,2) = 0.0; //A(1, N) = 10.;
  print(A, _T("A"));
  Qr qr3(A);
  writeResult(qr3);

  stk_cout << _T("---------------\n");
  stk_cout << _T("qr3.eraseCol(3)\n");
  qr3.eraseCol(3);
  writeResult(qr3);

  stk_cout << _T("-----------------------------------------------\n");
  stk_cout << _T("T3 = A.col(A.lastIdxCols()); qr3.pushBackCol(T3);\n");
  Vector T3(A.col(A.lastIdxCols()));
  stk_cout << _T("T3 =") << T3 << _T("\n");
  qr3.pushBackCol(T3);
  writeResult(qr3);

  stk_cout << _T("----------------------------------------------------\n");
  stk_cout << _T("T3 = 1.0; qr3.insertCol(T3, qr3.R().lastIdxCols() );\n");
  T3 = 1.0;
  stk_cout << _T("T3 =") << T3 << _T("\n");
  qr3.insertCol(T3, qr3.R().lastIdxCols() );
  writeResult(qr3);

  stk_cout << _T("==========================\n");
  stk_cout << _T("Test4: qr4(A) with A(N,M).\n");
  A.resize(Range(1,N),Range(1,M));
  for (i=N, k=1; i>=1; i--)
    for (j=1; j<=M; j++)
    { A(i,j) = M*N-k++;}
  for (i=1; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(2,2) = 0.0;
  print(A, _T("A"));
  Qr qr4(A);
  writeResult(qr4);

  stk_cout << _T("------------------\n");
  stk_cout << _T("qr4.popBackCols();\n");
  qr4.popBackCols();
  writeResult(qr4);

  stk_cout << _T("----------------\n");
  stk_cout << _T("qr4.eraseCol(3);\n");
  qr4.eraseCol(3);
  writeResult(qr4);

  stk_cout << _T("----------------------------------------------\n");
  stk_cout << _T("Vector T4(A.rows(), 1.0); qr4.pushBackCol(T4);\n");
  Vector T4(A.rows(), 1.0);
  stk_cout << _T("T4 =") << T4 << _T("\n");
  qr4.pushBackCol(T4);
  writeResult(qr4);

  stk_cout << _T("------------------------------------\n");
  stk_cout << _T("T4 = A.col(3); qr4.insertCol(T4, 3);\n");
  T4 = A.col(3);
  stk_cout << _T("T4 =") << T4 << _T("\n");
  qr4.insertCol(T4, 3);
  writeResult(qr4);
}

// Main
int main(int argc, char *argv[])
{ 
  int M, N;
  if (argc < 3)
  {
    stk_cout << _T("Usage: M, N\n");
    stk_cout << _T("Setting: M=16 and N=10\n");
    M = 16;
    N =10;
  }
  else
  {
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    if (M<=N)
    {
      stk_cout << _T("Usage: M > N\n");
      exit(1);
    }

  }

  try
  {
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++""\n");
    stk_cout << _T("+ Test STK::Algebra::Qr                               +""\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++""\n");
    stk_cout << _T("\n\n");

    test_qr(M,N);

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
