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
 * Project: stkpp::Arrays
 * Purpose:  test program for testing Arrays classes.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 */

/** @file testCArrays.h
 *  @brief In this file we test the CArray* classes.
 **/

#include "../../include/STKernel.h"
#include "../../include/Arrays.h"


using namespace STK;

template <typename Type>
struct numberingVisitor
{
  Type value_;
  numberingVisitor() : value_(1) {};
  inline void operator() ( Type& value)
  { value = value_; ++value_;}
};


template<typename Derived>
void numbering(Derived& matrix)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  numberingVisitor<Type> visitor;
  ArrayBaseVisitor<Derived, numberingVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.visit(visitor);
}

/* main print. */
template<class CArray >
void printHead(CArray const& A, String const& name)
{
  stk_cout << "print: " << name << "\n";
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


template< class TYPE>
int Test( int M, int N, Range I, Range J, bool output)
{
    //
    stk_cout << _T("C0 = L, with L lower triangular.\n");
    CArray<TYPE, Arrays::general_, UnknownSize, UnknownSize, Arrays::by_col_> C0(M, N, 0);
    numbering(C0);
    Array2DLowerTriangular<TYPE> L(M-2, N-2); numbering(L);
    C0 = L;
    print(C0,_T("C0"));

    Array2DDiagonal<TYPE> C1(M, 1);
    numbering(C1);
    stk_cout << _T("Creating and numbering C1.\n");
    print(C1,_T("C1"));
    CArray<TYPE, Arrays::point_, 1, 13, Arrays::by_col_> C2(M);
    numbering(C2);
    stk_cout << _T("Creating and numbering C1.\n");
    print(C2,_T("C2"));
    CArray<TYPE, Arrays::vector_, 15, 1, Arrays::by_row_> C3(N);
    numbering(C3);
    stk_cout << _T("Creating and numbering C1.\n");
    print(C3,_T("C3"));

    stk_cout << _T("C1 = C2, with C1 diagonal and C2 point.\n");
    C1 = C2;

    print(C1,_T("C1"));
    stk_cout << _T("C1 = C3, with C1 diagonal and C3 vector.\n");
    C1 = C3;
    print(C1,_T("C1"));

    return 0;
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
      M      = 13;
      N      = 15;
      Istart = 3;
      Jstart = 4;
      Iend   = 10;
      Jend   = 12;
      output = true;
      iter = 1;

    }
    else
    {
      // dimensions
      M      = std::atoi(argv[1]);
      N      = std::atoi(argv[2]);
      Istart = std::atoi(argv[3]);
      Jstart = std::atoi(argv[4]);
      Iend   = std::atoi(argv[5]);
      Jend   = std::atoi(argv[6]);
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
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("+ Test CArray.                                     +\n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("Test<int> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      Test<int>(M, N, I, J, output);
      Real test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");

      M *=2;
      N *=2;
      Istart *= 2;
      Jstart *= 2;
      Iend   *= 2;
      Jend   *= 2;
    }

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for CArray.      +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception const& error)
  {
    stk_cerr << _T("An error occured : ") << error.error() << "\n";
  }

  return 0;
}
