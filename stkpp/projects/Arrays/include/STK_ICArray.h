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
 * Project:  stkpp::Array
 * created on: 10 août 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ICArray.h
 *  @brief Interface base class for the CArray, this is an internal header file,
 *  included by other Containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class like CArray, except if you want to create your own
 *  Container Class.
 **/

#ifndef STK_DENSEARRAYBASE_H
#define STK_DENSEARRAYBASE_H

#include "STK_ExprBaseVisitor.h"
#include "STK_ExprBaseDot.h"
#include "STK_ExprBaseProduct.h"

#include "STK_ArrayBaseApplier.h"
#include "STK_ArrayBaseAssign.h"
#include "STK_ArrayBaseInitializer.h"

namespace STK
{
/** @class ICArray
  * @ingroup Arrays
  *
  * @brief Interface class for all CArray, CPoint, CVector
  *
  * This class is the base that is inherited by all objects (matrix, vector,
  * point) which are not expression and stored as CArrays. The common API for
  * these objects is contained in this class.
  *
  * This is essentially a wrapper of a CAllocator
  *
  * @tparam Derived is the derived type, e.g., a matrix type.
  */
template<class Derived>
class ICArray : public ArrayBase<Derived>
{
  public:
    typedef ArrayBase<Derived> Base;

    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::Row Row;
    typedef typename hidden::Traits<Derived>::Col Col;
    typedef typename hidden::Traits<Derived>::SubRow SubRow;
    typedef typename hidden::Traits<Derived>::SubCol SubCol;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;
    typedef typename hidden::Traits<Derived>::SubArray SubArray;
    typedef typename hidden::Traits<Derived>::Transposed Transposed;

    typedef typename hidden::Traits<Derived>::Allocator Allocator;

    enum
    {
      structure_ = hidden::Traits<Derived>::structure_,
      orient_    = hidden::Traits<Derived>::orient_,
      sizeRows_  = hidden::Traits<Derived>::sizeRows_,
      sizeCols_  = hidden::Traits<Derived>::sizeCols_,
      storage_   = hidden::Traits<Derived>::storage_
    };

  protected:
    /** allocator of the memory  */
    Allocator allocator_;
    /** default constructor. */
    inline ICArray() : Base(), allocator_()
    {}
    /** constructor with specified sizes.
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     **/
    inline ICArray( int const& sizeRows, int const& sizeCols)
                  : Base(), allocator_(sizeRows, sizeCols)
    {}
    /** constructor with specified sizes and value.
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     *  @param value the value to set
     **/
    inline ICArray( int const& sizeRows, int const& sizeCols, Type const& value)
                  : Base(), allocator_(sizeRows, sizeCols, value)
    {}
    /** copy or wrapper constructor.
     *  @param T size of the rows
     *  @param ref is this owning its own data ?
     **/
    inline ICArray( ICArray const& T, bool ref = false)
                  : Base(), allocator_(T.allocator_, ref)
    {}
    /** wrapper constructor for 0 based C-Array.
     *  @param q pointer on the array
     *  @param nbRow number of rows
     *  @param nbCol number of columns
     **/
    inline ICArray( Type* const& q, int nbRow, int nbCol)
                  : Base(), allocator_(q, nbRow, nbCol)
    {}
    /** constructor by reference, ref_=1.
     *  @param allocator with the data
     **/
    template< class OtherAllocator>
    inline ICArray( OtherAllocator const& allocator)
                  : Base(), allocator_(allocator, true)
    {}
    /**  destructor */
    inline ~ICArray() {}
  public:
    /** @return the Horizontal range */
    inline Range cols() const { return allocator_.cols();};
    /**  @return the index of the first column */
    inline int firstIdxCols() const { return allocator_.firstIdxCols();}
    /** @return the index of the last column */
    inline int lastIdxCols() const { return allocator_.lastIdxCols();}
    /** @return the Horizontal size (the number of column) */
    inline int sizeColsImpl() const { return allocator_.sizeCols();}
    /**  @return the Range of the rows of the container. */
    inline Range rows() const { return allocator_.rows();}
    /** @return the index of the first row*/
    inline int firstIdxRows() const { return allocator_.firstIdxRows();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return allocator_.lastIdxRows();}
    /** @return the Vertical size (the number of rows) */
    inline int sizeRowsImpl() const { return allocator_.sizeRows();}
    /** @return @c true if *this is reference container, @c false otherwise */
    inline bool isRef() const { return allocator_.isRef();}

    /** Get a constant reference on the main allocator. */
    inline Allocator const& allocator() const { return allocator_;}
    /** Get a constant reference on the main pointer. */
    inline Type const* p_data() const { return allocator_.p_data();}
    /** Get a reference on the main pointer. */
    inline Type* p_data() { return allocator_.p_data();}

    // general arrays
    inline Type& elt2Impl( int const& i, int const& j)
    { return allocator_.elt(i, j);}
    inline Type const elt2Impl( int const& i, int const& j) const
    { return allocator_.elt(i, j);}

