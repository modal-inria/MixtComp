/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * created on: 27 sept. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_ArrayBaseVisitor.h
 *  @brief In this file we define the ArrayBaseVisitor classes.
 **/

#ifndef STK_ARRAYBASEVISITOR_H
#define STK_ARRAYBASEVISITOR_H

#include "../../STKernel/include/STK_MetaTemplate.h"
#include "STK_Arrays_Util.h"
#include "../../STatistiK/include/STK_RandBase.h"

namespace STK
{

namespace hidden
{
// forward declarations
template<typename Visitor, typename Derived, bool Orient_, int SizeRows_, int SizeCols_>
struct GenVisitor;
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct ColVisitor;
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct RowVisitor;
/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows
 *  and we don't unroll the columns. */
template<typename Visitor, typename Derived, int Unroll2_>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, UnknownSize, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = matrix.lastIdxRows(); i >= 1; --i) visitor(matrix.elt(i, Unroll2_), i, Unroll2_);
    GenVisitor<Visitor, Derived, Arrays::by_col_, UnknownSize, Unroll2_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = matrix.lastIdxRows(); i >= 1; --i) visitor(matrix.elt(i, Unroll2_));
    GenVisitor<Visitor, Derived, Arrays::by_col_, UnknownSize, Unroll2_-1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with 1 row (Point), column oriented
 *  arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, UnknownSize, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { for(int i = matrix.lastIdxRows(); i >= 1; --i) visitor(matrix.elt(i, 1), i, 1);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { for(int i = matrix.lastIdxRows(); i >= 1; --i)  visitor(matrix.elt(i, 1));}
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows
 *  and we don't unroll the columns. */
template<typename Visitor, typename Derived, int Unroll2_>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, UnknownSize, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = 1; i <= matrix.lastIdxRows(); ++i)
      RowVisitor<Visitor, Derived, UnknownSize, Unroll2_>::run(matrix, visitor, i);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = 1; i <= matrix.lastIdxRows(); ++i)
      RowVisitor<Visitor, Derived, UnknownSize, Unroll2_>::apply(matrix, visitor, i);
  }
};

/** @ingroup hidden
 *  @brief specialization for 1 column (Vector), row oriented arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, UnknownSize, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = 1; i <= matrix.lastIdxRows(); ++i)
      visitor(matrix.elt(i, 1), i, 1);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = 1; i <= matrix.lastIdxRows(); ++i)
      visitor(matrix.elt(i, 1));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows
 *  and we don't unroll the columns. */
