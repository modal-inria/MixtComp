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
 * Project: stkpp::Arrays
 * Purpose:  Define the Array2DPoint class.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_Array2DPoint.h
  * @brief A Array2DPoint is a one dimensional horizontal container
 **/

#ifndef STK_ARRAYHO_H
#define STK_ARRAYHO_H

#include "../../STKernel/include/STK_Real.h"
#include "../../Sdk/include/STK_Traits.h"

#include "STK_Arrays_Util.h"
#include "STK_IArray2D.h"

namespace STK
{

template<typename> class Array2DPoint;
template<typename> class Array2DVector;

/** @ingroup Arrays
  * @brief final class for a Real horizontal container.
  *
  * A Point is a row oriented 1D container of Real.
  */
typedef Array2DPoint<Real> Point;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for Array2DPoint class.
 **/
template<class _Type>
struct Traits< Array2DPoint<_Type> >
{
  typedef _Type          Type;
  typedef Array2DPoint<_Type> Row;
  typedef Array2DPoint<_Type> Col;
  typedef Array2DPoint<_Type> SubRow;
  typedef Array2DPoint<_Type> SubCol;
  typedef Array2DPoint<_Type> SubArray;
  typedef Array2DPoint<_Type> SubVector;
  enum
  {
    structure_ = Arrays::point_,
    orient_    = Arrays::by_col_,
    sizeRows_  = 1,
    sizeCols_  = UnknownSize,
    storage_   = Arrays::dense_ // always dense
  };
};

} // namespace hidden

/** @ingroup Arrays
 *  @brief Templated one dimensional horizontal Array. 
 * 
 * An Array2DPoint is an implementation of the interface IArray2D.
 * It's a one dimensional row-vector and is refered as a point.
 * 
 *  By default the index of the first element is 1 but this can be
 *  modified using the appropriate constructor or using the method @c shift.
 **/
template<class Type>
class Array2DPoint : public IArray2D< Array2DPoint<Type> >
{
  public:
    typedef Array2DPoint<Type> Row;
    typedef Array2DPoint<Type> Col;
    typedef Array2DPoint<Type> SubRow;
    typedef Array2DPoint<Type> SubCol;
    typedef Array2DPoint<Type> SubArray;
    typedef Array2DPoint<Type> SubVector;
    enum
    {
      structure_ = Arrays::point_,
      orient_    = Arrays::by_col_,
      sizeRows_  = 1,
      sizeCols_  = UnknownSize,
      storage_   = Arrays::dense_ // always dense
    };

    typedef AllocatorBase<Type*> Allocator;
    typedef IArray2D< Array2DPoint<Type> > Base;
    typedef ArrayBase < Array2DPoint<Type> > LowBase;

    /** Default constructor */
    Array2DPoint() : Base(Range(1), Range()) {}
    /** constructor with specified range.
     *  @param J range of the container
     **/
    Array2DPoint( Range const& J) : Base(Range(1), J) {}
    /** constructor with specified range, initialization with a constant.
     *  @param J range of the container
     *  @param v initial value of the container
     **/
    Array2DPoint( Range const& J, Type const& v) : Base(Range(1), J)
    { this->setValue(v);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if this is a wrapper of T
     **/
    Array2DPoint( Array2DPoint const& T, bool ref =false)
                : Base(T, ref)
    {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param J the range of the columns to wrap
     **/
    Array2DPoint( const Array2DPoint<Type>& T, Range const& J)
                : Base(T, T.rows(), J) {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param J the range of the data to wrap
     *  @param row the index of the row to wrap
     **/
     Array2DPoint( const Allocator& T, Range const& J, int row)
                 : Base(T.p_data(), Range(row, 1), J) {}
    /** destructor. */
    ~Array2DPoint() {}
    /** @return a constant reference on the jth element
     *  @param j index of the element (const)
     **/
    inline Type const & elt1Impl(int const& j) const { return this->data(j)[this->firstIdxRows()];}
    /** @return a reference on the jth element
     *  @param j index of the element
     **/
    inline Type& elt1Impl(int const& j) { return this->data(j)[this->firstIdxRows()];}
    /** New first indexes for the object.
     *  @param cbeg the index of the first column to set
     **/
    void shift1D(int const& cbeg) { Base::shift(this->firstIdxRows(), cbeg);}
    /**  Resize the container.
     *  @param J the range to set to the container
     **/
    inline Array2DPoint<Type>& resize1D(Range const& J)
    { Base::resize(this->rows(), J); return *this;}
    /** Add n elements to the container.
     *  @param n number of elements to add
     **/
    void pushBack( int const& n=1) { Base::pushBackCols(n);}
    /** Delete n elts at the pos index to the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
    **/
    void erase( int const& pos, int const& n=1)
    { Base::eraseCols(pos, n);}
    /** Insert n elts at the position pos of the container. The bound
     *  last_ should be modified at the very end of the insertion as pos
     *  can be a reference to it.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    void insertElt(int const& pos, int const& n =1)
    { Base::insertCols(pos, n);}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2DPoint& operator=(Rhs const& T) { return LowBase::operator=(T);}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    inline Array2DPoint& operator=(const Array2DPoint &T) { return LowBase::assign(T);}
    /** set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array2DPoint& operator=(Type const& v) { this->setValue(v); return *this;}
};

/** @ingroup Arrays
 *  ostream for Array2DPoint.
 *  @param s the output stream
 *  @param V the Array2DPoint to write
 **/
template<class Type>
ostream& operator<<(ostream& s, const Array2DPoint<Type>& V)
{ return out2D(s,V);}


} // namespace STK

#endif // STK_ARRAYHO_H
