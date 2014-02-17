/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published + the Free Software Foundation; either version 2 of the
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
 * Project:  stkpp::Arrays
 * created on: 21 June 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testBiSum.cpp
 *  @brief In this file we test the behavior of the all bi-sum operators.
 **/

#include "../../include/Arrays.h"

using namespace STK;

/** @ingroup Arrays
  * @brief compute the difference between two 2D arrays
  */
template <typename Lhs, typename Rhs>
Real sum2D(Lhs const& lhs, Rhs const& rhs)
{ return ((lhs + rhs)-sum(lhs, rhs)).abs().maxElt();}

/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test all bi-sums                               +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  Array2D<Real> lhsGeneral(80, 80), rhsGeneral(80, 80);
  Array2DSquare<Real> lhsSquare(80), rhsSquare(80);
  Array2DDiagonal<Real> lhsDiagonal(80), rhsDiagonal(80);
  Array2DLowerTriangular<Real> lhsLowTriangular(80, 80), rhsLowTriangular(80, 80);
  Array2DUpperTriangular<Real> lhsUpTriangular(80, 80), rhsUpTriangular(80, 80);
  Array2DVector<Real> lhsVector(80), rhsVector(80);
  Array2DPoint<Real> lhsPoint(80), rhsPoint(80);

  rhsGeneral.randGauss(); rhsDiagonal.randGauss(); rhsLowTriangular.randGauss();
  rhsPoint.randGauss(); rhsSquare.randGauss(); rhsUpTriangular.randGauss(); rhsVector.randGauss();
  // General lhs
  stk_cout << _T("\nGeneral Lhs.\n");
  lhsGeneral.randGauss();
  try
  {
    stk_cout << _T("General + General. diff =") << sum2D(lhsGeneral, rhsGeneral)  << _T("\n");
    stk_cout << _T("General + Diagonal. diff =") << sum2D(lhsGeneral, rhsDiagonal)  << _T("\n");
    stk_cout << _T("General + Square. diff =") << sum2D(lhsGeneral, rhsSquare.resize(80))  << _T("\n");
    stk_cout << _T("General + LowerTriangular. diff =") << sum2D(lhsGeneral, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("General + UpperTriangular. diff =") << sum2D(lhsGeneral, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("General + Point. diff =") << sum2D(lhsGeneral, rhsPoint)  << _T("\n"); // will not compile
    //stk_cout << _T("General + Vector. diff =") << sum2D(lhsGeneral, rhsVector.resize(80))  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Square lhs
  stk_cout << _T("\nSquare Lhs.\n");
  lhsSquare.randGauss();
  try
  {
    stk_cout << _T("Square + General. diff =") << sum2D(lhsSquare, rhsGeneral)  << _T("\n");
    stk_cout << _T("Square + Diagonal. diff =") << sum2D(lhsSquare, rhsDiagonal)  << _T("\n");
    stk_cout << _T("Square + Square. diff =") << sum2D(lhsSquare, rhsSquare)  << _T("\n");
    stk_cout << _T("Square + LowerTriangular. diff =") << sum2D(lhsSquare, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("Square + UpperTriangular. diff =") << sum2D(lhsSquare, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("Square + Point. diff =") << sum2D(lhsSquare, rhsPoint)  << _T("\n"); // will not compile
    //stk_cout << _T("Square + Vector. diff =") << sum2D(lhsSquare, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Diagonal lhs
  stk_cout << _T("\nDiagonal Lhs.\n");
  lhsDiagonal.randGauss();
  try
  {
    stk_cout << _T("Diagonal + General. diff =") << sum2D(lhsDiagonal, rhsGeneral)  << _T("\n");
    stk_cout << _T("Diagonal + Diagonal. diff =") << sum2D(lhsDiagonal, rhsDiagonal)  << _T("\n");
    stk_cout << _T("Diagonal + Square. diff =") << sum2D(lhsDiagonal, rhsSquare)  << _T("\n");
    stk_cout << _T("Diagonal + LowerTriangular. diff =") << sum2D(lhsDiagonal, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("Diagonal + UpperTriangular. diff =") << sum2D(lhsDiagonal, rhsUpTriangular)  << _T("\n");
//    stk_cout << _T("Diagonal + Point. diff =") << sum2D(lhsDiagonal, rhsPoint)  << _T("\n"); // will not compile
//    stk_cout << _T("Diagonal + Vector. diff =") << sum2D(lhsDiagonal, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Diagonal lhs
  stk_cout << _T("\nLowTriangular Lhs.\n");
  lhsLowTriangular.randGauss();
  try
  {
    stk_cout << _T("LowTriangular + General. diff =") << sum2D(lhsLowTriangular, rhsGeneral)  << _T("\n");
    stk_cout << _T("LowTriangular + Diagonal. diff =") << sum2D(lhsLowTriangular, rhsDiagonal)  << _T("\n");
    stk_cout << _T("LowTriangular + Square. diff =") << sum2D(lhsLowTriangular, rhsSquare)  << _T("\n");
    stk_cout << _T("LowTriangular + LowerTriangular. diff =") << sum2D(lhsLowTriangular, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("LowTriangular + UpperTriangular. diff =") << sum2D(lhsLowTriangular, rhsUpTriangular)  << _T("\n");
//    stk_cout << _T("LowTriangular + Point. diff =") << sum2D(lhsDiagonal, rhsPoint)  << _T("\n"); // will not compile
//    stk_cout << _T("LowTriangular + Vector. diff =") << sum2D(lhsLowTriangular, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // UpperTriangular lhs
  stk_cout << _T("\nUpperTriangular Lhs.\n");
  lhsUpTriangular.randGauss();
  try
  {
    stk_cout << _T("UpperTriangular + General. diff =") << sum2D(lhsUpTriangular, rhsGeneral)  << _T("\n");
    stk_cout << _T("UpperTriangular + Diagonal. diff =") << sum2D(lhsUpTriangular, rhsDiagonal)  << _T("\n");
    stk_cout << _T("UpperTriangular + Square. diff =") << sum2D(lhsUpTriangular, rhsSquare)  << _T("\n");
    stk_cout << _T("UpperTriangular + LowerTriangular. diff =") << sum2D(lhsUpTriangular, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("UpperTriangular + UpperTriangular. diff =") << sum2D(lhsUpTriangular, rhsUpTriangular)  << _T("\n");
//    stk_cout << _T("UpperTriangular + Point. diff =") << sum2D(lhsUpTriangular, rhsPoint)  << _T("\n"); // will not compile
//    stk_cout << _T("UpperTriangular + Vector. diff =") << sum2D(lhsUpTriangular, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

//  // vector lhs
//  stk_cout << _T("\nVector Lhs.\n");
//  lhsVector.randGauss();
//  try
//  {
//    stk_cout << _T("Vector + General. diff =") << sum2D(lhsVector, rhsGeneral)  << _T("\n");// will not compile
//    stk_cout << _T("Vector + Diagonal. diff =") << sum2D(lhsVector, rhsDiagonal)  << _T("\n");// will not compile
//    stk_cout << _T("Vector + Square. diff =") << sum2D(lhsVector, rhsSquare)  << _T("\n");// will not compile
//    stk_cout << _T("Vector + LowerTriangular. diff =") << sum2D(lhsVector, rhsLowTriangular)  << _T("\n");// will not compile
//    stk_cout << _T("Vector + UpperTriangular. diff =") << sum2D(lhsVector, rhsUpTriangular)  << _T("\n");// will not compile
//    stk_cout << _T("Vector + Point. diff =") << sum2D(lhsVector, rhsPoint)  << _T("\n");
//    stk_cout << _T("Vector + Vector. diff =") << sum2D(lhsVector, rhsVector)  << _T("\n");// will not compile
//  }
//  catch (Exception const& error)
//  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Point lhs
//  stk_cout << _T("\nPoint Lhs.\n");
//  lhsPoint.randGauss();
//  try
//  {
//    stk_cout << _T("Point + General. diff =") << sum2D(lhsPoint.resize(80), rhsGeneral)  << _T("\n");
//    stk_cout << _T("Point + Diagonal. diff =") << sum2D(lhsPoint, rhsDiagonal)  << _T("\n");
//    stk_cout << _T("Point + Square. diff =") << sum2D(lhsPoint, rhsSquare)  << _T("\n");
//    stk_cout << _T("Point + LowerTriangular. diff =") << sum2D(lhsPoint, rhsLowTriangular)  << _T("\n");
//    stk_cout << _T("Point + UpperTriangular. diff =") << sum2D(lhsPoint, rhsUpTriangular)  << _T("\n");
//    stk_cout << _T("Point + Point. diff =") << sum2D(lhsPoint, rhsPoint)  << _T("\n"); // will not compile
//    stk_cout << _T("Point + Vector. diff =") << sum2D(lhsPoint, rhsVector)  << _T("\n");// will not compile
//  }
//  catch (Exception const& error)
//  { stk_cerr << error.error() << _T("\n"); return -1;}



  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for all bi sums    +\n");
  stk_cout << _T("+ No errors detected.                                 +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");


  stk_cout << _T("\n\n");
  return 0;
}