template<typename Visitor, typename Derived, int Unroll1_>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      ColVisitor<Visitor, Derived, Unroll1_, UnknownSize>::run(matrix, visitor, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      ColVisitor<Visitor, Derived, Unroll1_, UnknownSize>::apply(matrix, visitor, j);
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with 1 row (Point), column oriented
 *  arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, 1, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      visitor(matrix.elt(1, j), 1, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      visitor(matrix.elt(1, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows
 *  and we don't unroll the columns. */
template<typename Visitor, typename Derived, int Unroll1_>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= 1; --j)
      visitor(matrix.elt(Unroll1_, j), Unroll1_, j);
    GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_-1, UnknownSize>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= 1; --j)
      visitor(matrix.elt(Unroll1_, j));
    GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_-1, UnknownSize>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for 1 row (Point), row oriented arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, 1, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= 1; --j)
      visitor(matrix.elt(1, j), 1, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= 1; --j)
      visitor(matrix.elt(1, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows and the
 *  and the columns with a column oriented arrays */
template<typename Visitor, typename Derived, int Unroll1_ , int Unroll2_>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    ColVisitor<Visitor, Derived, Unroll1_, Unroll2_>::run(matrix, visitor);
    GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_, Unroll2_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    ColVisitor<Visitor, Derived, Unroll1_, Unroll2_>::apply(matrix, visitor);
    GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_, Unroll2_-1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows and the
 *  columns with a row oriented arrays */
template<typename Visitor, typename Derived, int Unroll1_ , int Unroll2_>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_, Unroll2_>
{
    inline static void run( Derived const& matrix, Visitor& visitor)
    {
      RowVisitor<Visitor, Derived, Unroll1_, Unroll2_>::run(matrix, visitor);
      GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_-1, Unroll2_>::run(matrix, visitor);
    }
    inline static void apply( Derived& matrix, Visitor& visitor)
    {
      RowVisitor<Visitor, Derived, Unroll1_, Unroll2_>::apply(matrix, visitor);
      GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_-1, Unroll2_>::apply(matrix, visitor);
    }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll2_>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, 1, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(1, Unroll2_), 1, Unroll2_);
    GenVisitor<Visitor, Derived, Arrays::by_col_, 1, Unroll2_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(1, Unroll2_));
    GenVisitor<Visitor, Derived, Arrays::by_col_, 1, Unroll2_-1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 column (Vector) */
template<typename Visitor, typename Derived, int Unroll2_>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, 1, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(1, Unroll2_), 1, Unroll2_);
    GenVisitor<Visitor, Derived, Arrays::by_row_, 1, Unroll2_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(1, Unroll2_));
    GenVisitor<Visitor, Derived, Arrays::by_row_, 1, Unroll2_-1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the Arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll1_>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, 1), Unroll1_, 1);
    GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_-1, 1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, 1));
    GenVisitor<Visitor, Derived, Arrays::by_row_, Unroll1_-1, 1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the Arrays with 1 row (Point)  */
template<typename Visitor, typename Derived, int Unroll1_>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, 1), Unroll1_, 1);
    GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_-1, 1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, 1));
    GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_-1, 1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with 1 row (Point)a nd 1 column
 *  arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, 1, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, 1), 1, 1);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, 1));}
};

/** @ingroup hidden
 *  @brief specialization for the general case with 1 row (Point)a nd 1 column
 *  arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, 1, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, 1), 1, 1);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, 1));}
};
/*
 * General case
 */

/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_col_, UnknownSize, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      for(int i = 1; i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      for(int i = 1; i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with row oriented arrays */
template<typename Visitor, typename Derived>
struct GenVisitor<Visitor, Derived, Arrays::by_row_, UnknownSize, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = 1; i <= matrix.lastIdxRows(); ++i)
      for(int j = 1; j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = 1; i <= matrix.lastIdxRows(); ++i)
      for(int j = 1; j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll1_, int Unroll2_>
struct ColVisitor
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, Unroll2_), Unroll1_, Unroll2_);
    ColVisitor<Visitor, Derived, Unroll1_-1, Unroll2_>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, Unroll2_));
    ColVisitor<Visitor, Derived, Unroll1_-1, Unroll2_>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll2_>
struct ColVisitor<Visitor, Derived, 1, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, Unroll2_), 1, Unroll2_);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, Unroll2_));}
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll1_>
struct ColVisitor<Visitor, Derived, Unroll1_, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor, int col)
  {
    ColVisitor<Visitor, Derived, Unroll1_-1, UnknownSize>::run(matrix, visitor, col);
    visitor(matrix.elt(Unroll1_, col), Unroll1_, col);
  }
  inline static void apply( Derived& matrix, Visitor& visitor, int col)
  {
    ColVisitor<Visitor, Derived, Unroll1_-1, UnknownSize>::apply(matrix, visitor, col);
    visitor(matrix.elt(Unroll1_, col));
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived>
struct ColVisitor<Visitor, Derived, 1, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor, int col)
  { visitor(matrix.elt(1, col), 1, col);}
  inline static void apply( Derived& matrix, Visitor& visitor, int col)
  { visitor(matrix.elt(1, col));}
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll1_, int Unroll2_>
struct RowVisitor
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, Unroll2_), Unroll1_, Unroll2_);
    RowVisitor<Visitor, Derived, Unroll1_, Unroll2_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, Unroll2_));
    RowVisitor<Visitor, Derived, Unroll1_, Unroll2_-1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 column */
