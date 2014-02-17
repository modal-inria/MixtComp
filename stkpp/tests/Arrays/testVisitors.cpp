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

/** @file testVisitors.cpp
 *  @brief In this file we test the behavior of all the visitors.
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
  int i,j;
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test all visitors                                   +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  Array2D<Real> general(5, 4);
  general << 1, 3, 4, 6
           , 1, 3, 4, 6
           , 1, 3, 4, 6
           , 1, 3, 4, 6
           , 1, 3, 4, 6;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for general arrays.\n");
    stk_cout << _T("general =\n") << general;
    stk_cout << _T("general.sum() =") << general.sum() << _T("\n");
    stk_cout << _T("general.norm() =") << general.norm() << _T("\n");
    stk_cout << _T("general.mean() =") << general.mean() << _T("\n");
    stk_cout << _T("general.variance() =") << general.variance() << _T("\n");
    stk_cout << _T("general.maxElt(i,j) =") << general.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("general.maxElt() =") << general.maxElt() << _T("\n");
    stk_cout << _T("general.minElt(i,j) =") << general.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("general.minElt() =") << general.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for general arrays.\n");
    general(general.firstIdxRows(), general.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("general =\n") << general;
    stk_cout << _T("general.sumSafe() =") << general.sumSafe() << _T("\n");
    stk_cout << _T("general.normSafe() =") << general.normSafe() << _T("\n");
    stk_cout << _T("general.meanSafe() =") << general.meanSafe() << _T("\n");
    stk_cout << _T("general.varianceSafe() =") << general.varianceSafe() << _T("\n");
    stk_cout << _T("general.maxEltSafe(i,j) =") << general.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("general.maxEltSafe() =") << general.maxEltSafe() << _T("\n");
    stk_cout << _T("general.minEltSafe(i,j) =") << general.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("general.minEltSafe() =") << general.minEltSafe() << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DSquare<Real> square(5);
  square  << 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9
           , 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for general arrays.\n");
    stk_cout << _T("square =\n") << square;
    stk_cout << _T("square.sum() =") << square.sum() << _T("\n");
    stk_cout << _T("square.norm() =") << square.norm() << _T("\n");
    stk_cout << _T("square.mean() =") << square.mean() << _T("\n");
    stk_cout << _T("square.variance() =") << square.variance() << _T("\n");
    stk_cout << _T("square.maxElt(i,j) =") << square.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("square.maxElt() =") << square.maxElt() << _T("\n");
    stk_cout << _T("square.minElt(i,j) =") << square.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("square.minElt() =") << square.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for square arrays.\n");
    square(square.firstIdxRows(), square.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("square =\n") << square;
    stk_cout << _T("square.sumSafe() =") << square.sumSafe() << _T("\n");
    stk_cout << _T("square.normSafe() =") << square.normSafe() << _T("\n");
    stk_cout << _T("square.meanSafe() =") << square.meanSafe() << _T("\n");
    stk_cout << _T("square.varianceSafe() =") << square.varianceSafe() << _T("\n");
    stk_cout << _T("square.maxEltSafe(i,j) =") << square.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("square.maxEltSafe() =") << square.maxEltSafe() << _T("\n");
    stk_cout << _T("square.minEltSafe(i,j) =") << square.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("square.minEltSafe() =") << square.minEltSafe() << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DDiagonal<Real> diagonal(5);
  diagonal << 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for diagonal arrays.\n");
    stk_cout << _T("diagonal =\n") << diagonal;
    stk_cout << _T("diagonal.sum() =") << diagonal.sum() << _T("\n");
    stk_cout << _T("diagonal.norm() =") << diagonal.norm() << _T("\n");
    stk_cout << _T("diagonal.mean() =") << diagonal.mean() << _T("\n");
    stk_cout << _T("diagonal.variance() =") << diagonal.variance() << _T("\n");
    stk_cout << _T("diagonal.maxElt(i,j) =") << diagonal.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("diagonal.maxElt(i) =") << diagonal.maxElt(i);
    stk_cout << _T(" At position(") << i << j << _T(")\n");
    stk_cout << _T("diagonal.maxElt() =") << diagonal.maxElt() << _T("\n");
    stk_cout << _T("diagonal.minElt(i,j) =") << diagonal.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("diagonal.minElt() =") << diagonal.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for diagonal arrays.\n");
    diagonal(diagonal.firstIdxRows(), diagonal.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("diagonal =\n") << diagonal;
    stk_cout << _T("diagonal.sumSafe() =") << diagonal.sumSafe() << _T("\n");
    stk_cout << _T("diagonal.normSafe() =") << diagonal.normSafe() << _T("\n");
    stk_cout << _T("diagonal.meanSafe() =") << diagonal.meanSafe() << _T("\n");
    stk_cout << _T("diagonal.varianceSafe() =") << diagonal.varianceSafe() << _T("\n");
    stk_cout << _T("diagonal.maxEltSafe(i,j) =") << diagonal.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("diagonal.maxEltSafe(i) =") << diagonal.maxEltSafe(i);
    stk_cout << _T(" At position(") << i << _T(")\n");
    stk_cout << _T("diagonal.maxEltSafe() =") << diagonal.maxEltSafe() << _T("\n");
    stk_cout << _T("diagonal.minEltSafe(i,j) =") << diagonal.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("diagonal.minEltSafe() =") << diagonal.minEltSafe() << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DLowerTriangular<Real> lowTriangular(5, 4);
  lowTriangular << 1
                 , 1, 3
                 , 1, 3, 4
                 , 1, 3, 4, 6
                 , 1, 3, 4, 6;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for lowTriangular arrays.\n");
    stk_cout << _T("lowTriangular =\n") << lowTriangular;
    stk_cout << _T("lowTriangular.sum() =") << lowTriangular.sum() << _T("\n");
    stk_cout << _T("lowTriangular.norm() =") << lowTriangular.norm() << _T("\n");
    stk_cout << _T("lowTriangular.mean() =") << lowTriangular.mean() << _T("\n");
    stk_cout << _T("lowTriangular.variance() =") << lowTriangular.variance() << _T("\n");
    stk_cout << _T("lowTriangular.maxElt(i,j) =") << lowTriangular.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("lowTriangular.maxElt() =") << lowTriangular.maxElt() << _T("\n");
    stk_cout << _T("lowTriangular.minElt(i,j) =") << lowTriangular.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("lowTriangular.minElt() =") << lowTriangular.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for lowTriangular arrays.\n");
    lowTriangular(lowTriangular.firstIdxRows(), lowTriangular.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("lowTriangular =\n") << lowTriangular;
    stk_cout << _T("lowTriangular.sumSafe() =") << lowTriangular.sumSafe() << _T("\n");
    stk_cout << _T("lowTriangular.normSafe() =") << lowTriangular.normSafe() << _T("\n");
    stk_cout << _T("lowTriangular.meanSafe() =") << lowTriangular.meanSafe() << _T("\n");
    stk_cout << _T("lowTriangular.varianceSafe() =") << lowTriangular.varianceSafe() << _T("\n");
    stk_cout << _T("lowTriangular.maxEltSafe(i,j) =") << lowTriangular.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("lowTriangular.maxEltSafe() =") << lowTriangular.maxEltSafe() << _T("\n");
    stk_cout << _T("lowTriangular.minEltSafe(i,j) =") << lowTriangular.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("lowTriangular.minEltSafe() =") << lowTriangular.minEltSafe() << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DUpperTriangular<Real> upTriangular(5, 4);
  upTriangular << 1, 3, 4, 6
                   , 3, 4, 6
                      , 4, 6
                         , 6;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for upTriangular arrays.\n");
    stk_cout << _T("upTriangular =\n") << upTriangular;
    stk_cout << _T("upTriangular.sum() =") << upTriangular.sum() << _T("\n");
    stk_cout << _T("upTriangular.norm() =") << upTriangular.norm() << _T("\n");
    stk_cout << _T("upTriangular.mean() =") << upTriangular.mean() << _T("\n");
    stk_cout << _T("upTriangular.variance() =") << upTriangular.variance() << _T("\n");
    stk_cout << _T("upTriangular.maxElt(i,j) =") << upTriangular.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("upTriangular.maxElt() =") << upTriangular.maxElt() << _T("\n");
    stk_cout << _T("upTriangular.minElt(i,j) =") << upTriangular.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("upTriangular.minElt() =") << upTriangular.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for upTriangular arrays.\n");
    upTriangular(upTriangular.firstIdxRows(), upTriangular.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("upTriangular =\n") << upTriangular;
    stk_cout << _T("upTriangular.sumSafe() =") << upTriangular.sumSafe() << _T("\n");
    stk_cout << _T("upTriangular.normSafe() =") << upTriangular.normSafe() << _T("\n");
    stk_cout << _T("upTriangular.meanSafe() =") << upTriangular.meanSafe() << _T("\n");
    stk_cout << _T("upTriangular.varianceSafe() =") << upTriangular.varianceSafe() << _T("\n");
    stk_cout << _T("upTriangular.maxEltSafe(i,j) =") << upTriangular.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("upTriangular.maxEltSafe() =") << upTriangular.maxEltSafe() << _T("\n");
    stk_cout << _T("upTriangular.minEltSafe(i,j) =") << upTriangular.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("upTriangular.minEltSafe() =") << upTriangular.minEltSafe() << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DVector<Real> vector(5);
  vector << 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for vector arrays.\n");
    stk_cout << _T("vector =\n") << vector;
    stk_cout << _T("vector.sum() =") << vector.sum() << _T("\n");
    stk_cout << _T("vector.norm() =") << vector.norm() << _T("\n");
    stk_cout << _T("vector.mean() =") << vector.mean() << _T("\n");
    stk_cout << _T("vector.variance() =") << vector.variance() << _T("\n");
    stk_cout << _T("vector.maxElt(i,j) =") << vector.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("vector.maxElt(i) =") << vector.maxElt(i);
    stk_cout << _T(" At position(") << i << j << _T(")\n");
    stk_cout << _T("vector.maxElt() =") << vector.maxElt() << _T("\n");
    stk_cout << _T("vector.minElt(i,j) =") << vector.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("vector.minElt() =") << vector.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for vector arrays.\n");
    vector(vector.firstIdxRows(), vector.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("vector =\n") << vector;
    stk_cout << _T("vector.sumSafe() =") << vector.sumSafe() << _T("\n");
    stk_cout << _T("vector.normSafe() =") << vector.normSafe() << _T("\n");
    stk_cout << _T("vector.meanSafe() =") << vector.meanSafe() << _T("\n");
    stk_cout << _T("vector.varianceSafe() =") << vector.varianceSafe() << _T("\n");
    stk_cout << _T("vector.maxEltSafe(i,j) =") << vector.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("vector.maxEltSafe(i) =") << vector.maxEltSafe(i);
    stk_cout << _T(" At position(") << i << _T(")\n");
    stk_cout << _T("vector.maxEltSafe() =") << vector.maxEltSafe() << _T("\n");
    stk_cout << _T("vector.minEltSafe(i,j) =") << vector.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("vector.minEltSafe() =") << vector.minEltSafe() << _T("\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n"); return -1;}

  Array2DPoint<Real> point(5);
  point << 1, 3, 4, 6, 9;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("Test visitors for point arrays.\n");
    stk_cout << _T("point =\n") << point;
    stk_cout << _T("point.sum() =") << point.sum() << _T("\n");
    stk_cout << _T("point.norm() =") << point.norm() << _T("\n");
    stk_cout << _T("point.mean() =") << point.mean() << _T("\n");
    stk_cout << _T("point.variance() =") << point.variance() << _T("\n");
    stk_cout << _T("point.maxElt(i,j) =") << point.maxElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("point.maxElt(i) =") << point.maxElt(i);
    stk_cout << _T(" At position(") << i << j << _T(")\n");
    stk_cout << _T("point.maxElt() =") << point.maxElt() << _T("\n");
    stk_cout << _T("point.minElt(i,j) =") << point.minElt(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("point.minElt() =") << point.minElt() << _T("\n");
    stk_cout << _T("\nTest safe visitors for point arrays.\n");
    point(point.firstIdxRows(), point.firstIdxCols()) = Arithmetic<Real>::NA();
    stk_cout << _T("point =\n") << point;
    stk_cout << _T("point.sumSafe() =") << point.sumSafe() << _T("\n");
    stk_cout << _T("point.normSafe() =") << point.normSafe() << _T("\n");
    stk_cout << _T("point.meanSafe() =") << point.meanSafe() << _T("\n");
    stk_cout << _T("point.varianceSafe() =") << point.varianceSafe() << _T("\n");
    stk_cout << _T("point.maxEltSafe(i,j) =") << point.maxEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("point.maxEltSafe(i) =") << point.maxEltSafe(i);
    stk_cout << _T(" At position(") << i << _T(")\n");
    stk_cout << _T("point.maxEltSafe() =") << point.maxEltSafe() << _T("\n");
    stk_cout << _T("point.minEltSafe(i,j) =") << point.minEltSafe(i,j);
    stk_cout << _T(" At position(") << i <<_T(",") << j << _T(")\n");
    stk_cout << _T("point.minEltSafe() =") << point.minEltSafe() << _T("\n");
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



