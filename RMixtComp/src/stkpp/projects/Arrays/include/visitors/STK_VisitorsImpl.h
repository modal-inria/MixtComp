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

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::Arrays
 * created on: 27 sept. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_VisitorsImpl.h
 *  @brief In this file we implement the visitors
 **/

#ifndef STK_VISITORSIMPL_H
#define STK_VISITORSIMPL_H

namespace STK
{

namespace hidden
{
// forward declarations
template<typename Visitor, typename Derived, bool orient_, int SizeRows_, int SizeCols_>
struct GenVisitor;
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VectorVisitor;
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
      VectorVisitor<Visitor, Derived, Unroll1_, UnknownSize>::run(matrix, visitor, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = 1; j <= matrix.lastIdxCols(); ++j)
      VectorVisitor<Visitor, Derived, Unroll1_, UnknownSize>::apply(matrix, visitor, j);
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
    VectorVisitor<Visitor, Derived, Unroll1_, Unroll2_>::run(matrix, visitor);
    GenVisitor<Visitor, Derived, Arrays::by_col_, Unroll1_, Unroll2_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    VectorVisitor<Visitor, Derived, Unroll1_, Unroll2_>::apply(matrix, visitor);
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
struct VectorVisitor
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, Unroll2_), Unroll1_, Unroll2_);
    VectorVisitor<Visitor, Derived, Unroll1_-1, Unroll2_>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    visitor(matrix.elt(Unroll1_, Unroll2_));
    VectorVisitor<Visitor, Derived, Unroll1_-1, Unroll2_>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll2_>
struct VectorVisitor<Visitor, Derived, 1, Unroll2_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, Unroll2_), 1, Unroll2_);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(1, Unroll2_));}
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int Unroll1_>
struct VectorVisitor<Visitor, Derived, Unroll1_, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor, int col)
  {
    VectorVisitor<Visitor, Derived, Unroll1_-1, UnknownSize>::run(matrix, visitor, col);
    visitor(matrix.elt(Unroll1_, col), Unroll1_, col);
  }
  inline static void apply( Derived& matrix, Visitor& visitor, int col)
  {
    VectorVisitor<Visitor, Derived, Unroll1_-1, UnknownSize>::apply(matrix, visitor, col);
    visitor(matrix.elt(Unroll1_, col));
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived>
struct VectorVisitor<Visitor, Derived, 1, UnknownSize>
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
// forward declaration
template<typename Visitor, typename Derived, int orient_> struct Visitor2D;
// forward declaration
template<typename Visitor, typename Derived, int orient_> struct VisitorUpper;
// forward declaration
template<typename Visitor, typename Derived, int orient_> struct VisitorLower;


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

} // namespace STK

#endif /* STK_VISITORSIMPL_H */
