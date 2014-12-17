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
 *  @brief In this file we implement the different visitors allowing unrolling
 *  of the visit
 **/

#ifndef STK_VISITORSIMPL_H
#define STK_VISITORSIMPL_H

#define Idx(size) baseIdx + size - 1

namespace STK
{

namespace hidden
{
// forward declarations, for arrays
template<typename Visitor, typename Derived, bool orient_, int SizeRows_, int SizeCols_>
struct VisitorArrayImpl;
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorUnrollCol;
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorUnrollRow;

// other kind of structure
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorVectorImpl;
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorPointImpl;
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorDiagonalImpl;
template<typename Visitor, typename Derived, int orient_>
struct VisitorUpperImpl;
template<typename Visitor, typename Derived, int orient_>
struct VisitorLowerImpl;


/** @ingroup hidden
 *  @brief Specialization for general 2D arrays, data stored by column and
 *  dimensions not known at compile time. */
template<typename Visitor, typename Derived>
struct VisitorArrayImpl<Visitor, Derived, Arrays::by_col_, UnknownSize, UnknownSize>
{
  inline static void run( Derived const& expr, Visitor& visitor)
  {
    for(int j = expr.beginCols(); j < expr.endCols(); ++j)
      for(int i = expr.beginRows(); i < expr.endRows(); ++i)
        visitor(expr.elt(i, j), i, j);
  }
  inline static void apply( Derived& array, Visitor& visitor)
  {
    for(int j = array.beginCols(); j < array.endCols(); ++j)
      for(int i = array.beginRows(); i < array.endRows(); ++i)
        visitor(array.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief Specialization for general 2D arrays, data stored by rows and
 *  dimensions are not known at compile time.*/
template<typename Visitor, typename Derived>
struct VisitorArrayImpl<Visitor, Derived, Arrays::by_row_, UnknownSize, UnknownSize>
{
  inline static void run( Derived const& expr, Visitor& visitor)
  {
    for(int i = expr.beginRows(); i < expr.endRows(); ++i)
      for(int j = expr.beginCols(); j < expr.endCols(); ++j)
        visitor(expr.elt(i, j), i, j);
  }
  inline static void apply( Derived& array, Visitor& visitor)
  {
    for(int i = array.beginRows(); i < array.endRows(); ++i)
      for(int j = array.beginCols(); j < array.endCols(); ++j)
        visitor(array.elt(i, j));
  }
};

/** @ingroup hidden
 *  Specialization of VisitorArrayImpl when the number of rows is less than
 *  MaxUnrollSquareRoot and the number of column is unknown or greater
 *  than MaxUnrollSquareRoot
 */
template<typename Visitor, typename Derived, int Orient_, int SizeRows_>
struct VisitorArrayImpl< Visitor, Derived, Orient_, SizeRows_, UnknownSize>
{
  inline static void run( Derived const& expr, Visitor& visitor)
  {
    VisitorArrayImpl<Visitor, Derived, Orient_, SizeRows_ -1, UnknownSize>::run(expr, visitor);
    for(int j = expr.beginCols(); j < expr.endCols(); ++j)
      visitor(expr.elt(Idx(SizeRows_), j), Idx(SizeRows_), j);
  }
  inline static void apply( Derived& array, Visitor& visitor)
  {
    VisitorArrayImpl<Visitor, Derived, Orient_, SizeRows_ -1, UnknownSize>::apply(array, visitor);
    for(int j = array.beginCols(); j < array.endCols(); ++j)
      visitor(array.elt(Idx(SizeRows_), j));
  }
};

/** @ingroup hidden
 *  @brief Specialization of VisitorArrayImpl when the number of rows is 1
 *  and the number of column is unknown or greater than MaxUnrollSquareRoot.
 *  This class will stop the unrolling of on the rows.
 **/
template<typename Visitor, typename Derived, int Orient_>
struct VisitorArrayImpl< Visitor, Derived, Orient_, 1, UnknownSize>
{
  inline static void run( Derived const& expr, Visitor& visitor)
  {
    for(int j = expr.beginCols(); j < expr.endCols(); ++j)
      visitor(expr.elt(baseIdx, j), baseIdx, j);
  }
  inline static void apply( Derived& array, Visitor& visitor)
  {
    for(int j = array.beginCols(); j < array.endCols(); ++j)
      visitor(array.elt(baseIdx, j));
  }
};

/** @ingroup hidden
 *  Specialization of VisitorArrayImpl when the number of columns is less than
 *  MaxUnrollSquareRoot and the number of rows is unknown or greater
 *  thanMaxUnrollSquareRoot
 */
template<typename Visitor, typename Derived, int Orient_, int SizeCols_>
struct VisitorArrayImpl< Visitor, Derived, Orient_, UnknownSize, SizeCols_>
{
  inline static void run( Derived const& expr, Visitor& visitor)
  {
    VisitorArrayImpl<Visitor, Derived, Orient_, UnknownSize, SizeCols_-1>::run(expr, visitor);
    for(int i = expr.beginRows(); i < expr.endRows(); ++i)
      visitor(expr.elt(i, Idx(SizeCols_)), i, Idx(SizeCols_));
  }
  inline static void apply( Derived& array, Visitor& visitor)
  {
    VisitorArrayImpl<Visitor, Derived, Orient_, SizeCols_ -1, UnknownSize>::apply(array, visitor);
    for(int i = array.beginRows(); i < array.endRows(); ++i)
      visitor(array.elt(i, Idx(SizeCols_)));
  }
};

/** @ingroup hidden
 *  Specialization of VisitorArrayImpl when the number of columns is less than
 *  MaxUnrollSquareRoot and the number of rows is unknown or greater
 *  thanMaxUnrollSquareRoot
 */
template<typename Visitor, typename Derived, int Orient_>
struct VisitorArrayImpl< Visitor, Derived, Orient_, UnknownSize, 1>
{
  inline static void run( Derived const& expr, Visitor& visitor)
  {
    for(int i = expr.beginRows(); i < expr.endRows(); ++i)
      visitor(expr.elt(i, baseIdx), i, baseIdx);
  }
  inline static void apply( Derived& array, Visitor& visitor)
  {
    for(int i = array.beginRows(); i < array.endRows(); ++i)
      visitor(array.elt(i, baseIdx));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows and the
 *  and the columns with a column oriented arrays */
template<typename Visitor, typename Derived, int SizeRows_ , int SizeCols_>
struct VisitorArrayImpl<Visitor, Derived, Arrays::by_col_, SizeRows_, SizeCols_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    // this will unroll the column baseIdx + SizeCols_ -1
    VisitorUnrollCol<Visitor, Derived, SizeRows_, SizeCols_>::run(matrix, visitor);
    // do the same for column
    VisitorArrayImpl<Visitor, Derived, Arrays::by_col_, SizeRows_, SizeCols_-1>::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    VisitorUnrollCol<Visitor, Derived, SizeRows_, SizeCols_>::apply(matrix, visitor);
    VisitorArrayImpl<Visitor, Derived, Arrays::by_col_, SizeRows_, SizeCols_-1>::apply(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the Arrays with 1 column.
 *  In this case, just unroll the column. */
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorArrayImpl<Visitor, Derived, Arrays::by_col_, SizeRows_, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { VisitorUnrollCol<Visitor, Derived, SizeRows_, 1>::run(matrix, visitor);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { VisitorUnrollCol<Visitor, Derived, SizeRows_, 1>::apply(matrix, visitor);}
};

/** @ingroup hidden
 *  @brief specialization for the general case when we unroll the rows and the
 *  columns with a row oriented arrays */
template<typename Visitor, typename Derived, int SizeRows_ , int SizeCols_>
struct VisitorArrayImpl<Visitor, Derived, Arrays::by_row_, SizeRows_, SizeCols_>
{
    inline static void run( Derived const& matrix, Visitor& visitor)
    {
      // this will unroll the current row
      VisitorUnrollRow<Visitor, Derived, SizeRows_, SizeCols_>::run(matrix, visitor);
      // this will unroll the current row
      VisitorArrayImpl<Visitor, Derived, Arrays::by_row_, SizeRows_-1, SizeCols_>::run(matrix, visitor);
    }
    inline static void apply( Derived& matrix, Visitor& visitor)
    {
      VisitorUnrollRow<Visitor, Derived, SizeRows_, SizeCols_>::apply(matrix, visitor);
      VisitorArrayImpl<Visitor, Derived, Arrays::by_row_, SizeRows_-1, SizeCols_>::apply(matrix, visitor);
    }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 column (Vector) */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorArrayImpl<Visitor, Derived, Arrays::by_row_, 1, SizeCols_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { VisitorUnrollRow<Visitor, Derived, 1, SizeCols_>::run(matrix, visitor);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { VisitorUnrollRow<Visitor, Derived, 1, SizeCols_>::apply(matrix, visitor);}
};

/** @ingroup hidden
 *  @brief specialization for the general case with 1 row and 1 column arrays.
 *  This specialization allow to disambiguate instantiation.
 **/
template<typename Visitor, typename Derived, int Orient_>
struct VisitorArrayImpl<Visitor, Derived, Orient_, 1, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(baseIdx, baseIdx), baseIdx, baseIdx);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(baseIdx, baseIdx));}
};

/** @ingroup hidden
 *  @brief unroll the column baseIdx + SizeCols_ -1 */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorUnrollCol
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    VisitorUnrollCol<Visitor, Derived, SizeRows_-1, SizeCols_>::run(matrix, visitor);
    visitor(matrix.elt(Idx(SizeRows_), Idx(SizeCols_)), Idx(SizeRows_), Idx(SizeCols_));
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    VisitorUnrollCol<Visitor, Derived, SizeRows_-1, SizeCols_>::apply(matrix, visitor);
    visitor(matrix.elt(Idx(SizeRows_), Idx(SizeCols_)));
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 row (Point) */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorUnrollCol<Visitor, Derived, 1, SizeCols_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(baseIdx, Idx(SizeCols_)), baseIdx, Idx(SizeCols_));}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(baseIdx, Idx(SizeCols_)));}
};

