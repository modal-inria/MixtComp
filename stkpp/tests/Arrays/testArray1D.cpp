/*--------------------------------------------------------------------*/
/*   Copyright (C) 2004-2007  Serge Iovleff

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

/** @file testArray1D.cpp
 *  @brief In this file we test the TContainer 1D classes.
 **/

#include "Arrays.h"

using namespace STK;

template< class TYPE, template<class> class Container2D >
void print(Container2D<TYPE> const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".capacityHo() =")   << A.capacityHo()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n");
  stk_cout << name << _T(".rangeCols().isRef() =")  << A.rangeCols().isRef() << _T("\n");
  stk_cout << name << _T(".rangeCols() = ")  << A.rangeCols() << _T("\n");
  stk_cout << name << _T(".capacityCols().isRef() =") << A.capacityCols().isRef()  << _T("\n");
  stk_cout << name << _T(".capacityCols() = ") << A.capacityCols()  << _T("\n");
  stk_cout << name << _T("=\n")               << A << _T("\n\n");
}

template< class TYPE >
void print(Array1D<TYPE> const& A, String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".range() =")      << A.range()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n");
  stk_cout << name << _T(".capacity() = ") << A.capacity() << _T("\n");
  stk_cout << name << _T("=\n")               << A << _T("\n\n");
}

template< class TYPE, template<class> class Container1D >
void write_Array(const Container1D<TYPE>& C, String const& name, String const& message)
{
  stk_cout << _T("==>") << message;
  print(C, name);
}

/* templated main test function for Array2DVector.                          */
template< class TYPE, template<class> class Container1D >
void testArray( int M, int N, Range J, bool output)
{
  Container1D<TYPE> B(N,4);
  if (output)
  { write_Array(B, _T("B"), _T("Test constructor:  B(N,4)\n"));}
 
  Container1D<TYPE> C;
  if (output)
  { write_Array(C, _T("C"), _T("Test null constructor: C()\n"));}

  Container1D<TYPE> D(Range(0,M, 0)); D= TYPE(3);
  if (output)
  { write_Array(D, _T("D"), _T("Test constructor: D(Range(0,M)); D = 3\n"));}

  B.exchange(D);
  if (output)
  { write_Array(B, _T("B"), _T("Test B.exchange(D);\n"));
    write_Array(D, _T("D"), _T(""));
  }

  D.sub(J)= TYPE(5);
  if (output)
  { write_Array(D, _T("D"), _T("Test D.sub(J) = 5 (modification of a lhs object !)\n"));}

  Container1D<TYPE> Dref(D.sub(Range(J-1)), true);
  Dref.copy(B.sub(J));
  if (output)
  { write_Array(D, _T("D"), _T("Test Dref(D.sub(Range(J-1)), true); Dref.copy(B.sub(J))\n"));}

  D.shift(1);
  if (output)
  { write_Array(D, _T("D"), _T("Test D.shift(1)\n"));}

  D.pushBack(2); D.sub(Range(M,D.lastIdx(), 0)) = TYPE(1);
  if (output)
  { write_Array(D, _T("D"), _T("Test D.pushBack(2); D.sub(Range(M,D.lastIdx()), 0) = 1\n"));}

  D.swap(D.firstIdx(),D.lastIdx());
  if (output)
  { write_Array(D, _T("D"), _T("Test D.swap(D.firstIdx(),D.lastIdx())\n"));}

  D.erase(2, D.size()-1);
  if (output)
  { write_Array(D, _T("D"), _T("Test D.erase(2,D.size()-1)\n"));}

  D.erase(D.lastIdx());
  if (output)
  { write_Array(D, _T("D"), _T("Test D.erase(D.lastIdx())\n"));}

  D.pushBack(M); D = TYPE(5);
  if (output)
  { write_Array(D, _T("D"), _T("Test D.pushBack(M); D=5\n"));}

  D.resize(J);
  if (output)
  { write_Array(D, _T("D"), _T("Test D.resize(J)\n"));}

  C.resize(D.range());
  C= TYPE(0);
  if (output)
  { write_Array(C, _T("C"), _T("Test C.resize(D.range());C= TYPE(0);\n"));}

  C = D;
  if (output)
  { write_Array(C, _T("C"), _T("Test C = D\n"));
    write_Array(D, _T("D"), _T("\n"));
  }

  C.front() = TYPE(-2); C.back()  = TYPE(-2);
  if (output)
  { write_Array(C, _T("C"), _T("Test C.front() = -2 and C.back() = -2\n"));}

  C.elt(J.firstIdx()) = C.front();
  C.elt(J.lastIdx())  = C.back();
  if (output)
  { write_Array(C, _T("C"), _T("Test C.at(J.firstIdx()) = C.front() and C.at(J.lastIdx()) = C.back()\n"));}

  C.elt(C.lastIdx()-1) = -C.elt(C.lastIdx()-1);
  if (output)
  { write_Array(C, _T("C"), _T("Test C.insert(Range(C.lastIdx(),C.lastIdx()), 4); C.at(C.lastIdx()-1) = -C.at(C.lastIdx()-1)\n"));}

  C.erase(J.firstIdx(), J.size());
  if (output)
  { write_Array(C, _T("C"), _T("Test C.erase(J.firstIdx(), J.size())\n"));}
}


/* main. */
int main(int argc, char *argv[])
{
  int M, N;
  int Jbeg, Jend;
  bool output = true;
  
  if (argc < 5)
  {
    M=12;
    N=15;
    Jbeg = 3;
    Jend = 9;
  }
  else
  {
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    Jbeg = atoi(argv[3]);
    Jend = atoi(argv[4]);
    if (argc == 6) output = atoi(argv[5]);
  }
//
//  try
//  {
    Range J(Jbeg, Jend, 0);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test 1D Arrays                                      +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Using M  = ") << M    << _T(" N    = ") << N  << _T("\n")
             << _T("J = ") << J << _T("\n");
    stk_cout << _T("\n\n");

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("Test Array1D<int> : \n");
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    testArray<int, Array1D>(M, N, J, output);
//    stk_cout << _T("\n\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Test Array1D<Real> : \n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    testArray<Real, Array1D>(M, N, J, output);
    stk_cout << _T("\n\n");

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("Test Array2DVector<int> : \n");
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    testArray<int, Array2DVector>(M, N, J, output);
//    stk_cout << _T("\n\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Test Array2DVector<Real> : \n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    testArray<Real, Array2DVector>(M, N, J, output);
    stk_cout << _T("\n\n");

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("Test Array2DPoint<int> : \n");
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    testArray<int, Array2DPoint>(M, N, J, output);
//    stk_cout << _T("\n\n");
//
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Test Array2DPoint<Real> : \n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    testArray<Real, Array2DPoint>(M, N, J, output);
    stk_cout << _T("\n\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::Array     +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
//  }
//  catch (Exception & error)
//  {
//    std::cerr << "An error occured : " << error.error() << _T("\n";);
//    return -1;
//  }
  return 0;
}
