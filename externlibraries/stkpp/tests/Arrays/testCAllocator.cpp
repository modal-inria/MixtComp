/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-204  Serge Iovleff

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
 * Project:  stkpp::Arrays
 * created on: 13 sept. 204
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file testAllocator.cpp
 *  @brief In this file .
 **/

#include "../../include/STKernel.h"
#include "../../projects/Arrays/include/STK_CAllocator.h"
#include "../../projects/Arrays/include/STK_Display.h"

using namespace STK;

/* main print. */
template<class Allocator >
void printHead(Allocator const& A, String const& name)
{
    stk_cout << "print: " << name << "\n";
    stk_cout << name << _T(".isRef() =")    << A.isRef()  << _T("\n");
    stk_cout << name << _T(".idx() =")    << A.idx()  << _T("\n");
    stk_cout << name << _T(".rows() =")   << A.rows() << _T("\n");
    stk_cout << name << _T(".cols() =")   << A.cols() << _T("\n");
    stk_cout << name << _T(".sizeRows() =")   << A.sizeRows() << _T("\n");
    stk_cout << name << _T(".sizeCols() =")   << A.sizeCols() << _T("\n");
    stk_cout << name << _T(".firstData() =")   << A.firstData() << _T("\n");
    stk_cout << name << _T(".sizeData() =")   << A.sizeData() << _T("\n");
}

/* main print.*/
template<class Allocator >
void print(Allocator const& A, String const& name)
{
    printHead(A, name);
    stk_cout << name << _T("=\n");

    stk_cout << A;
}