/** @ingroup hidden
 *  @brief  unroll the row baseIdx + SizeRows_ -1  */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorUnrollRow
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    VisitorUnrollRow<Visitor, Derived, SizeRows_, SizeCols_-1>::run(matrix, visitor);
    visitor(matrix.elt(Idx(SizeRows_), Idx(SizeCols_)), Idx(SizeRows_), Idx(SizeCols_));
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    VisitorUnrollRow<Visitor, Derived, SizeRows_, SizeCols_-1>::apply(matrix, visitor);
    visitor(matrix.elt(Idx(SizeRows_), Idx(SizeCols_)));
  }
};

/** @ingroup hidden
 *  @brief specialization for the arrays with 1 column */
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorUnrollRow<Visitor, Derived, SizeRows_, 1>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(Idx(SizeRows_), baseIdx), Idx(SizeRows_), baseIdx);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt(Idx(SizeRows_), baseIdx));}
};

/** @ingroup hidden
 *  @brief Specialization when the size is unknown */
template<typename Visitor, typename Derived>
struct VisitorVectorImpl<Visitor, Derived, UnknownSize>
{
  inline static void run( Derived const& vect, Visitor& visitor)
  { for(int i = vect.begin(); i < vect.end(); ++i) visitor(vect.elt(i), i, vect.colIdx());}
  inline static void apply( Derived& array, Visitor& applier)
  { for(int i = array.begin(); i < array.end(); ++i) applier(array.elt(i));}
};

