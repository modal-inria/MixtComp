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
 * Project:  stkpp::hidden::TContainer
 * Purpose:  Define the interface 2D Container classes.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IContainer2D.h
 *  @brief This is an internal header file, included by other
 *  Containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class like Array2D, except if you want to create your own
 *  Container Class.
 **/

#ifndef STK_ICONTAINER2D_H
#define STK_ICONTAINER2D_H

#include "../../STKernel/include/STK_Range.h"

namespace STK
{
template<int SizeRows_ = UnknownSize, int SizeCols_ = UnknownSize>
class TContainer2D;

/** @ingroup Arrays
 *  typedef for backward compatibility when the number of rows and columns is
 *  unknown */
typedef TContainer2D<UnknownSize, UnknownSize> IContainer2D;
/** @ingroup Arrays
 *  @brief Interface class for 2D containers.
 *
 *  The TContainer2D class is the base class for all two-dimensional containers.
 *  A two-dimensional container is defined by an horizontal range of index
 *  for the columns and a vectical range of index for the rows.
 **/
template<int SizeRows_, int SizeCols_>
class TContainer2D
{
  public:
    /** Type of the Range for the rows */
    typedef TRange<SizeRows_> RowRange;
    /** Type of the Range for the columns */
    typedef TRange<SizeCols_> ColRange;

  protected:
    /** Default constructor. cols_ = 1:0 and rows_ = 1:0. */
    inline TContainer2D() : rows_(), cols_() {}
    /** Constructor with specified ranges
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline TContainer2D( RowRange const& I, ColRange const& J) : rows_(I), cols_(J) {}
    /** Copy constructor
     *  @param T the container to copy
     **/
    inline TContainer2D( TContainer2D const& T) : rows_(T.rows_), cols_(T.cols_) {}
    /** destructor. **/
    inline ~TContainer2D() {}

  public:
    /** @return the index of the first column */
    inline int const& firstIdxCols() const { return cols_.firstIdx();}
    /** @return the index of the last column */
    inline int lastIdxCols() const { return cols_.lastIdx();}
    /** @return the number of column */
    inline int sizeColsImpl() const { return cols_.size();}
    /** @return the range of the columns */
    inline Range cols() const { return  Range(firstIdxCols(), sizeColsImpl());}
    /** @return the index of the first row */
    inline int const& firstIdxRows() const { return rows_.firstIdx();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return rows_.lastIdx();}
    /** @return the number of rows */
    inline int sizeRowsImpl() const { return rows_.size();}
    /** @return the range of the rows */
    inline Range rows() const { return Range(firstIdxRows(), sizeRowsImpl());}
    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return (cols_.empty() || rows_.empty());}

  protected:
    /** Set the first index of the rows and columns.
     *  @param rbeg the first index of the rows
     *  @param cbeg the first index of the columns
     **/
    inline void shift( int const& rbeg, int const& cbeg)
    { rows_.shift(rbeg); cols_.shift(cbeg);}
    /** Set the ranges of the container.
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline void setRanges(RowRange const& I = RowRange(), ColRange const& J = ColRange())
    { rows_ = I; cols_ =J;}
    /** Set the range of the number of rows.
     *  @param I the range of the rows number
     **/
    inline void setRows( RowRange const& I = RowRange()) { rows_ = I;}
    /** Set the first index of the rows.
     *  @param beg the first index of the rows
     **/
    inline void shiftFirstIdxRows( int const& beg) { rows_.shift(beg);}
    /** Increment the range of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incRangeRows( int const& inc) { rows_.inc(inc);}
    /** Increment the first index of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incFirstIdxRows( int const& inc) { rows_.incFirst(inc);}
    /** Decrement the first index of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decFirstIdxRows( int const& dec) { rows_.decFirst(dec);}
    /** Increment the end of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incLastIdxRows( int const& inc) { rows_.incLast(inc);}
    /** Decrement the end of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decLastIdxRows( int const& dec) { rows_.decLast(dec);}
    /** Set the range of the columns.
     * @param J the range of the cols number
     **/
    inline void setCols( ColRange const& J = ColRange()) { cols_ = J;}
    /** Shift the first index of the columns to beg.
     *  @param beg the new first index
     **/
    inline void shiftFirstIdxCols( int const& beg) { cols_.shift(beg);}
    /** Increment the range of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incRangeCols( int const& inc) { cols_.inc(inc);}
    /** increment the first index of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incFirstIdxCols( int const& inc) { cols_.incFirst(inc);}
    /** Decrement the first index of the columns.
     *  @param dec the decrement to apply
     **/
    inline void decFirstIdxCols( int const& dec) { cols_.decFirst(dec);}
    /** Increment the last index of the columns.
     *  @param inc the increment to apply
     **/
    inline void incLastIdxCols( int const& inc)  { cols_.incLast(inc);}
    /** Decrement the last index of the columns.
     *  @param dec the decrement to apply
     **/
    inline void decLastIdxCols( int const& dec) { cols_.decLast(dec);}
    /** exchange this container with T
     * @param T the container to exchange with this
     **/
     inline void exchange(TContainer2D& T)
     {
       std::swap(T.rows_, this->rows_ );
       std::swap(T.cols_, this->cols_ );
     }
  private:
    /** Vertical range : Range of the indexes for the rows. */
    RowRange rows_;
    /** Horizontal range : Range of the indexes for the columns. */
    ColRange cols_;
};

} // namespace STK

#endif // STK_ICONTAINER2D_H