template<typename Visitor, typename Derived, int Unroll1_>
struct RowVisitor<Visitor, Derived, Unroll1_, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(Unroll1_, 1), Unroll1_, 1);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(Unroll1_, 1));}
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll2_>
struct RowVisitor<Visitor, Derived, UnknownSize, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor, int row)
  {
    RowVisitor<Visitor, Derived, UnknownSize, Unroll2_-1>::run(matrix, visitor, row);
    visitor(matrix.elt(row, Unroll2_), row, Unroll2_);
  }
  inline static void apply( Derived& matrix, Visitor& visitor, int row)
  {
    RowVisitor<Visitor, Derived, UnknownSize, Unroll2_-1>::apply(matrix, visitor, row);
    visitor(matrix.elt(row, Unroll2_));
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 column */
template<typename Visitor, typename Derived>
struct RowVisitor<Visitor, Derived, UnknownSize, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor, int row)
  { visitor(matrix.elt(row, 1), row, 1);}
  inline static void apply( Derived& matrix, Visitor& visitor, int row)
  { visitor(matrix.elt(row, 1));}
};

} // namespace hidden


namespace hidden
{
template<typename Visitor, typename Derived, bool Orient_>
struct Visitor2D;
template<typename Visitor, typename Derived, bool Orient_>
struct VisitorUpper;
template<typename Visitor, typename Derived, bool Orient_>
struct VisitorLower;
template<typename Visitor, typename Derived>
struct VisitorDiagonal;
template<typename Visitor, typename Derived>
struct VisitorVector;
template<typename Visitor, typename Derived>
struct VisitorPoint;


/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorDiagonal
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j), j);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j));}
};

/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorVector
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j), j);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j));}
};

/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorPoint
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j), j);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j));}
};

/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct Visitor2D<Visitor, Derived, Arrays::by_col_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.firstIdxCols(); j <= matrix.lastIdxCols(); ++j)
      for(int i = matrix.firstIdxRows(); i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.firstIdxCols(); j <= matrix.lastIdxCols(); ++j)
      for(int i = matrix.firstIdxRows(); i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with row oriented arrays */
template<typename Visitor, typename Derived>
struct Visitor2D<Visitor, Derived, Arrays::by_row_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = matrix.firstIdxRows(); i <= matrix.lastIdxRows(); ++i)
      for(int j = matrix.firstIdxCols(); j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = matrix.firstIdxRows(); i <= matrix.lastIdxRows(); ++i)
      for(int j = matrix.firstIdxCols(); j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorUpper<Visitor, Derived, Arrays::by_col_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= matrix.firstIdxCols(); --j)
      for(int i = std::min(j, matrix.lastIdxRows()); i >= matrix.firstIdxRows(); --i)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= matrix.firstIdxCols(); --j)
      for(int i = std::min(j, matrix.lastIdxRows()); i >= matrix.firstIdxRows(); --i)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with row oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorUpper<Visitor, Derived, Arrays::by_row_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = matrix.firstIdxRows(); i <= matrix.lastIdxRows(); ++i)
      for(int j = std::max(i, matrix.firstIdxCols()); j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = matrix.firstIdxRows(); i <= matrix.lastIdxRows(); ++i)
      for(int j = std::max(i, matrix.firstIdxCols()); j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j));
  }
};


/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorLower<Visitor, Derived, Arrays::by_col_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.firstIdxCols(); j <= matrix.lastIdxCols(); ++j)
      for(int i = std::max(j, matrix.firstIdxRows()); i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.firstIdxCols(); j <= matrix.lastIdxCols(); ++j)
      for(int i = std::max(j, matrix.firstIdxRows()); i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with row oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorLower<Visitor, Derived, Arrays::by_row_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = matrix.lastIdxRows(); i >= matrix.firstIdxRows(); --i)
      for(int j = std::min(i, matrix.lastIdxCols()); j >= matrix.firstIdxCols(); --j)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = matrix.lastIdxRows(); i >= matrix.firstIdxRows(); --i)
      for(int j = std::min(i, matrix.lastIdxCols()); j >= matrix.firstIdxCols(); --j)
        visitor(matrix.elt(i, j));
  }
};

} // namespace hidden