/** @ingroup hidden
 *  @brief A visitor Vector allow to unroll the visit of a vector if possible
 **/
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorVectorImpl
{
  inline static void run( Derived const& vect, Visitor& visitor)
  {
    VisitorVectorImpl<Visitor, Derived, SizeRows_-1>::run(vect, visitor);
    visitor(vect.elt(Idx(SizeRows_), vect.colIdx()), Idx(SizeRows_), vect.colIdx());
  }
  inline static void apply( Derived& array, Visitor& applier)
  {
    VisitorVectorImpl<Visitor, Derived, SizeRows_-1>::apply(array, applier);
    applier(array.elt(Idx(SizeRows_), array.colIdx()));
  }
};

/** @ingroup hidden
 *  @brief specialization for the vectors with 1 row */
template<typename Visitor, typename Derived>
struct VisitorVectorImpl<Visitor, Derived, 1>
{
  inline static void run( Derived const& vect, Visitor& visitor)
  { visitor(vect.elt(baseIdx), baseIdx, vect.colIdx());}
  inline static void apply( Derived& array, Visitor& applier)
  { applier(array.elt(baseIdx));}
};

/** @ingroup hidden
 *  @brief Specialization when the size is unknown */
template<typename Visitor, typename Derived>
struct VisitorPointImpl<Visitor, Derived, UnknownSize>
{
  inline static void run( Derived const& row, Visitor& visitor)
  { for(int j = row.begin(); j < row.end(); ++j) visitor(row.elt(j), row.rowIdx(),j);}
  inline static void apply( Derived& row, Visitor& visitor)
  { for(int j = row.begin(); j < row.end(); ++j) visitor(row.elt(j));}
};


/** @ingroup hidden
 *  @brief A VisitorPointImpl allow to unroll the visit of a row-vector if possible
 *  */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorPointImpl
{
  inline static void run( Derived const& row, Visitor& visitor)
  {
    VisitorPointImpl<Visitor, Derived, SizeCols_-1>::run(row, visitor);
    visitor(row.elt(Idx(SizeCols_)), row.rowIdx(), Idx(SizeCols_));
  }
  inline static void apply( Derived& row, Visitor& visitor)
  {
    VisitorPointImpl<Visitor, Derived, SizeCols_-1>::apply(row, visitor);
    visitor(row.elt(Idx(SizeCols_)));
  }
};

