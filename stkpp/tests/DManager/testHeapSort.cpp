/*--------------------------------------------------------------------*/
/*     Copyright (C) 2003  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA
  
    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                   */

/* Project : DManager
 * File    : testHeapSort.cpp
 * Contents: test heapsort program
 * Author  : Serge Iovleff
*/

#include "../../include/STatistiK.h"
#include "../../include/DManager.h"

using namespace STK;

/*--------------------------------------------------------------------*/
/* main.                                                                */
int main(int argc, char *argv[])
{
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+   Test HeapSort                                   +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  // first test
  Matrix A(11,3);
  RandBase ran;
  // simulate 1D manifold
  stk_cout << _T("Test heapSort method with with 11 rows and 3 column\n");
  for (int i=A.firstIdxRows(); i<=A.lastIdxRows(); i++)
  {
	for (int j=A.firstIdxCols(); j<= A.lastIdxCols(); j++ )
      A(i, j) = ran.randGauss();
  }
  int iCol =A.firstIdxCols();
  stk_cout << _T("A=\n") << A << _T("\n");
  stk_cout << _T("Test heapSort() of the first column\n");
  Vector A1(A.col(iCol), true);
  heapSort(A1);
  stk_cout << _T("A=\n") << A << _T("\n");

  // running heapsort
  stk_cout << _T("Test heapSort() method with shift(-5,1) on the column 2\n");
  A.shift(-5, 1);
  Vector A2(A.col(2), true);
  heapSort(A2);
  stk_cout << _T("A=\n") << A << _T("\n");

  // running heapsort
  stk_cout << _T("Test heapSort() with index on the third column\n");
  Vector A3(A.col(iCol+2), true);
  Array2DVector<int> index;
  heapSort(index, A3);
  stk_cout << _T("indexes = ") << index << _T("\n");
  stk_cout << _T("Test ApplySort1D to the third column\n");
  applySort1D(A3, index);
  stk_cout << _T("A=\n") << A << _T("\n");

  stk_cout << _T("Test heapSort method with indexes, sort the column 1\n");
  A.resize(10, 2);
  Vector Ab1(A.col(1), true);
  for (int i=A.firstIdxRows(); i<=A.lastIdxRows(); i++)
  {
	for (int j=A.firstIdxCols(); j<= A.lastIdxCols(); j++ )
      A(i, j) = ran.randGauss();
  }
  stk_cout << _T("A=\n") << A << _T("\n");

  heapSort(index, Ab1);
  stk_cout << _T("indexes = ") << index << _T("\n");
  stk_cout << _T("Test applySort2D()\n");
  applySort2D(A, index);
  stk_cout << _T("A=\n") << A << _T("\n");

  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for STK::HeapSort  +\n");
  stk_cout << _T("+ No errors detected.                                 +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

}

