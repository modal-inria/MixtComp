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

/** @file testVisitorsUnrolled.cpp
 *  @brief In this file we test the behavior of the visitors when they are unrolled.
 **/

#include "Arrays.h"

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
  int i,j;
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test all visitors                                   +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  Array2D<Real> general(5, 4);
  CArray<Real, 5, 4> cgeneral1;
  CArray<Real, 5, 4, Arrays::by_row_> cgeneral2;
  general << 1, 3, 4, 6
           , 1, 3, 4, 6
           , 1, 3, 4, 6
           , 1, 3, 4, 6
           , 1, 3, 4, 6;
  cgeneral1 << 1, 3, 4, 6
            , 1, 3, 4, 6
            , 1, 3, 4, 6
            , 1, 3, 4, 6
            , 1, 3, 4, 6;
  cgeneral2 << 1, 3, 4, 6
            , 1, 3, 4, 6
            , 1, 3, 4, 6
            , 1, 3, 4, 6
            , 1, 3, 4, 6;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for general and cgeneral arrays.\n");
    stk_cout << _T("general =\n") << general;
    stk_cout << _T("cgeneral1 =\n") << cgeneral1;
    stk_cout << _T("cgeneral1 =\n") << cgeneral2;
    stk_cout << _T("general.sum() =") << general.sum() << _T("\n");
    stk_cout << _T("cgeneral1.sum() =") << cgeneral1.sum() << _T("\n");
    stk_cout << _T("cgeneral2.sum() =") << cgeneral2.sum() << _T("\n");
    stk_cout << _T("general.maxElt(i,j) =") << general.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("cgeneral1.maxElt(i,j) =") << cgeneral1.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("cgeneral2.maxElt(i,j) =") << cgeneral2.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DSquare<Real> square(5);
  CArraySquare<Real, 5> csquare1;
  CArraySquare<Real, 5, Arrays::by_row_> csquare2;
  square  << 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9;
  csquare1  << 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9;
  csquare2  << 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for square arrays.\n");
    stk_cout << _T("square =\n") << square;
    stk_cout << _T("csquare1 =\n") << csquare1;
    stk_cout << _T("csquare2 =\n") << csquare2;
    stk_cout << _T("square.sum() =") << square.sum() << _T("\n");
    stk_cout << _T("csquare1.sum() =") << csquare1.sum() << _T("\n");
    stk_cout << _T("csquare2.sum() =") << csquare2.sum() << _T("\n");
    stk_cout << _T("square.maxElt(i,j) =") << square.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("csquare1.maxElt(i,j) =") << csquare1.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("csquare2.maxElt(i,j) =") << csquare2.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

//  Array2DDiagonal<Real> diagonal(5);
//  diagonal << 1, 3, 4, 6, 9;
//  try
//  {
//    stk_cout << _T("\n\n");
//    stk_cout << _T("Test visitors for diagonal arrays.\n");
//    stk_cout << _T("diagonal =\n") << diagonal;
//    stk_cout << _T("diagonal.sum() =") << diagonal.sum() << _T("\n");
//    stk_cout << _T("diagonal.maxElt(i) =") << diagonal.maxElt(i);
//    stk_cout << _T(" At position(") << i << j << _T(")\n");
//  }
//  catch (Exception const& error)
//  { stk_cerr << error.error() << _T("\n"); return -1;}
//
//  Array2DLowerTriangular<Real> lowTriangular(5, 4);
//  lowTriangular << 1
//                 , 1, 3
//                 , 1, 3, 4
//                 , 1, 3, 4, 6
//                 , 1, 3, 4, 6;
//  try
//  {
//    stk_cout << _T("\n\n");
//    stk_cout << _T("Test visitors for lowTriangular arrays.\n");
//    stk_cout << _T("lowTriangular =\n") << lowTriangular;
//    stk_cout << _T("lowTriangular.sum() =") << lowTriangular.sum() << _T("\n");
//    stk_cout << _T("lowTriangular.maxElt(i,j) =") << lowTriangular.maxElt(i,j);
//    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
//  }
//  catch (Exception const& error)
//  { stk_cerr << error.error() << _T("\n"); return -1;}
//
//  Array2DUpperTriangular<Real> upTriangular(5, 4);
//  upTriangular << 1, 3, 4, 6
//                   , 3, 4, 6
//                      , 4, 6
//                         , 6;
//  try
//  {
//    stk_cout << _T("\n\n");
//    stk_cout << _T("Test visitors for upTriangular arrays.\n");
//    stk_cout << _T("upTriangular =\n") << upTriangular;
//    stk_cout << _T("upTriangular.sum() =") << upTriangular.sum() << _T("\n");
//    stk_cout << _T("upTriangular.maxElt(i,j) =") << upTriangular.maxElt(i,j);
//    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
//  }
//  catch (Exception const& error)
//  { stk_cerr << error.error() << _T("\n"); return -1;}
//
  Array2DVector<Real> vector(5);
  CArrayVector<Real, 5> cvector(5);
  vector << 1, 3, 4, 6, 9;
  cvector << 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for vector arrays.\n");
    stk_cout << _T("vector =\n") << vector;
    stk_cout << _T("cvector =\n") << cvector;
    stk_cout << _T("vector.sum() =") << vector.sum() << _T("\n");
    stk_cout << _T("cvector.sum() =") << cvector.sum() << _T("\n");
    stk_cout << _T("vector.maxElt(i,j) =") << vector.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("cvector.maxElt(i,j) =") << cvector.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DPoint<Real> point(5);
  CArrayPoint<Real> cpoint(5);
  point << 1, 3, 4, 6, 9;
  cpoint << 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for point arrays.\n");
    stk_cout << _T("point =\n") << point;
    stk_cout << _T("cpoint =\n") << cpoint;
    stk_cout << _T("point.sum() =") << point.sum() << _T("\n");
    stk_cout << _T("cpoint.sum() =") << cpoint.sum() << _T("\n");
    stk_cout << _T("point.maxElt(i,j) =") << point.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("cpoint.maxElt(i,j) =") << cpoint.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
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



