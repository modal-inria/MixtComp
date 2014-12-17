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

/** @file testCArrays.cpp
 *  @brief In this file we test the CArray* classes.
 **/

#include "STKernel.h"
#include "Arrays.h"

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
  ArrayBaseApplier<Derived, numberingVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.apply(visitor);
}


/* main print. */
template<class CArray >
void printHead(CArray const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")    << A.isRef()  << _T("\n");
  stk_cout << name << _T(".rows() =")     << A.rows() << _T("\n");
  stk_cout << name << _T(".cols() =")     << A.cols() << _T("\n");
  stk_cout << name << _T(".sizeRows() =") << A.sizeRows() << _T("\n");
  stk_cout << name << _T(".sizeCols() =") << A.sizeCols() << _T("\n");
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
int TestGeneral( int M, int N, Range I, Range J, bool output)
{
    //
    int row=1, col=1;
    stk_cout << _T("Test row, column and subArray acccess.\n");
    CArray<Type , UnknownSize, UnknownSize, Arrays::by_col_> C0(M, N, 0);
    numbering(C0);  C0(2,4)=8; C0(4,3)=8;
    C0.elt(3, 4)=0;  C0.elt(M-1, N-1)=0;
    print(C0, "C0");
    stk_cout << "C0: Min Coeff =" << C0.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C0: Max Coeff =" << C0.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , 10, 12, Arrays::by_col_> C1(M, N, 1);
    numbering(C1); C1(2,4)=8; C1(4,3)=8;
    C1.elt(3, 4)=0;  C1.elt(M-1, N-1)=0;
    print(C1, "C1");
    stk_cout << "C1: Min Coeff =" << C1.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C1: Max Coeff =" << C1.maxElt( row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , UnknownSize, 12, Arrays::by_col_> C2(M, N, 2);
    numbering(C2); C2(2,4)=8; C2(4,3)=8;
    C2.elt(3, 4)=0;  C2.elt(M-1, N-1)=0;
    print(C2, "C2");
    stk_cout << "C2: Min Coeff =" << C2.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C2: Max Coeff =" << C2.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , 10, UnknownSize, Arrays::by_col_> C3(M, N, 3);
    numbering(C3); C3(2,4)=8; C3(4,3)=8;
    C3.elt(3, 4)=0;  C3.elt(M-1, N-1)=0;
    print(C3, "C3");
    stk_cout << "C3: Min Coeff =" << C3.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C3: Max Coeff =" << C3.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , UnknownSize, UnknownSize, Arrays::by_row_> C4(M, N, 4);
    numbering(C4); C4(2,4)=8; C4(4,3)=8;
    C4.elt(3, 4)=0;  C4.elt(M-1, N-1)=0;
    print(C4, "C4");
    stk_cout << "C4: Min Coeff =" << C4.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C4: Max Coeff =" << C4.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , 12, UnknownSize, Arrays::by_row_> C5(M, N, 5);
    numbering(C5); C5(2,4)=8; C5(4,3)=8;
    C5.elt(3, 4)=0;  C5.elt(M-1, N-1)=0;
    print(C5, "C5");
    stk_cout << "C5: Min Coeff =" << C5.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C5: Max Coeff =" << C5.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , UnknownSize, 12, Arrays::by_row_> C6(M, N, 6);
    numbering(C6); C6(2,4)=8; C6(4,3)=8;
    C6.elt(3, 4)=0;  C6.elt(M-1, N-1)=0;
    print(C6, "C6");
    stk_cout << "C6: Min Coeff =" << C6.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C6: Max Coeff =" << C6.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    CArray<Type , 10, UnknownSize, Arrays::by_row_> C7(M, N, 7);
    numbering(C7); C7(2,4)=8; C7(4,3)=8;
    C7.elt(3, 4)=0;  C7.elt(M-1, N-1)=0;
    print(C7, "C7");
    stk_cout << "C7: Min Coeff =" << C7.minElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
    stk_cout << "C7: Max Coeff =" << C7.maxElt(row, col);
    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;

    return 0;
}

//template< class TYPE>
//int TestUpper( int M, int N, Range I, Range J, bool output)
//{
//    //
//    int row=1, col=1;
//    stk_cout << _T("Test row, column and subArray acccess.\n");
//    CArray<TYPE, Arrays::upper_triangular_, UnknownSize, UnknownSize, Arrays::by_col_> C0(M, N, 0);
//    numbering(C0);  C0(2,4)=8; C0.col(3)[2]=8;
//    C0.sub(I, J).setValue(6); C0.elt(3, 4)=0;  C0.elt(M-1, N-1)=0;
//    print(C0, "C0");
//    stk_cout << "C0: Min Coeff =" << C0.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C0: Max Coeff =" << maxElt(C0, row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, 10, 12, Arrays::by_col_> C1(M, N, 1);
//    numbering(C1); C1(2,4)=8; C1.col(3)[2]=8;
//    C1.sub(I, J).setValue(6); C1.elt(3, 4)=0;  C1.elt(M-1, N-1)=0;
//    print(C1, "C1");
//    stk_cout << "C1: Min Coeff =" << C1.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C1: Max Coeff =" << C1.maxElt( row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, UnknownSize, 12, Arrays::by_col_> C2(M, N, 2);
//    numbering(C2); C2(2,4)=8; C2.col(3)[2]=8;
//    C2.sub(I, J).setValue(6); C2.elt(3, 4)=0;  C2.elt(M-1, N-1)=0;
//    print(C2, "C2");
//    stk_cout << "C2: Min Coeff =" << C2.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C2: Max Coeff =" << C2.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, 10, UnknownSize, Arrays::by_col_> C3(M, N, 3);
//    numbering(C3); C3(2,4)=8; C3.col(3)[2]=8;
//    C3.sub(I, J).setValue(6); C3.elt(3, 4)=0;  C3.elt(M-1, N-1)=0;
//    print(C3, "C3");
//    stk_cout << "C3: Min Coeff =" << C3.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C3: Max Coeff =" << C3.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, UnknownSize, UnknownSize, Arrays::by_row_> C4(M, N, 4);
//    numbering(C4); C4(2,4)=8; C4.col(3)[2]=8;
//    C4.sub(I, J).setValue(6);
//    C4.elt(3, 4)=0;  C4.elt(M-1, N-1)=0;
//    print(C4, "C4");
//    stk_cout << "C4: Min Coeff =" << C4.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C4: Max Coeff =" << C4.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, 10, 12, Arrays::by_row_> C5(M, N, 5);
//    numbering(C5); C5(2,4)=8; C5.col(3)[2]=8;
//    C5.sub(I, J).setValue(6); C5.elt(3, 4)=0;  C5.elt(M-1, N-1)=0;
//    print(C5, "C5");
//    stk_cout << "C5: Min Coeff =" << C5.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C5: Max Coeff =" << C5.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, UnknownSize, 12, Arrays::by_row_> C6(M, N, 6);
//    numbering(C6); C6(2,4)=8; C6.col(3)[2]=8;
//    C6.sub(I, J).setValue(6); C6.elt(3, 4)=0;  C6.elt(M-1, N-1)=0;
//    print(C6, "C6");
//    stk_cout << "C6: Min Coeff =" << C6.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C6: Max Coeff =" << C6.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, 10, UnknownSize, Arrays::by_row_> C7(M, N, 7);
//    numbering(C7); C7(2,4)=8; C7.col(3)[2]=8;
//    C7.sub(I, J).setValue(6); C7.elt(3, 4)=0;  C7.elt(M-1, N-1)=0;
//    print(C7, "C7");
//    stk_cout << "C7: Min Coeff =" << C7.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C7: Max Coeff =" << C7.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    return 0;
//}
//
//template< class TYPE>
//int TestLower( int M, int N, Range I, Range J, bool output)
//{
//    //
//    int row=1, col=1;
//    stk_cout << _T("Test row, column and subArray acccess.\n");
//    CArray<TYPE, Arrays::lower_triangular_, UnknownSize, UnknownSize, Arrays::by_col_> C0(M, N, 0);
//    numbering(C0);  C0(2,4)=8; C0.col(3)[2]=8;
//    C0.sub(I, J).setValue(6); C0.elt(3, 4)=0;  C0.elt(M-1, N-1)=0;
//    print(C0, "C0");
//    stk_cout << "C0: Min Coeff =" << C0.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C0: Max Coeff =" << maxElt(C0, row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::lower_triangular_, 10, 12, Arrays::by_col_> C1(M, N, 1);
//    numbering(C1); C1(2,4)=8; C1.col(3)[2]=8;
//    C1.sub(I, J).setValue(6); C1.elt(3, 4)=0;  C1.elt(M-1, N-1)=0;
//    print(C1, "C1");
//    stk_cout << "C1: Min Coeff =" << C1.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C1: Max Coeff =" << C1.maxElt( row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::lower_triangular_, UnknownSize, 12, Arrays::by_col_> C2(M, N, 2);
//    numbering(C2); C2(2,4)=8; C2.col(3)[2]=8;
//    C2.sub(I, J).setValue(6); C2.elt(3, 4)=0;  C2.elt(M-1, N-1)=0;
//    print(C2, "C2");
//    stk_cout << "C2: Min Coeff =" << C2.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C2: Max Coeff =" << C2.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::lower_triangular_, 10, UnknownSize, Arrays::by_col_> C3(M, N, 3);
//    numbering(C3); C3(2,4)=8; C3.col(3)[2]=8;
//    C3.sub(I, J).setValue(6); C3.elt(3, 4)=0;  C3.elt(M-1, N-1)=0;
//    print(C3, "C3");
//    stk_cout << "C3: Min Coeff =" << C3.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C3: Max Coeff =" << C3.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::lower_triangular_, UnknownSize, UnknownSize, Arrays::by_row_> C4(M, N, 4);
//    numbering(C4); C4(2,4)=8; C4.col(3)[2]=8;
//    C4.sub(I, J).setValue(6); C4.elt(3, 4)=0;  C4.elt(M-1, N-1)=0;
//    print(C4, "C4");
//    stk_cout << "C4: Min Coeff =" << C4.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C4: Max Coeff =" << C4.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::lower_triangular_, 10, 12, Arrays::by_row_> C5(M, N, 5);
//    numbering(C5); C5(2,4)=8; C5.col(3)[2]=8;
//    C5.sub(I, J).setValue(6); C5.elt(3, 4)=0;  C5.elt(M-1, N-1)=0;
//    print(C5, "C5");
//    stk_cout << "C5: Min Coeff =" << C5.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C5: Max Coeff =" << C5.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::upper_triangular_, UnknownSize, 12, Arrays::by_row_> C6(M, N, 6);
//    numbering(C6); C6(2,4)=8; C6.col(3)[2]=8;
//    C6.sub(I, J).setValue(6); C6.elt(3, 4)=0;  C6.elt(M-1, N-1)=0;
//    print(C6, "C6");
//    stk_cout << "C6: Min Coeff =" << C6.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C6: Max Coeff =" << C6.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    CArray<TYPE, Arrays::lower_triangular_, 10, UnknownSize, Arrays::by_row_> C7(M, N, 7);
//    numbering(C7); C7(2,4)=8; C7.col(3)[2]=8;
//    C7.sub(I, J).setValue(6); C7.elt(3, 4)=0;  C7.elt(M-1, N-1)=0;
//    print(C7, "C7");
//    stk_cout << "C7: Min Coeff =" << C7.minElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n" ;
//    stk_cout << "C7: Max Coeff =" << C7.maxElt(row, col);
//    stk_cout << "; pos=("<< row <<"," << col << ")\n\n" ;
//
//    return 0;
//}
//
template< class TYPE>
int TestPoint( int M, int N, Range I, Range J, bool output)
{
    //
    int row=1;
    stk_cout << _T("Test constructor (M), resize(M+1) and elt acccess.\n");
    CPointX C0(M); C0.resize(M+1); C0.elt(C0.lastIdx()) = M;
    numbering(C0);
    C0.elt(I.begin())=M;  C0.elt(I.lastIdx())=M;
    print(C0, "C0");
    print(C0.transpose(), "C0.transpose()");
    stk_cout << "C0: Min Coeff =" << C0.minElt(row);
    stk_cout << "; pos= "<< row << "\n" ;
    stk_cout << "C0: Max Elt =" << C0.maxElt(row);
    stk_cout << "; pos= "<< row << "\n\n" ;

    return 0;
}

template< class TYPE>
int TestVector( int M, int N, Range I, Range J, bool output)
{
    //
    int row=1;
    stk_cout << _T("Test constructor (N), resize(N+1) and elt acccess.\n");
    CVectorX C0(N); C0.resize(N+1); C0.elt(C0.lastIdx()) = N;
    numbering(C0);
    C0.elt(J.begin())=N;  C0.elt(I.lastIdx())=N;
    print(C0, "C0");
    print(C0.transpose(), "C0.transpose()");
    stk_cout << "C0: Min Coeff =" << C0.minElt(row);
    stk_cout << "; pos= "<< row  << "\n" ;
    stk_cout << "C0: Max Coeff =" << C0.maxElt(row);
    stk_cout << "; pos= "<< row << "\n\n" ;

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
      M      = 5;
      N      = 6;
      Istart = baseIdx+1;
      Iend   = Istart+2;
      Jstart = baseIdx+2;
      Jend   = Jstart+3;
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
      stk_cout << _T("+ Test CArray.                                        +\n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestGeneral<Real> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      Chrono::start();
      TestGeneral<Real>(M, N, I, J, output);
      Real test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestUpper<Real> : \n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      Chrono::start();
      //TestUpper<Real>(M, N, I, J, output);
      test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestLower<Real> : \n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      Chrono::start();
      //TestLower<Real>(M, N, I, J, output);
      test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestPoint<Real> : \n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      Chrono::start();
      TestPoint<Real>(M, N, I, J, output);
      test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestVector<Real> : \n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      Chrono::start();
      TestVector<Real>(M, N, I, J, output);
      test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
    }

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for CArray.      +\n");
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