    inline Row row(int const& i) const { return  Row( allocator_.row(i));}
    inline SubRow row(int const& i, Range const& J) const { return SubRow( allocator_.row( i, J));}

    inline Col col(int const& j) const { return  Col( allocator_.col(j));}
    inline SubCol col(Range const& I, int const& j) const { return SubCol( allocator_.col( I, j));}

    inline SubArray sub(Range const& I, Range const& J) const { return SubArray(allocator_.sub(I, J));}

    // vectors and points
    inline Type& elt1Impl( int const& j) { return allocator_.elt(j);}
    inline Type const elt1Impl( int const& j) const { return allocator_.elt(j);}

    // numbers
    inline Type& elt0Impl() { return allocator_.elt();}
    inline Type const elt0Impl() const { return allocator_.elt();}

    inline SubVector sub( Range const& J) const { return SubVector( allocator_.sub(J));}

    /** @return the transposed CArray. */
    inline Transposed transpose() const { return Transposed(allocator_.transpose());}
    /** exchange this with T.
     *  @param T the container to exchange with this
     **/
    inline void exchange(Derived& T) { allocator_.exchange(T.allocator_);}
    /** move T to this.
     *  @param T the array to move
     **/
    inline void move(Derived const& T) { allocator_.move(T.allocator_);}
    /** shift the Array.
     *  @param firstIdxRows,firstIdxCols  first indexes of the rows and columns
     **/
    Derived& shift(int firstIdxRows, int firstIdxCols)
    {
      if((this->firstIdxRows() == firstIdxRows) && (this->firstIdxCols()==firstIdxCols)) return this->asDerived();
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(ICArray::shift,firstIdxRows,firstIdxCols,cannot operate on reference);}
      allocator_.shift(firstIdxRows, firstIdxCols);
      return this->asDerived();
    }
    /** shift the Array.
     *  @param firstIdx first index of the vector/point
     **/
    Derived& shift(int firstIdx)
    {
      if((this->firstIdx() == firstIdx)) return this->asDerived();
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(ICArray::shift,firstIdx,cannot operate on reference);}
      allocator_.shift(firstIdx);
      return this->asDerived();
    }
    /** resize the Array.
     *  @param I range of the rows
     *  @param J range of the columns
     **/
    Derived& resize(Range const& I, Range const& J)
    {
      if((rows() == I) && (cols()==J)) return this->asDerived();
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(ICArray::resize,I,J,cannot operate on reference);}
      allocator_.resize(I.size(), J.size());
      allocator_.shift(I.firstIdx(), J.firstIdx());
      return this->asDerived();
    }
    /** Resize the vector.
     *  @param I Range of the vector
     **/
    inline Derived& resize(Range const& I)
    {
      if (this->range() == I) return this->asDerived();
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(ICArray::resize,I,cannot operate on reference);}
      allocator_.resize(I.size());
      allocator_.shift(I.firstIdx());
      return this->asDerived();
    }
  private:
    /** TODO: overwrite @c this with @c src.
     *  @note If the size match, @c this is not resized, and in this case,
     *  the method take care of the possibly of overlapping.
     *  @param src the container to copy
     **/
    Derived& copy( ICArray const& src)
    {
      // Resize if necessary.
      if ( (this->sizeRows() != src.sizeRows()) || (this->sizeCols() != src.sizeCols()) )
        this->resize(src.rows(), src.cols());

      // Copy without overlapping
      if (src.firstIdxRows()>=this->firstIdxRows())
      {
        if (src.firstIdxCols()>this->firstIdxCols())
        {
          for ( int jSrc=src.firstIdxCols(), jDst=this->firstIdxCols(); jSrc<=src.lastIdxCols(); jDst++, jSrc++)
          { this->copyColumnForward(src, jDst, jSrc);}
        }
        else
        {
          for ( int jSrc=src.lastIdxCols(), jDst=this->lastIdxCols(); jSrc>=src.firstIdxCols(); jDst--, jSrc--)
          { this->copyColumnForward(src, jDst, jSrc);}
        }
        return this->asDerived();
      }
      // src.firstIdxRows()<this->firstIdxRows()
      if (src.firstIdxCols()>=this->firstIdxCols())
      {
        for ( int jSrc=src.firstIdxCols(), jDst=this->firstIdxCols(); jSrc<=src.lastIdxCols(); jDst++, jSrc++)
        { this->copyColumnBackward(src, jDst, jSrc);}
      }
      else // src.firstIdxCols()<this->firstIdxCols()
      {
        for ( int jSrc=src.lastIdxCols(), jDst=this->lastIdxCols(); jSrc>=src.firstIdxCols(); jDst--, jSrc--)
        { this->copyColumnBackward(src, jDst, jSrc);}
      }
      return this->asDerived();
    }

};

} // namespace STK

#endif /* STK_DENSEARRAYBASE_H */
