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
 * Project: stkpp::Arrays
 * Purpose:  Define the Array2DVector class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Array2DVector.h
  * @brief A Array2DVector is a one dimensional horizontal container
  * 
  * An Array2DVector is an implementation of the interface IArray2D.
  * It's a one dimensional horizontal container.
 **/

#ifndef STK_ARRAY2DVECTOR_H
#define STK_ARRAY2DVECTOR_H

#include "../../STKernel/include/STK_Real.h"


#include "STK_IArray2D.h"
#include "STK_Display.h"

namespace STK
{

template<typename> class Array2DPoint;
template<typename> class Array2DVector;


/** @ingroup Arrays
  * @brief final class for a Real vertical container.
  *
  * A Vector is a column oriented 1D container of Real.
  **/
typedef Array2DVector<Real> Vector;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Array2DVector class.
 **/
template<class _Type>
struct Traits< Array2DVector<_Type> >
{
  typedef _Type          Type;
  typedef Array2DPoint<_Type> Row;
  typedef Array2DVector<_Type> Col;
  typedef Array2DPoint<_Type> SubRow;
  typedef Array2DVector<_Type> SubCol;
  typedef Array2DVector<_Type> SubArray;
  typedef Array2DVector<_Type> SubVector;
  enum
  {
    structure_ = Arrays::vector_,
    orient_    = Arrays::by_col_,
    sizeCols_  = 1,
    sizeRows_  = UnknownSize,
    storage_ = Arrays::dense_ // always dense
  };
};

} // namespace hidden

/** @ingroup Arrays
 *  @brief Templated one dimensional horizontal Array. 
 * 
 *  An Array2DVector is a Vertical container of a single column.
 * 
 *  By default the index of the first element is 1 but this can be
 *  modified using the appropriate constructor or using the method @c shift.
 *
 *  @sa Array2DPoint
 **/
template<class Type>
class Array2DVector : public IArray2D< Array2DVector<Type> >
{
  public:
    typedef Array2DVector<Type> Row;
    typedef Array2DVector<Type> Col;
    typedef Array2DVector<Type> SubRow;
    typedef Array2DVector<Type> SubCol;
    typedef Array2DVector<Type> SubArray;
    typedef Array2DVector<Type> SubVector;

    enum
    {
      structure_ = Arrays::vector_,
      orient_    = Arrays::by_col_,
      sizeCols_  = 1,
      sizeRows_  = UnknownSize,
      storage_ = Arrays::dense_ // always dense
    };
    typedef AllocatorBase<Type*> Allocator;
    typedef IArray2D< Array2DVector<Type> > Base;
    typedef ArrayBase < Array2DVector<Type> > LowBase;

    /** Default constructor */
    Array2DVector() : Base( Range(), Range(1)) {}
    /** constructor with specified range.
     *  @param I range of the container
     **/
    Array2DVector( Range const& I) :Base(I, Range(1)) {}
    /** constructor with specified range, initialization with a constant.
     *  @param I range of the container
     *  @param v initial value of the container
     **/
    Array2DVector( Range const& I, Type const& v) : Base(I, Range(1))
    { this->setValue(v);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if this is a wrapper of T
     **/
    Array2DVector( const Array2DVector &T, bool ref =false)
                 : Base(T, ref) {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I the range of the columns to wrap
     **/
    Array2DVector( const Array2DVector<Type>& T, Range const& I)
                 : Base(T, I, T.cols())
    {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I the range of the data to wrap
     *  @param col the index of the col to wrap
     **/
    template<class OtherArray>
    Array2DVector( IArray2D<OtherArray> const& T, Range const& I, int col)
                : Base(T, I, Range(col, 1))
    {}
    /** constructor by reference, ref_=1.
     *  @param p_data a pointer on the data to wrap
     *  @param I the range of the data to wrap
     *  @param col the index of the col to wrap
     **/
     Array2DVector( Type** p_data, Range const& I, int col)
                  : Base(p_data, I, Range(col, 1))
    {}
    /** destructor. */
    ~Array2DVector() {}
    /** @return a constant reference on the ith element
     *  @param i index of the element (const)
     **/
    inline Type const & elt1Impl(int const& i) const { return this->data(this->firstIdxCols())[i];}
    /** @return a reference on the ith element
     *  @param i index of the element
     **/
    inline Type& elt1Impl(int const& i) { return this->data(this->firstIdxCols())[i];}
    /** New first index for the object.
     *  @param rbeg the index of the first row to set
     **/
    void shift1D(int const& rbeg)
    { Base::shift(rbeg, this->firstIdxCols());}
    /**  Resize the container.
     *  @param I the range to set to the container
     **/
    inline Array2DVector<Type>& resize1D(Range const& I)
    { Base::resize(I, this->cols()); return *this;}
    /** Add n elements to the container.
     *  @param n number of elements to add
     **/
    void pushBack( int const& n=1)
    { Base::pushBackRows(n);}
    /** Delete n elements at the pos index to the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
    **/
    void erase( int const& pos, int const& n=1)
    { Base::eraseRows(pos, n);}
    /** Insert n elements at the position pos of the container. The bound
     *  last_ should be modified at the very end of the insertion as pos
     *  can be a reference to it.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    void insertElt(int const& pos, int const& n =1)
    { Base::insertRows(pos, n);}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2DVector& operator=(Rhs const& T) { return LowBase::operator=(T);}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    inline Array2DVector& operator=(const Array2DVector &T) { return LowBase::assign(T);}
    /** set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array2DVector& operator=(Type const& v) { this->setValue(v); return *this;}
};

} // namespace STK

#endif // STK_ARRAY2DVECTOR_H