namespace hidden
{
/** @ingroup hidden
  * @brief Base class to implement min, max, sum,... visitors for 2D containers.
  */
template <typename Type>
struct EltVisitor2DBase
{
  EltVisitor2DBase() : row_(STKBASEARRAYS), col_(STKBASEARRAYS), res_(Arithmetic<Type>::NA()) {};
  int row_, col_;
  Type res_;
};

/** @ingroup hidden
  * @brief Base class to implement min, max, sum,... visitors for 1D containers.
  */
template <typename Type>
struct EltVisitor1DBase
{
  EltVisitor1DBase() : idx_(STKBASEARRAYS), res_(Arithmetic<Type>::NA()) {};
  int idx_;
  Type res_;
};

/** @ingroup hidden
  * \brief Visitor computing the min coefficient with its value and coordinates
  *
  * @sa ArrayBase::minElt(int, int)
  */
template <typename Type>
struct MinEltVisitor : EltVisitor2DBase<Type>
{
  MinEltVisitor() : EltVisitor2DBase<Type>()
  { this->res_ = Arithmetic<Type>::max(); }
  void operator()( Type const& value, int i, int j)
  {
    if(value < this->res_)
    { this->res_ = value; this->row_ = i; this->col_ = j;}
  }
};

/** @ingroup hidden
  * \brief Visitor computing the min coefficient with its value and coordinates
  *
  * @sa ArrayBase::minElt(int, int)
  */
template <typename Type>
struct MinEltVisitor1D : EltVisitor1DBase<Type>
{
  MinEltVisitor1D() : EltVisitor1DBase<Type>()
  { this->res_ = Arithmetic<Type>::max(); }

  void operator()( Type const& value, int i)
  {
    if(value < this->res_)
    { this->res_ = value; this->idx_ = i;}
  }
};

/** @ingroup hidden
 *  @brief Visitor computing the maximal coefficient of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct MaxEltVisitor : EltVisitor2DBase<Type>
{
  MaxEltVisitor(): EltVisitor2DBase<Type>() { this->res_ = -Arithmetic<Type>::max(); }
  void operator() ( Type const& value, int i, int j)
  {
    if(value > this->res_)
    { this->res_ = value; this->row_ = i; this->col_ = j;}
  }
};
/** @ingroup hidden
 *  @brief Visitor computing the maximal coefficient of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct MaxEltVisitor1D : EltVisitor1DBase<Type>
{
  MaxEltVisitor1D(): EltVisitor1DBase<Type>() { this->res_ = -Arithmetic<Type>::max(); }
  void operator() ( Type const& value, int i)
  {
    if(value > this->res_)
    { this->res_ = value; this->idx_ = i;}
  }
};

/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct SumVisitor
{
  Type res_;
  SumVisitor(): res_(Type(0)) {}
  void operator() ( Type const& value, int i, int j)
  { res_ += value;}
  void operator() ( Type const& value, int i)
  { res_ += value;}
};

/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct RandUnifVisitor
{
  RandBase ran;
  RandUnifVisitor() : ran() {}
  void operator() ( Type& value)
  { value = Type(ran.randUnif());}
};

/** @ingroup hidden
 *  @brief Visitor computing the sum of all the coefficients of the Array
 *
 * @sa ArrayBase::maxElt(int, int)
 */
template <typename Type>
struct RandGaussVisitor
{
  RandBase ran;
  RandGaussVisitor() : ran() {}
  void operator() ( Type& value)
  { value = Type(ran.randGauss());}
};

/** @ingroup hidden
  * @brief visitor putting a constant value
  */
template <typename Type>
struct ValueVisitor
{
  Type value_;
  ValueVisitor(Type const& value) : value_(value) {};
  inline void operator() ( Type& value)
  { value = value_;}
};


} //namespace hidden

