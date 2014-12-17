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

/** @file testQrShift.cpp
 *  @brief In this file we test the Qr class when the arrays are shifted.
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


template<class Derived>
void writeResult( IQr<Derived> const& q, bool verbose = false)
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
  Vector T3;
  ArrayXX A;

  stk_cout << _T("==============================\n");
  stk_cout << _T("Test3: qr1(A) with A(1:M,1:N).\n");
  A.resize(M, N);
  A.shift(1, 1);
  for (i=A.lastIdxRows(), k=1; i>=A.beginRows(); i--)
    for (j=A.beginCols(); j<=A.lastIdxCols(); j++)
    { A(i,j) = M*N-k++;}
  for (i=A.beginCols(); i<A.lastIdxCols(); i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;
  print(A, _T("A"));
  Qr qr1(A);
  qr1.run();

  stk_cout << _T("--------------\n");
  stk_cout << _T("qr.eraseCol(3)\n");
  qr1.eraseCol(3);
  writeResult(qr1, verbose);

  stk_cout << _T("==============================\n");
  stk_cout << _T("Test3: qr0(A) with A(0:M-1,0:N-1).\n");
  A.resize(M, N);
  A.shift(0, 0);
  for (i=A.lastIdxRows(), k=1; i>=A.beginRows(); i--)
    for (j=A.beginCols(); j<=A.lastIdxCols(); j++)
    { A(i,j) = M*N-k++;}
  for (i=A.beginCols(); i<A.lastIdxCols(); i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;

  print(A, _T("A"));
  Qr qr0(A);
  qr0.run();

  stk_cout << _T("--------------\n");
  stk_cout << _T("qr0.eraseCol(3)\n");
  qr0.eraseCol(3);
  writeResult(qr0, verbose);
}

/* test_qr. */
void test_lapack_qr(int M, int N, bool verbose = false)
{
  int i,j,k;
  Vector T3;
  ArrayXX A;

  stk_cout << _T("==============================\n");
  stk_cout << _T("Test3: qr1(A) with A(1:M,1:N).\n");
  A.resize(M, N);
  A.shift(1, 1);
  for (i=A.lastIdxRows(), k=1; i>=A.beginRows(); i--)
    for (j=A.beginCols(); j<=A.lastIdxCols(); j++)
    { A(i,j) = M*N-k++;}
  for (i=A.beginCols(); i<A.lastIdxCols(); i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;
  print(A, _T("A"));
  lapack::Qr qr1(A);
  qr1.run();

  stk_cout << _T("--------------\n");
  stk_cout << _T("qr.eraseCol(3)\n");
  qr1.eraseCol(3);
  writeResult(qr1, verbose);

  stk_cout << _T("==============================\n");
  stk_cout << _T("Test3: qr0(A) with A(0:M-1,0:N-1).\n");
  A.resize(M, N);
  A.shift(0, 0);
  for (i=A.lastIdxRows(), k=1; i>=A.beginRows(); i--)
    for (j=A.beginCols(); j<=A.lastIdxCols(); j++)
    { A(i,j) = M*N-k++;}
  for (i=A.beginCols(); i<A.lastIdxCols(); i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  A(A.beginRows()+1,A.beginCols()+1) = 0.0;

  print(A, _T("A"));
  lapack::Qr qr0(A);
  qr0.run();

  stk_cout << _T("--------------\n");
  stk_cout << _T("qr0.eraseCol(3)\n");
  qr0.eraseCol(3);
  writeResult(qr0, verbose);
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
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Algebra::Qr                               +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

    test_qr(M,N, verbose);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Algebra::lapack::Qr                       +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
    test_lapack_qr(M,N, verbose);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::Algebra Qr  +\n");
    stk_cout << _T("+ No errors detected.                                   +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << _T("\n";);
  }
  return 0;

}
