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
 * Purpose:  test program for testing EigenvaluesSymmetric class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testLapackSymmEigen.cpp
 *  @brief In this file we test the EigenvaluesSymmetric class.
 **/

#include "../../include/Algebra.h"

using namespace STK;
using namespace STK::lapack;

template <typename Type>
struct numberingVisitor
{
  Type value_;
  numberingVisitor() : value_(1) {};
  inline void operator() ( Type& value)
  { value = value_; ++value_;}
};

template<typename Container2D>
void numbering(ArrayBase<Container2D>& matrix)
{
  typedef typename hidden::Traits<Container2D>::Type Type;
  numberingVisitor<Type> visitor;
  ArrayBaseVisitor<Container2D, numberingVisitor<Type> > arrayVisitor(matrix.asDerived());
  arrayVisitor.apply(visitor);
}

template<typename Container2D>
void symmetrize(ArrayBase<Container2D>& matrix)
{
  for (int i=matrix.firstIdx(); i<=matrix.lastIdx(); i++)
    for (int j=i; j<=matrix.lastIdx(); j++)
    { matrix(i,j) = (matrix(i,j) + matrix(i,j))/2.;
      matrix(j,i) = matrix(i,j);
    }
}

/* print array with its name. */
template< class Container2D >
void print(ArrayBase<Container2D> const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n");
  stk_cout << name << _T("=\n")   << A.asDerived() << _T("\n\n");
}


void writeResult( SymEigen const& s)
{
  stk_cout << "s.rank()=" << s.rank()<< _T("\n");
  stk_cout << "s.det()=" << s.det()<< _T("\n");
  stk_cout << "s.norm()=" << s.norm()<< _T("\n");
  // print arrays
  print(s.eigenValues(), _T("eigenValues"));
  print(s.eigenVectors(), _T("eigenVectors"));
  //
  Array2DSquare<Real> Res1 ;
  Array2DDiagonal<Real> D ;
  D = s.eigenValues();

  // P'P
  Res1 = s.eigenVectors().transpose() * s.eigenVectors();
  print(Res1, _T("P'P"));
  // PDP'
  Res1 = s.eigenVectors()* D * s.eigenVectors().transpose() ;
  print(Res1, _T("PDP'"));
}


/* test SymmEigen*/
bool testSymmEigen(int N)
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
  SymEigen* eigen = new SymEigen(A);
  if (!eigen->run()) { delete eigen; return false;}
  writeResult(*eigen);
  delete eigen;

  // ---------------- Second test --------------------
  numbering(A);
  symmetrize(A);
  if (A.lastIdx()>=2) A(2,2) = 0.0;

  stk_cout << _T("+++++++++++++\n");
  stk_cout << _T("+Second test+\n");
  stk_cout << _T("+++++++++++++\n");
  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");

  eigen = new SymEigen(A);
  if (!eigen->run()) { delete eigen; return false;}
  writeResult(*eigen);
  delete eigen;

  // ---------------- Third test --------------------
  A.col(A.firstIdx()) = 0.0; A.row(A.firstIdx()) = 0.0;
  if (A.lastIdx()>=2) A(2,2) = 0.0;
  
  stk_cout << _T("+++++++++++++++++++++++++\n");
  stk_cout << _T("+Third test : A singular+\n");
  stk_cout << _T("+++++++++++++++++++++++++\n");
  stk_cout << _T("A = \n");
  stk_cout << A << _T("\n");
  print(A, _T("A"));
  eigen = new SymEigen(A);
  if (!eigen->run()) { delete eigen; return false;}
  writeResult(*eigen);
  delete eigen;
  return true;
}

// Main
int main(int argc, char *argv[])
{
  int N;

  if (argc < 2)
  { N=8;}
  else
  { N = atoi(argv[1]);}

  try
  {
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::lapack::SymmEigen                         +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T(" N = ") << N;

    stk_cout << _T("\n\n");

    if (!testSymmEigen(N)) return -1;

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
