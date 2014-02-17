/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::Arrays
 * created on: 21 June 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testBiProducts.cpp
 *  @brief In this file we test the behavior of the all bi-products.
 **/

#include "../../include/Arrays.h"

using namespace STK;

/** @ingroup Arrays
  * @brief compute the difference between two 2D arrays
  */
template <typename Lhs, typename Rhs>
Real diff2D(Lhs const& lhs, Rhs const& rhs)
{
  int i,j;
  return ((lhs * rhs)-mult(lhs, rhs)).abs().maxElt(i,j);
}

/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test all bi-products                               +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  Array2D<Real> lhsGeneral(100, 80), rhsGeneral(80, 200);
  Array2DSquare<Real> lhsSquare(80), rhsSquare(100);
  Array2DDiagonal<Real> lhsDiagonal(80), rhsDiagonal(80);
  Array2DLowerTriangular<Real> lhsLowTriangular(100, 80), rhsLowTriangular(80, 200);
  Array2DUpperTriangular<Real> lhsUpTriangular(100, 80), rhsUpTriangular(80, 100);
  Array2DVector<Real> lhsVector(100), rhsVector(100);
  Array2DPoint<Real> lhsPoint(100), rhsPoint(100);

  rhsGeneral.randGauss(); rhsDiagonal.randGauss(); rhsLowTriangular.randGauss();
  rhsPoint.randGauss(); rhsSquare.randGauss(); rhsUpTriangular.randGauss(); rhsVector.randGauss();
  // General lhs
  stk_cout << _T("\nGeneral Lhs.\n");
  lhsGeneral.randGauss();
  try
  {
    stk_cout << _T("General by General. diff =") << diff2D(lhsGeneral, rhsGeneral)  << _T("\n");
    stk_cout << _T("General by Diagonal. diff =") << diff2D(lhsGeneral, rhsDiagonal)  << _T("\n");
    stk_cout << _T("General by Square. diff =") << diff2D(lhsGeneral, rhsSquare.resize(80))  << _T("\n");
    stk_cout << _T("General by LowerTriangular. diff =") << diff2D(lhsGeneral, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("General by UpperTriangular. diff =") << diff2D(lhsGeneral, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("General by Point. diff =") << diff(lhsGeneral, rhsPoint)  << _T("\n"); // will not compile
    stk_cout << _T("General by Vector. diff =") << diff2D(lhsGeneral, rhsVector.resize(80))  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Square lhs
  stk_cout << _T("\nSquare Lhs.\n");
  lhsSquare.randGauss();
  try
  {
    stk_cout << _T("Square by General. diff =") << diff2D(lhsSquare, rhsGeneral)  << _T("\n");
    stk_cout << _T("Square by Diagonal. diff =") << diff2D(lhsSquare, rhsDiagonal)  << _T("\n");
    stk_cout << _T("Square by Square. diff =") << diff2D(lhsSquare, rhsSquare)  << _T("\n");
    stk_cout << _T("Square by LowerTriangular. diff =") << diff2D(lhsSquare, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("Square by UpperTriangular. diff =") << diff2D(lhsSquare, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("Square by Point. diff =") << diff2D(lhsSquare, rhsPoint)  << _T("\n"); // will not compile
    stk_cout << _T("Square by Vector. diff =") << diff2D(lhsSquare, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Diagonal lhs
  stk_cout << _T("\nDiagonal Lhs.\n");
  lhsDiagonal.randGauss();
  try
  {
    stk_cout << _T("Diagonal by General. diff =") << diff2D(lhsDiagonal, rhsGeneral)  << _T("\n");
    stk_cout << _T("Diagonal by Diagonal. diff =") << diff2D(lhsDiagonal, rhsDiagonal)  << _T("\n");
    stk_cout << _T("Diagonal by Square. diff =") << diff2D(lhsDiagonal, rhsSquare)  << _T("\n");
    stk_cout << _T("Diagonal by LowerTriangular. diff =") << diff2D(lhsDiagonal, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("Diagonal by UpperTriangular. diff =") << diff2D(lhsDiagonal, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("Diagonal by Point. diff =") << diff2D(lhsDiagonal, rhsPoint)  << _T("\n"); // will not compile
    stk_cout << _T("Diagonal by Vector. diff =") << diff2D(lhsDiagonal, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Diagonal lhs
  stk_cout << _T("\nLowTriangular Lhs.\n");
  lhsLowTriangular.randGauss();
  try
  {
    stk_cout << _T("LowTriangular by General. diff =") << diff2D(lhsLowTriangular, rhsGeneral)  << _T("\n");
    stk_cout << _T("LowTriangular by Diagonal. diff =") << diff2D(lhsLowTriangular, rhsDiagonal)  << _T("\n");
    stk_cout << _T("LowTriangular by Square. diff =") << diff2D(lhsLowTriangular, rhsSquare)  << _T("\n");
    stk_cout << _T("LowTriangular by LowerTriangular. diff =") << diff2D(lhsLowTriangular, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("LowTriangular by UpperTriangular. diff =") << diff2D(lhsLowTriangular, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("LowTriangular by Point. diff =") << diff2D(lhsDiagonal, rhsPoint)  << _T("\n"); // will not compile
    stk_cout << _T("LowTriangular by Vector. diff =") << diff2D(lhsLowTriangular, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // UpperTriangular lhs
  stk_cout << _T("\nUpperTriangular Lhs.\n");
  lhsUpTriangular.randGauss();
  try
  {
    stk_cout << _T("UpperTriangular by General. diff =") << diff2D(lhsUpTriangular, rhsGeneral)  << _T("\n");
    stk_cout << _T("UpperTriangular by Diagonal. diff =") << diff2D(lhsUpTriangular, rhsDiagonal)  << _T("\n");
    stk_cout << _T("UpperTriangular by Square. diff =") << diff2D(lhsUpTriangular, rhsSquare)  << _T("\n");
    stk_cout << _T("UpperTriangular by LowerTriangular. diff =") << diff2D(lhsUpTriangular, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("UpperTriangular by UpperTriangular. diff =") << diff2D(lhsUpTriangular, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("UpperTriangular by Point. diff =") << diff2D(lhsUpTriangular, rhsPoint)  << _T("\n"); // will not compile
    stk_cout << _T("UpperTriangular by Vector. diff =") << diff2D(lhsUpTriangular, rhsVector)  << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // vector lhs
  stk_cout << _T("\nVector Lhs.\n");
  lhsVector.randGauss();
  try
  {
    //stk_cout << _T("Vector by General. diff =") << diff2D(lhsVector, rhsGeneral)  << _T("\n");// will not compile
    //stk_cout << _T("Vector by Diagonal. diff =") << diff2D(lhsVector, rhsDiagonal)  << _T("\n");// will not compile
    //stk_cout << _T("Vector by Square. diff =") << diff2D(lhsVector, rhsSquare)  << _T("\n");// will not compile
    //stk_cout << _T("Vector by LowerTriangular. diff =") << diff2D(lhsVector, rhsLowTriangular)  << _T("\n");// will not compile
    //stk_cout << _T("Vector by UpperTriangular. diff =") << diff2D(lhsVector, rhsUpTriangular)  << _T("\n");// will not compile
    stk_cout << _T("Vector by Point. diff =") << diff2D(lhsVector, rhsPoint)  << _T("\n");
    //stk_cout << _T("Vector by Vector. diff =") << diff2D(lhsVector, rhsVector)  << _T("\n");// will not compile
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  // Point lhs
  stk_cout << _T("\nPoint Lhs.\n");
  lhsPoint.randGauss();
  try
  {
    stk_cout << _T("Point by General. diff =") << diff2D(lhsPoint.resize(80), rhsGeneral)  << _T("\n");
    stk_cout << _T("Point by Diagonal. diff =") << diff2D(lhsPoint, rhsDiagonal)  << _T("\n");
    stk_cout << _T("Point by Square. diff =") << diff2D(lhsPoint, rhsSquare)  << _T("\n");
    stk_cout << _T("Point by LowerTriangular. diff =") << diff2D(lhsPoint, rhsLowTriangular)  << _T("\n");
    stk_cout << _T("Point by UpperTriangular. diff =") << diff2D(lhsPoint, rhsUpTriangular)  << _T("\n");
    //stk_cout << _T("Point by Point. diff =") << diff2D(lhsPoint, rhsPoint)  << _T("\n"); // will not compile
    //stk_cout << _T("Point by Vector. diff =") << diff2D(lhsPoint, rhsVector)  << _T("\n");// will not compile
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}



  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for dot products   +\n");
  stk_cout << _T("+ No errors detected.                                 +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");


  stk_cout << _T("\n\n");
  return 0;
}