template< class TYPE>
int Test( int M, int N, Range I, Range J, bool output)
{
    //
    stk_cout << _T("Test 8 constructors with size(M, N).\n");
    CAllocator<TYPE, Arrays::general_, UnknownSize, UnknownSize, Arrays::by_col_> B0(M, N);
    B0.setValue(9); print(B0, "B0");
    CAllocator<TYPE, Arrays::general_, 3, 4, Arrays::by_col_> B1(M, N);
    B1.setValue(9); print(B1, "B1");
    CAllocator<TYPE, Arrays::general_, UnknownSize, 4, Arrays::by_col_> B2(M, N);
    B2.setValue(9); print(B2, "B2");
    CAllocator<TYPE, Arrays::general_, 3, UnknownSize, Arrays::by_col_> B3(M, N);
    B3.setValue(9); print(B3, "B3");
    CAllocator<TYPE, Arrays::general_, UnknownSize, UnknownSize, Arrays::by_row_> B4(M, N);
    B4.setValue(9); print(B4, "B4");
    CAllocator<TYPE, Arrays::general_, 3, 4, Arrays::by_row_> B5(M, N);
    B5.setValue(9); print(B5, "B5");
    CAllocator<TYPE, Arrays::general_, UnknownSize, 4, Arrays::by_row_> B6(M, N);
    B6.setValue(9); print(B6, "B6");
    CAllocator<TYPE, Arrays::general_, 3, UnknownSize, Arrays::by_row_> B7(M, N);
    B7.setValue(9); print(B7, "B7");
    //
    stk_cout << _T("Test 8 constructors with size(M, N) and value.\n");
    CAllocator<TYPE, Arrays::general_, UnknownSize, UnknownSize, Arrays::by_col_> C0(M, N, 0);
    CAllocator<TYPE, Arrays::general_, 3, 4, Arrays::by_col_> C1(M, N, 1);
    CAllocator<TYPE, Arrays::general_, UnknownSize, 4, Arrays::by_col_> C2(M, N, 2);
    CAllocator<TYPE, Arrays::general_, 3, UnknownSize, Arrays::by_col_> C3(M, N, 3);
    CAllocator<TYPE, Arrays::general_, UnknownSize, UnknownSize, Arrays::by_row_> C4(M, N, 4);
    CAllocator<TYPE, Arrays::general_, 3, 4, Arrays::by_row_> C5(M, N, 5);
    CAllocator<TYPE, Arrays::general_, UnknownSize, 4, Arrays::by_row_> C6(M, N, 6);
    CAllocator<TYPE, Arrays::general_, 3, UnknownSize, Arrays::by_row_> C7(M, N, 7);
    //
    stk_cout << _T("Test row and column Access.\n");
    B0.row(2).setValue(-8);
    B0.col(2).setValue(-9);
    B0.row(1, J).setValue(-1);
    B0.col(I, 1).setValue(-2);
    B0.sub(I, J).setValue(-3);
    print(B0,"B0");
    B1.row(2).setValue(-8);
    B1.col(2).setValue(-9);
    B1.row(1, J).setValue(-1);
    B1.col(I, 1).setValue(-2);
    B1.sub(I, J).setValue(-3);
    print(B1,"B1");
    B2.row(2).setValue(-8);
    B2.col(2).setValue(-9);
    B2.row(1, J).setValue(-1);
    B2.col(I, 1).setValue(-2);
    B2.sub(I, J).setValue(-3);
    print(B2,"B2");
    B3.row(2).setValue(-8);
    B3.col(2).setValue(-9);
    B3.row(1, J).setValue(-1);
    B3.col(I, 1).setValue(-2);
    B3.sub(I, J).setValue(-3);
    print(B3,"B3");
    B4.row(2).setValue(-8);
    B4.row(1, J).setValue(-1);
    B4.col(2).setValue(-9);
    B4.col(I, 1).setValue(-2);
    B4.sub(I, J).setValue(-3);
    print(B4,"B4");
    B5.row(2).setValue(-8);
    B5.col(2).setValue(-9);
    B5.row(1, J).setValue(-1);
    B5.col(I, 1).setValue(-2);
    B5.sub(I, J).setValue(-3);
    print(B5,"B5");
    B6.row(2).setValue(-8);
    B6.col(2).setValue(-9);
    B6.row(1, J).setValue(-1);
    B6.col(I, 1).setValue(-2);
    B6.sub(I, J).setValue(-3);
    print(B6,"B6");
    B7.row(2).setValue(-8);
    B7.col(2).setValue(-9);
    B7.row(1, J).setValue(-1);
    B7.col(I, 1).setValue(-2);
    B7.sub(I, J).setValue(-3);
    print(B7,"B7");

    //
    stk_cout << _T("Test swap and shift.\n");
    B0.swap(C0); B1.swap(C1);
    B2.swap(C2); B3.swap(C3);
    B4.swap(C4); B5.swap(C5);
    B6.swap(C6); B7.swap(C7);
    B0.shift(3, 2); B1.shift(3, 2);
    B2.shift(3, 2); B3.shift(3, 2);
    B4.shift(3, 2); B5.shift(3, 2);
    B6.shift(3, 2);
    B7.shift(3, 2);
    print(B0,"B0"); print(B1,"B1"); print(B2,"B2"); print(B3,"B3");
    print(B4,"B4"); print(B5,"B5"); print(B6,"B6");
    print(B6,"B6");
    print(B7,"B7");

    //
    stk_cout << _T("Test B.swap(C); B.realloc(M-1,N-1);\n");
    B0.swap(C0); B1.swap(C1);
    B2.swap(C2); B3.swap(C3);
    B4.swap(C4); B5.swap(C5);
    B6.swap(C6); B7.swap(C7);
    B0.realloc(M-1, N-1);print(B0,"B0");
    B1.realloc(M-1, N-1);print(B1,"B1");
    B2.realloc(M-1, N-1);print(B2,"B2");
    B3.realloc(M-1, N-1);print(B3,"B3");
    B4.realloc(M-1, N-1);print(B4,"B4");
    B5.realloc(M-1, N-1);print(B5,"B5");
    B6.realloc(M-1, N-1);print(B6,"B6");
    B7.realloc(M-1, N-1);print(B7,"B7");

    //
    stk_cout << _T("Test transpose.\n");
    print(B0.transpose(),"B0.transpose()"); print(B1.transpose(),"B1.transpose()");
    print(B2.transpose(),"B2.transpose()"); print(B3.transpose(),"B3.transpose()");
    print(B4.transpose(),"B4.transpose()"); print(B5.transpose(),"B5.transpose()");
    print(B6.transpose(),"B6.transpose()"); print(B7.transpose(),"B7.transpose()");
    //
    print(B0.col(1).transpose(),"B0.col(1).transpose()"); print(B1.col(1).transpose(),"B1.col(1).transpose()");
    print(B2.col(1).transpose(),"B2.col(1).transpose()"); print(B3.col(1).transpose(),"B3.col(1).transpose()");
    print(B4.col(1).transpose(),"B4.col(1).transpose()"); print(B5.col(1).transpose(),"B5.col(1).transpose()");
    print(B6.col(1).transpose(),"B6.col(1).transpose()"); print(B7.col(1).transpose(),"B7.col(1).transpose()");
    print(B0.row(1).transpose(),"B0.row(1).transpose()"); print(B1.row(1).transpose(),"B1.row(1).transpose()");
    //
    print(B2.row(1).transpose(),"B2.row(1).transpose()"); print(B3.row(1).transpose(),"B3.row(1).transpose()");
    print(B4.row(1).transpose(),"B4.row(1).transpose()"); print(B5.row(1).transpose(),"B5.row(1).transpose()");
    print(B6.row(1).transpose(),"B6.row(1).transpose()"); print(B7.row(1).transpose(),"B7.row(1).transpose()");
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
      M      = 3;
      N      = 4;
      Istart = 1;
      Iend   = 2;
      Jstart = 2;
      Jend   = 3;
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
      stk_cout << _T("+ TestCAllocator.                                     +\n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
          << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("Test<Real, CAllocator> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      Test<Real>(M, N, I, J, output);
      Real test_time = Chrono::elapsed();
      //      stk_cout << _T("\n");
      //      stk_cout << _T("Time used : ") << test_time << _T("\n");
      //      stk_cout << _T("\n\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("Test<int> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      Test<int>(M, N, I, J, output);
      test_time = Chrono::elapsed();
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
    stk_cout << _T("+ Successful completion of testing for CAllocator.    +\n");
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
