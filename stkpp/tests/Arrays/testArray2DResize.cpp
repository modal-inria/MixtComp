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

/** @file testArray2DResize.cpp
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
void TestArray2D(Range I, Range J, bool output)
{
    Range R=I, S =J;
    //
    if (output) stk_cout << _T("Test constructor B \n");
    Array2D<Type> B;
    if (output) { print(B,"B");}
    //
    if (output)
      stk_cout << _T("Testing B.resize(R,S); B=1\n");
    B.resize(R, S); B = Type(1);
    if (output)
    { stk_cout << _T("R=") << R << _T("\n");
      stk_cout << _T("S=") << S << _T("\n");
      print(B, "B");
    }
    //
    R.incLast(I.size()); S.decLast(J.size()/2);
    if (output)
      stk_cout << _T("R.incLast(I.size()); S.decLast(J.size()/2). Testing B.resize(R,S); B=3\n");
    B.resize(R, S); B = Type(3);
    if (output)
    { stk_cout << _T("R=") << R << _T("\n");
      stk_cout << _T("S=") << S << _T("\n");
      print(B, "B");
    }
    //
    R.incLast(I.size()); S.incLast(J.size());
    if (output)
      stk_cout << _T("R.incLast(I.size()); S.incLast(J.size()). Testing B.resize(R,S); B=2\n");
    B.resize(R, S); B = Type(2);
    if (output)
    { stk_cout << _T("R=") << R << _T("\n");
      stk_cout << _T("S=") << S << _T("\n");
      print(B, "B");
    }
    //
    R.decLast(I.size()); S.incLast(J.size());
    if (output)
      stk_cout << _T("R.decLast(I.size()); S.incLast(J.size()). Testing B.resize(R,S); B=4\n");
    B.resize(R, S); B = Type(4);
    if (output)
    { stk_cout << _T("R=") << R << _T("\n");
      stk_cout << _T("S=") << S << _T("\n");
      print(B, "B");
    }
    //
    if (output)
      stk_cout << _T("R.decLast(I.size()); S.decLast(J.size()); B.resize(R,S); B=5\n");
    R.decLast(I.size()); S.decLast(J.size());
    B.resize(R, S); B = Type(5);
    if (output)
    { stk_cout << _T("R=") << R << _T("\n");
      stk_cout << _T("S=") << S << _T("\n");
      print(B, "B");
    }
}


/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  try
  {
    int Istart, Jstart, Iend, Jend, output;
    if (argc < 4)
    {
      // 12 15 3 4 10 12 1 2
      Istart = 0;
      Jstart = 1;
      Iend   = 8;
      Jend   = 9;
      output = true;
    }
    else
    {
      // dimensions
      Istart = atoi(argv[1]);
      Jstart = atoi(argv[2]);
      Iend   = atoi(argv[3]);
      Jend   = atoi(argv[4]);
      // output
      output = true;
      if (argc >=5 ) output = atoi(argv[5]);
    }
    // ranges
    Range I(Istart, Iend, 0);
    Range J(Jstart, Jend, 0);
    
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Arrays                                    +\n");
    stk_cout << _T("+ Using: \n");
    stk_cout << _T(", I = ") << I << _T(", J = ") << J << _T("\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("TestArray2D<Real> : \n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    // time variables
    Chrono::start();
    TestArray2D<Real>(I, J, output);
    Real test_time = Chrono::elapsed();
    stk_cout << _T("\n");
    stk_cout << _T("Time used : ") << test_time << _T("\n");
    stk_cout << _T("\n\n");

    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("TestArray2D<int> : \n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    // time variables
    Chrono::start();
    TestArray2D<int>(I, J, output);
    test_time = Chrono::elapsed();
    stk_cout << _T("\n");
    stk_cout << _T("Time used : ") << test_time << _T("\n");
    stk_cout << _T("\n\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::Array2D   +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

  }
  catch (Exception const& error)
  {
    std::cerr << error.error() << _T("\n";);
  }

  return 0;
}
