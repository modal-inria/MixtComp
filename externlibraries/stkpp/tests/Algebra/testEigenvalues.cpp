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
 * Purpose:  test program for testing EigenvaluesSymmetric class.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 */

/** @file testEigenvaluesSymmetric.cpp
 *  @brief In this file we test the EigenvaluesSymmetric class.
 **/

#include "../../include/STKpp.h"

using namespace STK;

/* main print method. */
template< class Container2D >
void print(Container2D const& A, String const& name)
{
  stk_cout << "print: " << name << "\n";
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".capacityHo() =")   << A.capacityHo()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n");
  stk_cout << name << _T(".rangeCols().isRef() =")  << A.rangeCols().isRef() << _T("\n");
  stk_cout << name << _T(".rangeCols() =")  << A.rangeCols();
  stk_cout << name << _T(".capacityCols().isRef() =") << A.capacityCols().isRef()  << _T("\n");
  stk_cout << name << _T(".capacityCols() =") << A.capacityCols();
  stk_cout << name << _T(".p_data() =") << A.allocator().p_data()  << _T("\n");
  stk_cout << name << _T("=\n")   << A << _T("\n\n");
}


void writeResult( EigenvaluesSymmetric const& s)
{
  stk_cout << "s.rank()=" << s.rank()<< _T("\n");
  stk_cout << "s.det()=" << s.det()<< _T("\n");
  stk_cout << "s.norm()=" << s.norm()<< _T("\n");
  Range range(s.firstIdx(), s.lastIdx(), 0);
  // D
  print(s.eigenvalues(), _T("D"));
  // P
  print(s.rotation(), _T("P"));
  // P'
  MatrixSquare Pt;
  transpose(s.rotation(), Pt);
  // PP'
  MatrixSquare Res1;
  Res1.move(mult(Pt, s.rotation()));
  print(Res1, _T("P'P"));
  // PP'
  Res1.move(mult(s.rotation(), Pt));
  print(Res1, _T("PP'"));
  // PDP'
  Res1.move(mult(s.rotation(), mult(s.eigenvalues(), Pt)));
  print(Res1, _T("PDP'"));
}


/* test_svd.                                                                                                                                */
void test_diagsym(int N)
{
  MatrixSquare A(N), id2;

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test EigenvaluesSymmetric                         +\n");
  stk_cout << _T("\n\n");
  // ---------------- First test --------------------
  A = 0.0;
  for (int i=A.firstIdx(); i<=A.lastIdx(); i++) { A(i,i)  = 1.0;}
  for (int i=A.firstIdx(); i<A.lastIdx(); i++) { A(i,i+1) = 2.0; A(i+1, i) = 2.0;}
  if (A.lastIdx()>=2) A(2,2) = 0.0;

  stk_cout << _T("++++++++++++\n");
  stk_cout << _T("+First test+\n");
  stk_cout << _T("++++++++++++\n");
  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");
  // run test
  EigenvaluesSymmetric* eigen = new EigenvaluesSymmetric(&A);
  eigen->run();
  writeResult(*eigen);

  //  ginv test
  MatrixSquare* Ainv = eigen->ginv();
  stk_cout << _T("ginv() Done\n");
  stk_cout << _T("A^-1 =\n");
  stk_cout << (*Ainv) << _T("\n");

  id2.move(mult(*Ainv, A));
  print(id2,_T("A^-1*A"));

  delete Ainv;
  delete eigen;

  // ---------------- Second test --------------------
  for (int i=A.firstIdx(), k=1; i<=A.lastIdx(); i++)
    for (int j=i; j<=A.lastIdx(); j++)
    { A(i,j) = N*N-k++; A(j,i) = A(i,j);}

  for (int i=A.firstIdx(); i<=A.lastIdx(); i++) { A(i,i) = 1.0;}
  for (int i=A.firstIdx(); i<A.lastIdx(); i++) { A(i,i+1) = 2.0; A(i+1, i) = 2.0;}
  if (A.lastIdx()>=2) A(2,2) = 0.0;

  stk_cout << _T("+++++++++++++\n");
  stk_cout << _T("+Second test+\n");
  stk_cout << _T("+++++++++++++\n");
  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");

  eigen = new EigenvaluesSymmetric(&A);
  eigen->run();
  writeResult(*eigen);

  //  ginv test
  Ainv = eigen->ginv();
  stk_cout << _T("ginv() Done\n");
  print((*Ainv), _T("A^-1"));

  id2.move(mult(*Ainv, A));
  print(id2, _T("A^-1*A"));

  delete Ainv;
  delete eigen;

  // ---------------- Third test --------------------
  for (int i=A.firstIdx(), k=1; i<=A.lastIdx(); i++)
    for (int j=i; j<=A.lastIdx(); j++)
    { A(i,j) = N*N-k++;
      A(j,i) = A(i,j);
    }
  for (int i=A.firstIdx(); i<=A.lastIdx(); i++) { A(i,i) = 1.0;}
  for (int i=A.firstIdx(); i<A.lastIdx(); i++) { A(i,i+1) = 2.0; A(i+1, i) = 2.0;}
  if (A.lastIdx()>=2) A(2,2) = 0.0;
  A.col(A.firstIdx()) = 0.0; A.row(A.firstIdx()+1) = 0.0;
  
  stk_cout << _T("+++++++++++++++++++++++++\n");
  stk_cout << _T("+Third test : A singular+\n");
  stk_cout << _T("+++++++++++++++++++++++++\n");
  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");
  print(A, _T("A"));

  eigen = new EigenvaluesSymmetric(&A);
  eigen->run();
  writeResult(*eigen);

  // test ginv
  Ainv = eigen->ginv();
  stk_cout << _T("eigen->ginv() Done\n");
  //
  print(*Ainv, _T("A^-1"));
  // A^-1 * A
  id2.move(mult(A, *Ainv));
  print(id2, _T("A*A^-1"));
  id2.move(mult(*Ainv, A));
  print(id2, _T("A^-1*A"));
  delete Ainv;
  delete eigen;

  // ---------------- Fourth test --------------------
  stk_cout << _T("+++++++++++++++++++++++++++\n");
  stk_cout << _T("+Fourth test : A.shift(-1)+\n");
  stk_cout << _T("+++++++++++++++++++++++++++\n");
  A.shift(-1);
  print(A, _T("A"));
  eigen = new EigenvaluesSymmetric(&A);
  eigen->run();
  writeResult(*eigen);

  // test ginv
  Ainv = eigen->ginv();
  stk_cout << _T("Test eigen->ginv() Done\n");
  print(*Ainv, _T("A^-1"));

  // A^-1 * A
  id2.move(mult(A, *Ainv));
  print(id2, _T("A*A^-1"));

  id2.move(mult(*Ainv, A));
  print(id2, _T("A^-1*A"));
  delete Ainv;
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
    stk_cout << _T("+ Test STK::EigneValues                               +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T(" N = ") << N;

    stk_cout << _T("\n\n");

    test_diagsym(N); 

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::EigneValues +\n");
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
