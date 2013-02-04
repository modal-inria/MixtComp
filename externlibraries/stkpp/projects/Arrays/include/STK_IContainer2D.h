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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::hidden::TContainer
 * Purpose:  Define the interface 2D Container classes.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
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
/** @ingroup Arrays
 * @brief Interface class for 2D containers.
 *
 * The IContainer2D class is the base class for all two-dimensional containers.
 * A two-dimensional container is defined by an horizontal range of index
 * for the columns and a vectical range of index for the rows.
 **/
class IContainer2D
{
  protected:
    /** Default constructor. cols_ = 1:0 and rows_ = 1:0. */
    inline IContainer2D() : rows_(), cols_() {}
    /** Constructor with specified ranges
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline IContainer2D( Range const& I, Range const& J) : rows_(I), cols_(J) {}
    /** Copy constructor
     *  @param T the container to copy
     **/
    inline IContainer2D( IContainer2D const& T) : rows_(T.rows_), cols_(T.cols_) {}
    /** destructor. **/
    inline ~IContainer2D() {}

  public:
    /** Index of the first column.
     *  @return the index of the first column
     **/
    inline int const& firstIdxCols() const { return cols_.firstIdx();}
    /** Index of the last column.
     *  @return the index of the last column
     **/
    inline int const& lastIdxCols() const { return cols_.lastIdx();}
    /** get the number of columns.
     * @return the Horizontal size (the number of column)
     **/
    inline int const& sizeCols() const { return cols_.size();}
    /** Range of the column of the container.
     * @return the Horizontal range
     **/
    inline Range const& cols() const { return cols_;}
    /** Index of the first row.
     * @return the index of the first row
     **/
    inline int const& firstIdxRows() const { return rows_.firstIdx();}
    /** Index of the last row.
     * @return the index of the last row
     **/
    inline int const& lastIdxRows() const { return rows_.lastIdx();}
    /** get the number of rows.
     * @return the Vertical size (the number of rows)
     **/
    inline int const& sizeRows() const { return rows_.size();}
    /**  Range of the rows of the container.
     *  @return the Vertical range
     **/
    inline Range const& rows() const { return rows_;}
    /** Is there some data ?
     *  @return @c true if the container is empty, @c false otherwise
     **/
    inline bool empty() const { return (cols_.empty() || rows_.empty());}

  protected:
    /** Set the beginning of the rows and columns.
     *  @param rbeg the first index of the rows
     *  @param cbeg the first index of the columns
     **/
    inline void shift( int const& rbeg, int const& cbeg)
    { rows_.shift(rbeg); cols_.shift(cbeg);}
    /** Set the ranges of the container.
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline void setRanges(Range const& I = Range(), Range const& J = Range())
    { rows_ = I; cols_ =J;}
    /** Set the range of the number of rows.
     *  @param I the range of the rows number
     **/
    inline void setRows( Range const& I = Range()) { rows_ = I;}
    /** Increment the range of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incRangeVe( int const& inc)
    { rows_.inc(inc);}
    /** Decrement range of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decRangeVe( int const& dec)
    { rows_.dec(dec);}
    /** Set the beginning of the rows.
     *  @param beg the first index of the rows
     **/
    inline void shiftRows( int const& beg)
    { rows_.shift(beg);}
    /** Increment the beginning of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incFirstVe( int const& inc)
    { rows_.incFirst(inc);}
    /** Decrement the beginning of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decFirstVe( int const& dec)
    { rows_.decFirst(dec);}
    /** Increment the end of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incLastVe( int const& inc) { rows_.incLast(inc);}
    /** Decrement the end of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decLastRow( int const& dec) { rows_.decLast(dec);}
    /** Set the range of the columns.
     * @param J the range of the cols number
     **/
    inline void setCols( Range const& J = Range()) { cols_ = J;}
    /** Increment the range of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incRangeHo( int const& inc) { cols_.inc(inc);}
    /** Decrement range of the number of columns.
     *  @param dec the decrement to apply
     **/
    inline void decRangeHo( int const& dec) { cols_.dec(dec);}
    /** Set the beginning of the number of columns.
     *  @param beg the new first index
     **/
    inline void shiftCols( int const& beg) { cols_.shift(beg);}
    /** inc the beginning of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incFirstHo( int const& inc) { cols_.incFirst(inc);}
    /** Decrement the beginning of the number of columns.
     *  @param dec the decrement to apply
     **/
    inline void decFirstHo( int const& dec) { cols_.decFirst(dec);}
    /** Increment the end of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incLastHo( int const& inc)  { cols_.incLast(inc);}
    /** Decrement the end of the number of columns.
     *  @param dec the decrement to apply
     **/
    inline void decLastHo( int const& dec) { cols_.decLast(dec);}
    /** swap this container with T
     * @param T the container to swap with T
     **/
     inline void swap(IContainer2D& T)
     {
       std::swap(T.rows_, this->rows_ );
       std::swap(T.cols_, this->cols_ );
     }
  private:
    /** Vertical range : Range of the indexes for the rows. */
    Range rows_;
    /** Horizontal range : Range of the indexes for the columns. */
    Range cols_;
};

} // namespace STK

#endif // STK_ICONTAINER2D_H
