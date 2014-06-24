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
void TestArray2D( int M, int N, Range I, Range J, bool output)
{
    //
    if (output) stk_cout << _T("Test constructor B(N, M, 3) \n");
    Array2D<Type> B(N, M, 3);
    if (output) { print(B,"B");}

    //
    if (output) stk_cout << _T("Test constructor A(M,N); numbering(A);\n");
    Array2D<Type> A(M,N);
    numbering(A);
    if (output) { print(A, "A");}

    //
   if (output) stk_cout << _T("Test constructor Testing C(A, true) \n");
   Array2D<Type> C(A, true);
   if (output) { print(C, "C");}
    //
    if (output)
      stk_cout << _T("Testing B.resize(Inx(1,M),Inx(1,N)); B=2\n");
    B.resize(Range(1, M, 0), Range(1, N, 0)); B = Type(2);
    if (output) { print(B, "B");}

    //
    if (output) stk_cout << _T("Test A(I,J).copy(B(I,J))\n");
    A(I,J).copy(B(I,J));
    if (output) { print(A, "A");}

    //
    if (output)
    { stk_cout << _T("Testing Inx + operators: A(I+1,J-1) = 1;\n");
      stk_cout << _T("I= ")  << I  << _T(" ") << _T("J= ")  << J   << _T("\n");
      stk_cout << _T("I+1= ")<<I+1 << _T(" ") << _T("J-1= ")<< J-1 << _T("\n");
    }
    Array2D<Type> Aref(A, I+1, J-1);
    Aref = Type(1);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing  G = A(I,2):\n");
    typename Array2D<Type>::Col G = A(I,2);
    if (output) { print(G, "G");}

    //
    if (output) stk_cout << _T("Testing  A(I+2,1).copy(G)\n");
    if (output) { print(A(I+2,1), "A(I+2,1)");}
    A(I+2,1).copy(G);
    if (output) { print(A, "A");}
    
    //
    if (output) stk_cout << _T("Testing  H=A(3,J)\n");
    typename Array2D<Type>::Row H = A(3,J);
    if (output) stk_cout << _T("H = \n") << H << _T("\n\n");

    //
    if (output) stk_cout << _T("Testing  A(4,J+1).copy(H)\n");
    A(4,J.inc(1)).copy(H);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing  A.shift(-1,3)\n");
    A.shift(-1,3);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing  A.pushBackCols(1);A.col(A.lastIdxCols()) = Type(0); A.col(A.lastIdxCols()).copy(B.col(1))\n");
    A.pushBackCols(1);
    A.col(A.lastIdxCols()) = Type(0);
    A.col(A.lastIdxCols()).copy(B.col(1));
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing  A.insertCols(A.firstIdxCols()+1,4) and A.col(A.firstIdxCols()+Range(1,4))=4\n");
    A.insertCols(A.firstIdxCols()+1,4);
    A.col(A.firstIdxCols()+Range(1,4)) = Type(4);
    if (output) { print(A, "A");}

     //
    if (output) stk_cout << _T("Testing  A.eraseCols(A.firstIdxCols()+2)\n");
    A.eraseCols(A.firstIdxCols()+2);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing  A.popBackCols()\n");
    A.popBackCols();
    if (output) { print(A, "A");}

     //
    if (output) stk_cout << _T("Testing A.swapRows(A.firstIdxRows()+2,A.firstIdxRows()+M-1) \n");
    A.swapRows(A.firstIdxRows()+2,A.firstIdxRows()+M-1);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.eraseRows(A.firstIdxRows()+0, A.firstIdxRows()+2) \n");
    A.eraseRows(A.firstIdxRows()+0, A.firstIdxRows()+2);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.insertRows(A.firstIdxRows()+1, 2) and A(A.firstIdxRows()+1:2,:) = A(A.firstIdxRows()+4:5,:)\n");
    A.insertRows(A.firstIdxRows()+1,2);
    A(A.firstIdxRows()+Range(1,2), A.cols()).copy(A(A.firstIdxRows()+Range(4,5, 0), A.cols()));
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.resize(Inx(1, M), Inx(1, M))\n");
    A.resize(Range(1,M, 0), Range(1,M, 0));
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.eraseRows(A.firstIdxRows(), A.sizeRows())\n");
    A.eraseRows(A.firstIdxRows(),A.sizeRows());
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.insertCols(A.firstIdxCols()+1,2) and A.pushBackRows(2), A=2\n");
    A.insertCols(A.firstIdxCols()+1,2);
    A.pushBackRows(2);
    A = Type(2);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.clear() and A = B\n");
    A.clear();
    A = B;
    if (output) { print(A, "A");}

    if (output) stk_cout << _T("Testing A.insertRows(A.firstIdxRows()+2, 5) and A(A.firstIdxRows()+2:7,A.cols()) = 3.\n");
    A.insertRows(A.firstIdxRows()+2,5);
    A(Range(A.firstIdxRows()+2,5), A.cols()) = Type(3);
    if (output) { print(A, "A");}

   //
    if (output)  stk_cout << _T("Testing A.eraseCols(A.firstIdxCols(), A.sizeCols())\n");
    A.eraseCols(A.firstIdxCols(),A.sizeCols());
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.insertRows(1,2) and A.pushBackCols(2), A=2\n");
    A.insertRows(1,2);
    A.pushBackCols(2);
    A = Type(2);
    if (output) { print(A, "A");}

    //
    if (output) stk_cout << _T("Testing A.exchange(B) \n");
    A.exchange(B);
    if (output)
    { print(A, "A");  print(B, "B");}

    //
    if (output) stk_cout << _T("Testing A.resize(2*A.sizeRows(), A.sizeCols()) ")
                         << _T(" and A = 1 \n");
    A.resize(2*A.sizeRows(), A.sizeCols());
    A = Type(1);
    if (output)
    { print(A, "A"); print(B, "B");}

    //
    if (output) stk_cout << _T("Testing D(A); D = 2; \n");
    Array2D<Type> D(A);
    D=Type(2);
    if (output)
    { print(A, "A"); print(D, "D");}
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
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("+ Test STK::Arrays                                    +\n");
      stk_cout << _T("+ Using: \n");
      stk_cout << _T("M = ") << M << _T(", N = ") << N
               << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
      stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestArray2D<Real> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      TestArray2D<Real>(M, N, I, J, output);
      Real test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");
      
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      stk_cout << _T("TestArray2D<int> : \n");
      stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      // time variables
      Chrono::start();
      TestArray2D<int>(M, N, I, J, output);
      test_time = Chrono::elapsed();
      stk_cout << _T("\n");
      stk_cout << _T("Time used : ") << test_time << _T("\n");
      stk_cout << _T("\n\n");

      M *=2; N *=2; Istart *= 2; Jstart *= 2;  Iend   *= 2;  Jend   *= 2;
    }
  }
  catch (Exception const& error)
  {
    std::cerr << error.error() << _T("\n";);
  }

  return 0;
}
