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

/** @file testUnaryOperators.cpp
 *  @brief In this file we test UnaryOperator class
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
int Test( int M, int N)
{
    //
    //int row=1, col=1;
    stk_cout << _T("Test multiplication expression.\n");
    CArray<Type , UnknownSize, UnknownSize, Arrays::by_col_> C0;
    CArray<Type , 3, 4, Arrays::by_row_> C1(M, N, 1);
    numbering(C1);
    print(C0, "C0<Type, array2D_,by_col_, X, X>()");
    print(C1, "C1<Type, array2D_,by_row_, 3, 4>");
    C0 = C1 * Type(2);
    print(C0,"C0 = C1 * Type(2)");


    Real meanlog = C1.col(1).log().sum();
    stk_cout << "meanlog =" << meanlog << _T("\n";);
//    C1(1,1) = Arithmetic<Type>::NA();
//    meanlog = C1.col(1).log().sum();
//    stk_cout << "meanlog =" << meanlog << _T("\n";);

//    CArray<bool, Arrays::array2D_, 3, 4, Arrays::by_col_> C3(M, N, 1);
//    stk_cout << "C3<bool, array2D_,by_col_, 3, 4> =\n" << C3;
//    C0 = -Type(2)*C1;
//    stk_cout << "C0 = -2*C1; C0 =\n" << C0;
//    C0 = (-Type(2)*C1/Type(4)).square();
//    stk_cout << "C0 = (-2*C1/4).square(); C0 =\n" << C0;
//    C0 = C3.template cast<Type>() * Type(2);
//    stk_cout << "C0 = C3.template cast<Real>() * 2; C0 =\n" << C0;
    return 0;
}

/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  try
  {
    int M, N;
    if (argc < 7)
    {
      M = 3; N = 4;
    }
    else
    {
      // dimensions
      M      = std::atoi(argv[1]);
      N      = std::atoi(argv[2]);
    }

      stk_cout << _T("\n\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("+ Test UnaryOperators.                                     +\n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N << _T("\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("Test<Real> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      Test<Real>(M, N);
      Real test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
//
//      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//      stk_cout << _T("Test<int> : \n");
//      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//      // time variables
//      Chrono::start();
//      Test<int>(M, N);
//      test_time = Chrono::elapsed();
//      stk_cout << _T("\n");
//      stk_cout << _T("Time used : ") << test_time << _T("\n");
//      stk_cout << _T("\n\n");
//
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for UnaryOperators.+\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception const& error)
  {
    stk_cerr << _T("An error occured : ") << error.error() << _T("\n";);
  }

  return 0;
}