namespace hidden
{

// forward declaration
//template<typename Visitor, typename Derived, Arrays::Structure Structure_, int SizeRows_, int SizeCols_>
//struct VisitorType;

/** @ingroup hidden
 *  @brief general visitor */
template<typename Visitor, typename Derived, Arrays::Structure Structure_, int SizeRows_, int SizeCols_>
struct VisitorType
{ typedef Visitor2D<Visitor, Derived, hidden::Traits<Derived>::orient_> Type;};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorType<Visitor, Derived, Arrays::diagonal_, SizeRows_, SizeCols_>
{ typedef VisitorDiagonal<Visitor, Derived> Type;};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorType<Visitor, Derived, Arrays::vector_, SizeRows_, SizeCols_>
{ typedef VisitorVector<Visitor, Derived> Type;};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorType<Visitor, Derived, Arrays::point_, SizeRows_, SizeCols_>
{ typedef VisitorPoint<Visitor, Derived> Type;};

/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorType<Visitor, Derived, Arrays::upper_triangular_, SizeRows_, SizeCols_>
{ typedef VisitorUpper<Visitor, Derived, hidden::Traits<Derived>::orient_> Type;};
/** @ingroup hidden
 *  @brief specialization for the lower triangular arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorType<Visitor, Derived, Arrays::lower_triangular_, SizeRows_, SizeCols_>
{ typedef VisitorLower<Visitor, Derived, hidden::Traits<Derived>::orient_> Type;};

}




/** Run the visitor @a visitor to the whole coefficients of the array.
  *
  * The template parameter @a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor {
  *   // called for all  coefficients
  *   void operator() (Type const& value, Index i, Index j);
  * };
  * @endcode
  *
  * @note visitors offer automatic unrolling for small fixed size matrix.
  *
  * @sa minElt, maxElt
  */
template<typename Derived>
template<typename Visitor>
void ArrayBase<Derived>::visit(Visitor& visitor) const
{
//  if (!this->asDerived().isRef() || ((this->firstIdxRows()==1)&&(this->firstIdxCols()==1)))
//  {
//    Derived& ARef = const_cast<Derived&> (this->asDerived());
//    const int firstRow = ARef.firstIdxRows(), firstCol = ARef.firstIdxCols();
//    ARef.shift(1, 1);
//    enum { unrollrow_ = (sizeRows_ < MaxUnrollSquareRoot),
//           unrollcol_ = (sizeCols_ < MaxUnrollSquareRoot)
//         };
//    hidden::GenVisitor< Visitor, Derived, orient_
//                            , (unrollrow_) ? int(sizeRows_) : UnknownSize
//                            , (unrollcol_) ? int(sizeCols_) : UnknownSize
//                        >::run(ARef, visitor);
//    ARef.shift(firstRow, firstCol);
//  }
//  else // just loop over the array using the orientation order
//  { hidden::Visitor2D< Visitor, Derived, orient_>::run(this->asDerived(), visitor);}
  enum {// structure_ = structure_,
         unrollrow_ = (sizeRows_ < MaxUnrollSquareRoot),
         unrollcol_ = (sizeCols_ < MaxUnrollSquareRoot)
       };
  typedef typename hidden::VisitorType<Visitor, Derived, (Arrays::Structure)structure_, sizeRows_, sizeCols_>::Type Impl;
  Impl::run(this->asDerived(), visitor);
}

/** Apply the Visitor @a visitor to the whole coefficients of the array.
  *
  * The template parameter @a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor {
  *   // called for all  coefficients
  *   void operator() (Type& value);
  * };
  * @endcode
  *
  * @note visitors offer automatic unrolling for small fixed size matrix.
  *
  * @sa setValue
  */
template<typename Derived>
template<typename Visitor>
void ArrayBase<Derived>::visit(Visitor& visitor)
{
//  if (!this->asDerived().isRef() || ((this->firstIdxRows()==1)&&(this->firstIdxCols()==1)))
//  {
//    const int firstRow = this->firstIdxRows(), firstCol = this->firstIdxCols();
//    if (!this->asDerived().isRef()) this->asDerived().shift(1, 1);
//    enum { unrollrow_ = (sizeRows_ < MaxUnrollSquareRoot),
//           unrollcol_ = (sizeCols_ < MaxUnrollSquareRoot)
//         };
//    hidden::GenVisitor< Visitor, Derived, orient_
//                           , (unrollrow_) ? int(sizeRows_) : UnknownSize
//                           , (unrollcol_) ? int(sizeCols_) : UnknownSize
//                        >::apply(this->asDerived(), visitor);
//    if (!this->asDerived().isRef()) this->asDerived().shift(firstRow, firstCol);
//  }
//  else
//  {
//    hidden::Visitor2D< Visitor, Derived, orient_>::apply(this->asDerived(), visitor);
//  }
  typedef typename hidden::VisitorType<Visitor, Derived, (Arrays::Structure)structure_, sizeRows_, sizeCols_>::Type Impl;
  Impl::apply(this->asDerived(), visitor);
}

/*
 * start public function of the ArrayBase class using visitor
 */
/* @brief Set the value to all the Allocator */
template<typename Derived>
void ArrayBase<Derived>::setValue(Type const& value)
{
    hidden::ValueVisitor<Type> visitor(value);
    this->visit(visitor);
}

/* @brief Set the value one to all the Array */
template<typename Derived>
void ArrayBase<Derived>::ones()
{
  hidden::ValueVisitor<Type> visitor(Type(1));
  this->visit(visitor);
}

/* @brief Set the value one to all the Array */
template<typename Derived>
void ArrayBase<Derived>::zeros()
{
  hidden::ValueVisitor<Type> visitor(Type(0));
  visit(visitor);
}

template<typename Derived>
void ArrayBase<Derived>::randUnif()
{
    hidden::RandUnifVisitor<Type> visitor;
    visit(visitor);
}

template<typename Derived>
void ArrayBase<Derived>::randGauss()
{
    hidden::RandGaussVisitor<Type> visitor;
    this->visit(visitor);
}



/* @brief Set the value one to all the Array */
template<typename Derived>
typename hidden::Traits<Derived>::Type ArrayBase<Derived>::sum() const
{
  hidden::SumVisitor<Type> visitor;
  this->visit(visitor);
  return visitor.res_;
}

template<typename Derived>
typename hidden::Traits<Derived>::Type ArrayBase<Derived>::minElt( int& row, int& col) const
{
  hidden::MinEltVisitor<Type> visitor;
  this->visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}

template<typename Derived>
typename hidden::Traits<Derived>::Type ArrayBase<Derived>::minElt( int& idx) const
{
  hidden::MinEltVisitor1D<Type> visitor;
  this->visit(visitor);
  idx = visitor.idx_;
  return visitor.res_;
}

template<typename Derived>
typename hidden::Traits<Derived>::Type ArrayBase<Derived>::maxElt( int& row, int& col) const
{
  hidden::MaxEltVisitor<Type> visitor;
  this->visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type ArrayBase<Derived>::maxElt( int& idx) const
{
  hidden::MaxEltVisitor1D<Type> visitor;
  this->visit(visitor);
  idx = visitor.idx_;
  return visitor.res_;
}

/** @ingroup Arrays
 *  @brief Applies the visitor \a Visitor to the whole elements of the
 *  matrix or vector but don't modify the Array.
  * The template parameter \a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor
  * {
  *   // called for all elements
  *   void operator() (const Type& value, int i, int j);
  * };
  * @endcode
  *
  * @note compared to one or two @c for loops, visitors offer automatic
  * unrolling for small fixed size matrix.
  */
template< typename Derived, typename Visitor>
class ConstArrayBaseVisitor
{
  private:
    Derived const& array_;
  public:
    typedef typename hidden::VisitorType< Visitor, Derived
                                        , (Arrays::Structure)hidden::Traits<Derived>::structure_
                                        , hidden::Traits<Derived>::sizeRows_
                                        , hidden::Traits<Derived>::sizeCols_>::Type Impl;

    ConstArrayBaseVisitor( Derived const& T) : array_(T.asDerived()) { }
    ~ConstArrayBaseVisitor() { }
    inline void visit(Visitor& funct) const { Impl::run(array_, funct);}
};

/** @ingroup Arrays
 *  @brief Applies the visitor \a visitor to the whole elements of the matrix or
 *  vector.
  *
  * The template parameter \a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor {
  *   // called for all elements
  *   void operator() (Type& value, int i, int j);
  * };
  * struct MyVisitor1D {
  *   // called for all elements
  *   void operator() (Type& value, int j);
  * };
  * @endcode
  * The value is modified by the Visitor.
  *
  * @note compared to one or two @c for loops, visitors offer automatic
  * unrolling for small fixed size matrix.
  */
template< typename Derived, typename Visitor>
class ArrayBaseVisitor
{
  private:
    Derived& array_;
  public:
    typedef typename hidden::VisitorType< Visitor, Derived
                                        , (Arrays::Structure)hidden::Traits<Derived>::structure_
                                        , hidden::Traits<Derived>::sizeRows_
                                        , hidden::Traits<Derived>::sizeCols_>::Type Impl;

    ArrayBaseVisitor( Derived& T) : array_(T) { }
    ~ArrayBaseVisitor() { }
    inline void visit(Visitor& funct) { Impl::apply(array_, funct);}
};

/* @return the minimum of all elements of matrix
  * and puts in row and col its location.
  *
  * @sa  maxElt(Derived const&, int&, int&), minElt()
  */
template<typename Derived>
typename hidden::Traits<Derived>::Type minElt(Derived const& matrix, int& row, int& col)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  hidden::MinEltVisitor<Type> visitor;
  ConstArrayBaseVisitor<Derived, hidden::MinEltVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return (visitor.res_);
}

/* @returns the maximum of all elements of matrix
  * and puts in row and col its location.
  *
  * @sa ArrayBase::minElt(int), ArrayBase::maxElt(int,int), ArrayBase::visitor(), ArrayBase::minElt()
  */
template<typename Derived>
typename hidden::Traits<Derived>::Type maxElt(Derived const& matrix, int& row, int& col)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  hidden::MaxEltVisitor<Type> visitor;
  ConstArrayBaseVisitor<Derived, hidden::MaxEltVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}

