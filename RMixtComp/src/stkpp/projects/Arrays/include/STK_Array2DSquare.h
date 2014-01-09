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
 * Purpose:  Define the Array2DSquare class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Array2DSquare.h
  * @brief In this file, we define Array2DSquare class.
 **/

#ifndef STK_ARRAY2DSQUARE_H
#define STK_ARRAY2DSQUARE_H

#include "STK_Array2D.h"

namespace STK
{

template<class Type> class Array2DSquare;

typedef Array2DSquare<Real> MatrixSquare;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Array2DSquare class.
 **/
template<class _Type>
struct Traits<Array2DSquare<_Type> >
{
  private:
    class Void {};
  public:
    typedef _Type        Type;
    typedef Array2DPoint<_Type> Row;
    typedef Array2DVector<_Type> Col;
    typedef Array2DPoint<_Type> SubRow;
    typedef Array2DVector<_Type> SubCol;
    typedef Array2D<_Type> SubArray;
    typedef Void SubVector;

    enum
    {
      structure_ = Arrays::square_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_   = Arrays::dense_ // always dense
    };
};

} // namespace hidden

/** @ingroup Arrays
  * @brief Derivation of the Array2DSquare class for square arrays of
  * Real.
  *
  * A Array2DSquare is a column oriented two dimensional
  * container of Real with the same number of rows and columns.
  *
  * The range of the rows and the columns is the same.
  **/
template<class Type>
class Array2DSquare : public IArray2D< Array2DSquare<Type> >
{
  public:
    typedef Array2DPoint<Type> Row;
    typedef Array2DVector<Type> Col;
    typedef Array2DPoint<Type> SubRow;
    typedef Array2DVector<Type> SubCol;
    typedef Array2D<Type> SubArray;

    enum
    {
      structure_ = Arrays::square_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_   = Arrays::dense_ // always dense
    };
    /** Type for the Interface Class. */
    typedef IArray2D< Array2DSquare<Type> > Base;
    typedef ArrayBase <  Array2DSquare<Type> > LowBase;

    /** Default constructor with cols_=(1:0) and rows_=(1:0). */
    Array2DSquare(): Base() {}
    /** Default constructor with specified Range
     *  @param I range of the Rows and Cols
     **/
    Array2DSquare( Range const& I): Base(I, I) {}
    /** constructor with cols_and rows_ givens,
     *  initialization with a constant.
     *  @param I range of the Rows and Cols
     *  @param v initial value of the container
     **/
    Array2DSquare( Range const& I, Type const& v): Base(I, I)
    { this->setValue(v);}
    /** Copy constructor.
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    Array2DSquare( Array2DSquare const&T, bool ref=false) : Base(T, ref) {}
    /** constructor by reference, ref_=1 in the range given by I.
     *  @param T the Container to wrap
     *  @param I range of the container to wrap
     **/
    Array2DSquare( Array2DSquare const& T, Range const& I) : Base(T, I, I) {}
    /** destructor. */
    ~Array2DSquare() {}
    /** New beginning index for the object.
     *  @param beg first index of the container
     **/
    inline void shift1D(int beg)
    { Base::shift(beg, beg);}
    /** New size for the container.
     *  @param I range of the columns and rows of the container
     **/
    inline Array2DSquare& resize1D( Range const& I)
    { Base::resize(I, I); return *this;}
    /** Insert n rows and column at the given position to the container.
     *  @param pos position to insert the Rows and Cols
     *  @param n number of Rows and Cols insert
     **/
    inline void insert( int pos, int const& n =1)
    {
      Base::insertRows(pos, n);
      Base::insertCols(pos, n);
    }
    /** Delete n rows and columns at the specified position to
     *  the container.
     *  @param pos position to erase the Rows and Cols
     *  @param n number of Rows and Cols erase
     **/
    inline void erase( int pos, int const& n=1)
    {
      Base::eraseRows(pos, n);
      Base::eraseCols(pos, n);
    }
    /** Add n rows and columns to the container.
     *  @param n number of Rows and Cols to add
     **/
    inline void pushBack(int const& n=1)
    {
      Base::pushBackCols(n);
      Base::pushBackRows(n);
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
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2DSquare& operator=(Rhs const& T) { return LowBase::operator=(T);}
    /** overwrite the Array2D with T.
     *  @param T the container to copy
     **/
    inline Array2DSquare& operator=( Array2DSquare const& T)
    { return LowBase::assign(T);}
    /** set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array2DSquare& operator=(Type const& v) { this->setValue(v); return *this;}
};

} // namespace STK


#endif
// STK_ARRAY2DSQUARE_H
