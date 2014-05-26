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

/** @file testBinaryOperators.cpp
 *  @brief In this file we test the Binary operators classes.
 **/

#include "../../include/STKernel.h"
#include "../../include/Arrays.h"

using namespace STK;

/** @ingroup Arrays
  * @brief Base class to implement min and max visitors.
  * Allow to find the
  */
template <typename Type>
struct numberingVisitor
{
  Type value_;
  numberingVisitor() : value_(1) {};
  inline void operator() ( Type& value)
  { value = value_; ++value_;}
};

/** number the element of a container in the order they are visited.
  * @sa  maxElt(Derived const&, int&, int&), minElt()
  */
template<typename Derived>
void numbering(Derived& matrix)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  numberingVisitor<Type> visitor;
  ArrayBaseApplier<Derived, numberingVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.apply(visitor);
}

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


template< class Type>
int Test( int M, int N, bool output)
{
    //
    //int row=1, col=1;
    CArray<Type , UnknownSize, UnknownSize, Arrays::by_col_> C0(M, N, 0);
    CArray<Type , 3, 4, Arrays::by_col_> C1(M, N);
    CArray<bool, 3, 4, Arrays::by_row_> C2(M, N, 1);
    numbering(C1);
    stk_cout << "C1<Type,...> = \n" << C1;
    stk_cout << "C2<bool,...> = \n" << C2;
    // first test
    C0 = Type(2)*C1 - Type(3) * C2;
    stk_cout << "Test: C0 = 2*C1 - 3*C2; C0 =\n" << C0;
    C0 = Type(2)*C1 - Type(3) * C2.template cast<Type>();
    stk_cout << "Test: C0 = 2*C1 - 3*C2.cast<Type>(); C0 =\n" << C0;
    C0 -= Type(3)*C1;
    stk_cout << "Test: C0 -= 3*C1; C0 =\n" << C0;

    CArraySquare<Type , 4, 4> C3(M, 0);
    Array2DDiagonal<Type> C4(4, 4);
    numbering(C3);
    numbering(C4);
    stk_cout << "C3= \n" << C3;
    stk_cout << "C4= \n" << C4;

    CArray<Type , 4, 4> C5(M, N, 0);
    C5 = 3*C3 - Type(2)*C4;
    stk_cout << "Test: C5 = 3*C3 - 2*C4; C5 =\n" << C5;

    return 0;
}

/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
//  try
//  {
    int M, N, output, iter;
    if (argc < 3)
    {
      // 12 15 3 4 10 12 1 2
      M      = 5;
      N      = 4;
      output = true;
      iter = 1;

    }
    else
    {
      // dimensions
      M      = std::atoi(argv[1]);
      N      = std::atoi(argv[2]);
      output = true;
      if (argc >3 ) output = atoi(argv[4]);
      // number of test
      iter =1;
      if (argc >4 ) iter = atoi(argv[5]);
    }

    for( int i=1; i<=iter; i++)
    {
      stk_cout << _T("\n\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("+ Test Binary operators                               +\n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N << _T("\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

//      // time variables
      Chrono::start();
      Test<Real>(M, N, output);
      Real test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
//
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("Test with int : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      Test<int>(M, N, output);
      test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");

      M *=2;
      N *=2;
    }

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for CArray.      +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
//  }
//  catch (Exception const& error)
//  {
//    stk_cerr << _T("An error occured : ") << error.error() << _T("\n";);
//  }

  return 0;
}