/* @return the minimum of all elements of *this
  * and puts in index its location.
  *
  * @sa minElt(int,int), ArrayBase::maxElt(int,int), ArrayBase::visitor(), ArrayBase::minElt()
  */
template<typename Derived>
typename hidden::Traits<Derived>::Type minElt(Derived const& tab, int& index)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  hidden::MinEltVisitor1D<Type> visitor;
  ConstArrayBaseVisitor<Derived, hidden::MinEltVisitor1D<Type> > arrayVisitor(tab);
  arrayVisitor.visit(visitor);
  index = visitor.idx_;
  return visitor.res_;
}

/* @return the maximum of all elements of tab
  * and puts in index its location.
  *
  * @sa ArrayBase::maxElt(int&,int&), ArrayBase::minElt(int&,int&)
  * , ArrayBase::visitor(), ArrayBase::maxElt()
  */
template<typename Derived>
typename hidden::Traits<Derived>::Type maxElt(Derived const& tab, int& index)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  /* assert nn row or nb column is 1*/
  hidden::MaxEltVisitor1D<Type> visitor;
  ConstArrayBaseVisitor<Derived, hidden::MaxEltVisitor1D<Type> > arrayVisitor(tab);
  arrayVisitor.visit(visitor);
  index = visitor.idx_;
  return visitor.res_;
}

} // namespace STK

#endif /* STK_ARRAYBASEVISITOR_H */