/** @ingroup hidden
 *  @brief specialization for the RowVectors with one columns */
template<typename Visitor, typename Derived>
struct VisitorPointImpl<Visitor, Derived, 1>
{
  inline static void run( Derived const& row, Visitor& visitor)
  { visitor(row.elt(baseIdx), baseIdx, row.rowIdx());}
  inline static void apply( Derived& row, Visitor& visitor)
  { visitor(row.elt(baseIdx));}
};

/** @ingroup hidden
 *  @brief Specialization when the size is unknown */
template<typename Visitor, typename Derived>
struct VisitorDiagonalImpl<Visitor, Derived, UnknownSize>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { for(int j = matrix.begin(); j < matrix.end(); ++j) visitor(matrix.elt(j), j,j);}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { for(int j = matrix.begin(); j < matrix.end(); ++j) visitor(matrix.elt(j));}
};

/** @ingroup hidden
 *  @brief A VisitorDiagonalImpl allow to unroll the visit of a Diagonal matrix if possible
 *  */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorDiagonalImpl
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    VisitorPointImpl<Visitor, Derived, SizeCols_-1>::run(matrix, visitor);
    visitor(matrix.elt(Idx(SizeCols_)), Idx(SizeCols_), Idx(SizeCols_));
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    VisitorPointImpl<Visitor, Derived, SizeCols_-1>::apply(matrix, visitor);
    visitor(matrix.elt(Idx(SizeCols_)));
  }
};

/** @ingroup hidden
 *  @brief specialization for the Diagonal matrix of size one */
template<typename Visitor, typename Derived>
struct VisitorDiagonalImpl<Visitor, Derived, 1>
{
  inline static void run( Derived const& row, Visitor& visitor)
  { visitor(row.elt(baseIdx), baseIdx, baseIdx);}
  inline static void apply( Derived& row, Visitor& visitor)
  { visitor(row.elt(baseIdx));}
};

/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorUpperImpl<Visitor, Derived, Arrays::by_col_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= matrix.beginCols(); --j)
      for(int i = std::min(j, matrix.lastIdxRows()); i >= matrix.beginRows(); --i)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.lastIdxCols(); j >= matrix.beginCols(); --j)
      for(int i = std::min(j, matrix.lastIdxRows()); i >= matrix.beginRows(); --i)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with row oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorUpperImpl<Visitor, Derived, Arrays::by_row_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = matrix.beginRows(); i <= matrix.lastIdxRows(); ++i)
      for(int j = std::max(i, matrix.beginCols()); j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = matrix.beginRows(); i <= matrix.lastIdxRows(); ++i)
      for(int j = std::max(i, matrix.beginCols()); j <= matrix.lastIdxCols(); ++j)
        visitor(matrix.elt(i, j));
  }
};


/** @ingroup hidden
 *  @brief specialization for the general case with column oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorLowerImpl<Visitor, Derived, Arrays::by_col_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int j = matrix.beginCols(); j <= matrix.lastIdxCols(); ++j)
      for(int i = std::max(j, matrix.beginRows()); i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int j = matrix.beginCols(); j <= matrix.lastIdxCols(); ++j)
      for(int i = std::max(j, matrix.beginRows()); i <= matrix.lastIdxRows(); ++i)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with row oriented arrays */
template<typename Visitor, typename Derived>
struct VisitorLowerImpl<Visitor, Derived, Arrays::by_row_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    for(int i = matrix.lastIdxRows(); i >= matrix.beginRows(); --i)
      for(int j = std::min(i, matrix.lastIdxCols()); j >= matrix.beginCols(); --j)
        visitor(matrix.elt(i, j), i, j);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    for(int i = matrix.lastIdxRows(); i >= matrix.beginRows(); --i)
      for(int j = std::min(i, matrix.lastIdxCols()); j >= matrix.beginCols(); --j)
        visitor(matrix.elt(i, j));
  }
};

/** @ingroup hidden
 *  @brief specialization for the general case with 1 row and 1 column arrays.
 *  This specialization allow to disambiguate instantiation.
 **/
template<typename Visitor, typename Derived>
struct VisitorNumberImpl
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { visitor(matrix.elt(), matrix.beginRows(), matrix.beginCols());}
  inline static void apply( Derived& matrix, Visitor& visitor)
  { visitor(matrix.elt());}
};


} // namespace hidden

} // namespace STK

#undef Idx

#endif /* STK_VISITORSIMPL_H */
