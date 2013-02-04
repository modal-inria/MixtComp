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
 * Project:  Algebra
 * Purpose:  test program for testing Svd class.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 */

/** @file testSvd.h
 *  @brief In this file we test the Svd class.
 **/

#include "../../include/STKpp.h"


using namespace STK;

void writeResult(const Svd& s)
{
  // compute D
  Matrix D(s.ncolU(), s.nrowV(), 0.0);
  D=0.0;
  for (int i=1; i<=s.ncolD(); i++) {D(i,i)=s.getD()[i];}

  Matrix Res2;
  // UD
  Res2.move(mult(s.getU(),D));
  // v'
  MatrixSquare Vt;
  transpose(s.getV(), Vt);
  // UDV'
  Matrix Res;
  Res.move(mult(Res2, Vt));
  // write result
  stk_cout << _T(" U=\n");
  stk_cout << s.getU() << _T("\n");
  stk_cout << _T(" D=\n");
  stk_cout << D << _T("\n");
  stk_cout << _T(" V=\n");
  stk_cout << s.getV() << _T("\n");
  stk_cout << _T(" UDV'=\n");
  stk_cout << Res << _T("\n");
}

/* testing svd.                                                       */
void test_svd(int M, int N)
{
  int j,k;

  // Test svd
  Matrix A(N,M);
  int size = std::min(M,N);

  // First test
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test Svd                                          +\n");
  stk_cout << _T("\n\n");
  
  stk_cout << _T("preliminary test R0(T)\n");
  Matrix T(2, 2, 0.0);
  T(1,2) = 1.0;
  T(2,2) = 1.0;
  Svd R0(T);
  stk_cout << _T("Svd Done\n");
  writeResult(R0);
  stk_cout << _T("\n\n");
  
  stk_cout << _T("First test R1(A)\n");
  A = 0.0;
  for (int i=1; i<=size; i++) { A(i,i) = 1.0;}
  for (int i=1; i<size; i++) { A(i,i+1) = 2;}
  if (A.sizeCols()>size) A(size, size+1) = 2.0;
  if (size>=2) A(2,2) = 0.0;

  stk_cout << _T("A =\n") << A << _T("\n");
  Svd R1(A);
  writeResult(R1);

  // First test
  stk_cout << _T("First test (bis) R1.newSvd(A)\n");

  A.resize(M,N);
  A =0.0;
  for (int i=1; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  if (A.sizeCols()>size) A(size, size+1) = 2.0;
  if (size>=2) A(2,2) = 0.0;

  stk_cout << _T("A =\n") << A << _T("\n");
  R1.newSvd(A);
  writeResult(R1);

  // Second test
  stk_cout << _T("Second test :  R1.newSvd(A, true, true, true, N+3)\n");
  A.resize(M,N);
  for (int i=1, k=1; i<=M; i++)
    for (j=1; j<=N; j++)
    { A(i,j) = M*N-k++;}

  for (int i=1; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  if (A.sizeCols()>size) A(size, size+1) = 2.0;
  if (size>=2) A(2,2) = 0.0;

  stk_cout << _T("A =\n") << A << _T("\n");
  R1.newSvd(A, true, true);
  writeResult(R1);

  // Third test
  stk_cout << _T("Third test : Svd R3(A, false, true, true, M)\n");
  A.resize(N,M);
  for (int i=N, k=1; i>=1; i--)
    for (j=1; j<=M; j++)
      A(i,j) = M*N-k++;

  for (int i=1; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  if (A.sizeCols()>size) A(size, size+1) = 2.0;
  if (size>=2) A(2,2) = 0.0;

  stk_cout << _T("A =\n") << A << _T("\n");
  Svd R3(A, false, true, true);
  writeResult(R3);

  stk_cout << _T("Other test : R3 = R1\n");
  R3 = R1;

  stk_cout << _T("Other test : Svd  R4(R3)\n");
  Svd R4(R3);

  // Fourth test
  stk_cout << _T("Fourth test with A.resize(2*M,N)\n");

  A.resize(2*M,N);
  A = 0.0;
  for (j=2, k=0; j<=N; j++, k++)
    for (int i=1; i<=2*M; i++)
    { A(i,j) = k;}
  for (int i=2; i<size; i++) { A(i,i+1) = 2.0; A(i,i) = 1.0;}
  if (A.sizeCols()>size) A(size, size+1) = 2.0;
  if (size>=2) A(2,2) = 0.0;
  stk_cout << _T("A.capacityCols() =\n") << A.capacityCols()
            << _T("\n");
  stk_cout << _T("A.rangeCols() =\n") << A.rangeCols()
            << _T("\n\n");
  stk_cout << _T("A =\n") << A << _T("\n");

  R3.newSvd(A);

  writeResult(R3);

  // Fifth test
  stk_cout << _T("Fifth test (with pointer) : newSvd\n");
  Matrix* B = new Matrix(2*M, N, 0.0);
  for (j=2, k=0; j<=N; j++, k++)
    for (int i=1; i<=2*M; i++)
    { (*B)(i,j) = k;}

  for (int i=2; i<size; i++) { (*B)(i,i+1) = 2.0; (*B)(i,i) = 1.0;}
  if (size>=2) (*B)(2,2) = 0.0;

  stk_cout << _T("B =\n") << *B << _T("\n");
  R3.newSvd(*B);

  writeResult(R3);

  delete B;

// Fifth test
  stk_cout << _T("Fifth test 2(with pointer) : Svd(B)\n");
  B = new Matrix(2*M, N, 0.0);
  for (j=2, k=0; j<=N; j++, k++)
    for (int i=1; i<=2*M; i++)
    { (*B)(i,j) = k;}

  for (int i=2; i<size; i++) { (*B)(i,i+1) = 2.0; (*B)(i,i) = 1.0;}
  if (size>=2) (*B)(2,2) = 0.0;

  stk_cout << _T("B =\n") << *B << _T("\n");
  Svd R5(*B, true, true, true);

  writeResult(R5);

  delete B;

  stk_cout << _T("+ Test STK Svd Done.                                  +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");
}

// Main
int main(int argc, char *argv[])
{
  int M;
  int N;
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
  }
  try
  {
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK Algebra                                    +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("M = ") << M << _T(" N = ") << N;
    stk_cout << _T("\n\n");

    test_svd(M,N);
    //test_qr(M,N);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK Svd        +\n");
    stk_cout << _T("+ No errors detetected.                               +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << "\n";
  }
  return 0;
}
