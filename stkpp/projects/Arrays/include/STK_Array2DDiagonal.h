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
 * Project:  stkpp::Arrays
 * Purpose:  Define the Array2DDiagonal class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Array2DDiagonal.h
  * @brief In this file, we define Array2DDiagonal class.
 **/

#ifndef STK_ARRAY2DDIAGONAL_H
#define STK_ARRAY2DDIAGONAL_H

#include "STK_Array2D.h"

namespace STK
{

template<class Type> class Array2DDiagonal;

typedef Array2DDiagonal<Real> MatrixDiagonal;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Array2DDiagonal class.
 **/
template<class _Type>
struct Traits<Array2DDiagonal<_Type> >
{
  private:
    class Void {};
  public:
    typedef _Type        Type;
    typedef Array2DPoint<_Type> Row;
    typedef Array2DVector<_Type> Col;
    typedef Array2DPoint<_Type> SubRow;
    typedef Array2DVector<_Type> SubCol;
    typedef Array2DDiagonal<_Type> SubArray;
    typedef Array2DDiagonal<_Type> SubVector;

    enum
    {
      structure_ = Arrays::diagonal_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_   = Arrays::dense_ // always dense
    };
};

} // namespace hidden

/** @ingroup Arrays
  * @brief Derivation of the Array2DDiagonal class for square arrays of
  * Real.
  *
  * A Array2DDiagonal is a column oriented two dimensional
  * container of Real with the same number of rows and columns.
  *
  * The range of the rows and the columns is the same.
  **/
template<class Type>
class Array2DDiagonal : public IArray2D< Array2DDiagonal<Type> >
{
  public:
    typedef Array2DPoint<Type> Row;
    typedef Array2DVector<Type> Col;
    typedef Array2DPoint<Type> SubRow;
    typedef Array2DVector<Type> SubCol;
    typedef Array2DDiagonal<Type> SubArray;
    typedef Array2DDiagonal<Type> SubVector;

    enum
    {
      structure_ = Arrays::diagonal_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_   = Arrays::dense_ // always dense
    };
    /** Type for the Interface Class. */
    typedef IArray2D< Array2DDiagonal<Type> > Base;
    typedef ArrayBase <  Array2DDiagonal<Type> > LowBase;

    /** Default constructor. */
    Array2DDiagonal(): Base() {}
    /** Constructor with specified range.
     *  @param I range of the Rows and Cols
     **/
    Array2DDiagonal( Range const& I): Base(I, I) {}
    /** constructor with cols_and rows_ givens,
     *  initialization with a constant.
     *  @param I range of the Rows and Cols
     *  @param v initial value of the container
     **/
    Array2DDiagonal( Range const& I, Real const& v): Base(I, I) { this->setValue(v);}
    /** Copy constructor.
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    Array2DDiagonal( Array2DDiagonal const&T, bool ref=false) : Base(T, ref) {}
    /** constructor by reference, ref_=1 in the range given by I.
     *  @param T the Container to wrap
     *  @param I range of the container to wrap
     **/
    Array2DDiagonal( Array2DDiagonal const& T, Range const& I) : Base(T, I, I) {}
    /** destructor. */
    ~Array2DDiagonal() {}
    /** @param i index of the diagonal element
     *  @return a reference on the ith diagonal element
     **/
    inline Type& elt1Impl(int const& i)
    { return this->elt(i,i);}
    /** @param i index of the diagonal element
     *  @return a constant reference on the ith diagonal element
     **/
    inline Type const elt1Impl(int const& i) const
    { return this->elt(i,i);}
    /** New beginning index for the object.
     *  @param beg first index of the container
     **/
    inline void shift1D(int beg)
    { Base::shift(beg, beg);}

    /** New size for the container.
     *  @param I range of the columns and rows of the container
     **/
    inline Array2DDiagonal& resize1D( Range const& I)
    { Base::resize(I, I); return *this;}

    /** Insert n rows and column at the given position to the container.
     *  @param pos position to insert the Rows and Cols
     *  @param n number of Rows and Cols insert
     **/
    inline void insert( int const& pos, int const& n =1)
    {
      Base::insertCols(pos, n);
      Base::insertRows(pos, n);
    }

    /** Delete n rows and columns at the specified position to
     *  the container.
     *  @param pos position to erase the Rows and Cols
     *  @param n number of Rows and Cols erase
     **/
    inline void erase( int const& pos, int const& n=1)
    {
      Base::eraseRows(pos, n);
      Base::eraseCols(pos, n);
    }

    /** Add n rows and columns to the container.
     *  @param n number of Rows and Cols to add
     **/
    inline void pushBack(int n=1)
    {
      Base::pushBackRows(n);
      Base::pushBackCols(n);
    }

    /** Delete n rows and columns at the end of the container.
     *  @param n number of Rows and Cols to delete
     **/
    inline void popBack(int const& n=1)
    {
      Base::popBackRows(n);
      Base::popBackCols(n);
    }
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @note If the size match, @c this is not resized
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2DDiagonal& operator=(Rhs const& T) { return LowBase::operator=(T);}
    /** overwrite the Array2D with T.
     *  @note If the size match, @c this is not resized
     *  @param T the container to copy
     **/
    inline Array2DDiagonal& operator=(const Array2DDiagonal &T) { return LowBase::assign(T);}
    /** set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array2DDiagonal& operator=(Type const& v) { this->setValue(v); return *this;}
};

} // namespace STK


#endif
// STK_ARRAY2DDIAGONAL_H
