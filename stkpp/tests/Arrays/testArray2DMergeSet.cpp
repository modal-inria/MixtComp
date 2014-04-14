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

/** @file testArray2D.cpp
 *  @brief In this file we test the TContainer 2D classes.
 **/

#include "../../include/STKernel.h"
#include "../../include/Arrays.h"

using namespace STK;

/** @ingroup Arrays
  * @brief allow to number to the element of an Arrays
  */
template <typename Type>
struct numberingVisitor
{
  Type value_;
  numberingVisitor() : value_(1) {};
  inline void operator() ( Type& value)
  { value = value_; ++value_;}
};

/** Utility function for numbering an array */
template<typename Derived>
void numbering(Derived& matrix)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  numberingVisitor<Type> visitor;
  ArrayBaseVisitor<Derived, numberingVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.apply(visitor);
}



/* main print */
template< class Type, template<class> class Array2D >
void printHead(Array2D<Type> const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T(" | ");
  stk_cout << name << _T(".firstIdxRows() =")   << A.firstIdxRows()  << _T(" ");
  stk_cout << name << _T(".lastIdxRows() =")      << A.lastIdxRows()  << _T(" ");
  stk_cout << name << _T(".sizeRows() =")      << A.sizeRows()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T(" | ");
  stk_cout << name << _T(".firstIdxCols() =")   << A.firstIdxCols()  << _T(" ");
  stk_cout << name << _T(".lastIdxCols() =")      << A.lastIdxCols()  << _T(" ");
  stk_cout << name << _T(".sizeCols() =")      << A.sizeCols()  << _T("\n");
  stk_cout << name << _T(".capacityHo() =")   << A.capacityHo()  << _T("\n");
  stk_cout << name << _T(".rangeCols().isRef() =")  << A.rangeCols().isRef() << _T("\n");
  stk_cout << name << _T(".rangeCols() =")          << A.rangeCols();
  stk_cout << name << _T(".capacityCols().isRef() =") << A.capacityCols().isRef()  << _T("\n");
  stk_cout << name << _T(".capacityCols() =") << A.capacityCols()  << _T("\n");
}

template< class Type, template<class> class Array2D >
void print(Array2D<Type> const& A, String const& name)
{
  printHead(A, name);
  stk_cout << name << _T("=\n") << A << _T("\n\n");
}

template< class Type>
void TestArray2DMergeSet( int M, int N, Range I, Range J, bool output)
{
//  //
  if (output) stk_cout << _T("Array2D<Type>  A(M,N); numbering(A);\n");
  Array2D<Type> A(M,N);
  numbering(A);
  if (output) { print(A, _T("A"));}

  //
  if (output) stk_cout << _T("Array2D<Type> D(A); D=2; A.merge(D); \n");
  Array2D<Type> D(A);
  D=Type(2);
  A.merge(D);
  if (output) { print(A, _T("A")); print(D, _T("D"));}

  //
  if (output) stk_cout << _T("Array1D<Type> V(A.rows(), Type(1)); A.merge(V); \n");
  Array1D<Type> V(A.rows(), Type(1));
  A.merge(V);
  if (output) { print(A, _T("A"));}
  if (output) { stk_cout << _T("V=") << V;}

  //
  if (output) stk_cout << _T("A.pushBackCols(V); \n");
  A.pushBackCols(V);
  if (output) { print(A, _T("A"));}

  //
  if (output) stk_cout << _T("Array2D<Type> C(A, true); A.pushBackRows(C); \n");
  Array2D<Type> C(A, true);
  A.pushBackRows(C);
  if (output) { print(A, _T("A"));}

  //
  if (output)
    stk_cout << _T("Array2D<Type> B; B.set(C); \n");
  Array2D<Type> B;
  B.pushBackRows(C);
  if (output) { print(B, _T("B"));}

  if (output)
    stk_cout << _T("Array1D<Type> W(A.rows(), Type(2)); Array2D<Type> E; E.pushBackCols(V); \n");
  Array1D<Type> W(A.rows(), Type(2));
  Array2D<Type> E;
  E.pushBackCols(W);
  if (output)   { print(E, _T("E"));}
}


/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  try
  {
    int M, N, Istart, Jstart, Iend, Jend, output, iter;
    if (argc < 7)
    {
      // 12 15 3 4 10 12 1 2
      M      = 11;
      N      = 12;
      Istart = 2;
      Jstart = 3;
      Iend   = 8;
      Jend   = 9;
      output = true;
      iter = 1;

    }
    else
    {
      // dimensions
      M      = atoi(argv[1]);
      N      = atoi(argv[2]);
      Istart = atoi(argv[3]);
      Jstart = atoi(argv[4]);
      Iend   = atoi(argv[5]);
      Jend   = atoi(argv[6]);
      // output
      output = true;
      if (argc >7 ) output = atoi(argv[7]);
      // number of test
      iter =1;
      if (argc >8 ) iter = atoi(argv[8]);
    }
    // ranges
    Range I(Istart, Iend, 0);
    Range J(Jstart, Jend, 0);
    
    for( int i=1; i<=iter; i++)
    {
      stk_cout << _T("\n\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("+ Test merge and set using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T(" TestArray2DMergeSet<Real> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      TestArray2DMergeSet<Real>(M, N, I, J, output);
      Real test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
      
//      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//      stk_cout << _T(" TestArray2DMergeSet<int> : \n");
//      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//      // time variables
//      Chrono::start();
//      TestArray2DMergeSet<int>(M, N, I, J, output);
//      test_time = Chrono::elapsed();
//      stk_cout << _T("\n");
//      stk_cout << _T("Time used : ") << test_time << _T("\n");
//      stk_cout << _T("\n\n");

      M *=2; N *=2; Istart *= 2; Jstart *= 2;  Iend   *= 2;  Jend   *= 2;
    }
  }
  catch (Exception const& error)
  {
    std::cerr << error.error() << _T("\n";);
  }

  return 0;
}
