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
 * Project:  stkpp::Array
 * created on: 10 août 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_ICArray.h
 *  @brief In this file we define the ICArray templated class.
 **/

#ifndef STK_DENSEARRAYBASE_H
#define STK_DENSEARRAYBASE_H

#include "STK_ArrayBase.h"
#include "STK_ArrayBaseVisitor.h"
#include "STK_ArrayBaseAssign.h"
#include "STK_ArrayBaseProduct.h"
#include "STK_ArrayBaseInitializer.h"

namespace STK
{
/** @class ICArray
  * @ingroup Arrays
  *
  * @brief Interface class for all CArray, CPoint, CVector
  *
  * This class is the base that is inherited by all objects (matrix, vector,
  * point) which are not expression. The common API for these objects is
  * contained in this class.
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
    inline Range const& cols() const { return allocator_.cols();};
    /**  @return the Range of the rows of the container. */
    inline Range const& rows() const { return allocator_.rows();}
    /** @return @c true if *this is reference container, @c false otherwise */
    inline bool isRef() const { return allocator_.isRef();}

    /** Get a constant reference on the main allocator. */
    inline Allocator const& allocator() const { return allocator_;}
    /** Get a constant reference on the main pointer. */
    inline Type* const& p_data() const { return allocator_.p_data();}
    /** Get a reference on the main pointer. */
    inline Type*& p_data() { return allocator_.p_data();}

    // general arrays
    inline Type& elt2Impl( int const& i, int const& j)
    { return allocator_.elt(i, j);}
    inline Type const& elt2Impl( int const& i, int const& j) const
    { return allocator_.elt(i, j);}

    inline Row row(int const& i) const { return  Row( allocator_.row(i));}
    inline SubRow row(int const& i, Range const& J) const { return SubRow( allocator_.row( i, J));}

    inline Col col(int const& j) const { return  Col( allocator_.col(j));}
    inline SubCol col(Range const& I, int const& j) const { return SubCol( allocator_.col( I, j));}

    inline SubArray sub(Range const& I, Range const& J) const { return SubArray(allocator_.sub(I, J));}

    // vectors and points
    inline Type& elt1Impl( int const& j) { return allocator_.elt(j);}
    inline Type const& elt1Impl( int const& j) const { return allocator_.elt(j);}

    // numbers
    inline Type& elt0Impl() { return allocator_.elt();}
    inline Type const& elt0Impl() const { return allocator_.elt();}

    inline SubVector sub( Range const& J) const { return SubVector( allocator_.sub(J));}

    /** @return the transposed CArray. */
    inline Transposed transpose() const { return Transposed(allocator_.transpose());}
    /** swap this with T.
     *  @param T the container to swap
     **/
    inline void swap(Derived& T) { allocator_.swap(T.allocator_);}
    /** move T to this.
     *  @param T the array to move
     **/
    inline void move(Derived const& T) { allocator_.move(T.allocator_);}
    /** resize the Array.
     *  @param I range of the rows
     *  @param J range of the columns
     **/
    Derived& resize(Range const& I, Range const& J)
    {
      allocator_.resize(I.size(), J.size());
      allocator_.shift(I.firstIdx(), J.firstIdx());
      return this->asDerived();
    }
    /** Resize the vector.
     *  @param I Range of the vector
     **/
    inline Derived& resize(Range const& I)
    {
      allocator_.resize(I.size());
      allocator_.shift(I.firstIdx());
      return this->asDerived();
    }
};

} // namespace STK

#endif /* STK_DENSEARRAYBASE_H */
