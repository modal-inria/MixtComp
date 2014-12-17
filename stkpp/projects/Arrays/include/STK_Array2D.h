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
 * created on: 07 jul. 2007
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Array2D.h
  * @brief In this file, we define the final class @c Array2D.
 **/
#ifndef STK_ARRAY2D_H
#define STK_ARRAY2D_H

#include "STK_IArray2D.h"

namespace STK
{
// forward declaration
template< typename Type> class Array2D;
template< typename Type> class Array2DPoint;
template< typename Type> class Array2DVector;

/** @ingroup Arrays
  * @brief Specialization of the Array2D class for Real values.
  * An Array is a column oriented 2D container of Real.
 **/
typedef Array2D<Real>   ArrayXX;
typedef Array2D<double> ArrayXXd;
typedef Array2D<int>    ArrayXXi;


namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Array2D class.
 **/
template<class _Type>
struct Traits< Array2D<_Type> >
{
  private:
    class Void {};
  public:
    typedef _Type          Type;
    typedef Array2DPoint<_Type> Row;
    typedef Array2DVector<_Type> Col;
    typedef Array2DPoint<_Type> SubRow;
    typedef Array2DVector<_Type> SubCol;
    typedef Array2D<_Type> SubArray;
    typedef Void SubVector;

    enum
    {
      structure_ = Arrays::array2D_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_ = Arrays::dense_ // always dense
    };
};

} // namespace hidden

/** @ingroup Arrays
  * @brief Templated two dimensional column (vertically) oriented Array.
  *
  * A Array2D is a two-dimensional general implementation of an IArray2D.
  *
  * A column of an Array2D is (almost like) an @c Array2DVector
  * and a Row of an Array2D is (almost like) an @c Array2DPoint.
  *
  * When accessing to a row or a column, the methods return
  * the Array2DPoint or Array2DVector as a reference-like without copying
  * the data.
 *
 * The elements of the Array2D can be acceded with the operators ()
 * - T(2,3) allow to access the third element of the second row of T
 * - T(3,Range(1,2)) allow to access to the first two members of the third
 *  row of T
 * - T(Range(2,3),2) allow to access to the second and third element of the
 *   second column of  T.
 * - T(Range(2,3),Range(1,2)) allow to access to the specified sub-matrix in T
 *
 * @sa Array2DVector, Array2DPoint, Array2DSquare, Array2DUpperTriangular, Array2DLowerTriangular
 **/
template<class Type >
class Array2D : public IArray2D< Array2D<Type> >
{
  /** Type for the Interface base Class. */
  typedef IArray2D< Array2D<Type> > Base;
  typedef ArrayBase < Array2D<Type> > LowBase;

  public:
    typedef Array2DPoint<Type> Row;
    typedef Array2DVector<Type> Col;
    typedef Array2DPoint<Type> SubRow;
    typedef Array2DVector<Type> SubCol;
    typedef Array2D<Type> SubArray;

    enum
    {
      structure_ = Arrays::array2D_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_ = Arrays::dense_ // always dense
    };
    /** Default constructor */
    Array2D() : Base() {}
    /** constructor
     *  @param I,J range of the rows and columns
     **/
    Array2D( Range const& I, Range const& J) : Base(I, J) {}
    /** constructor with rbeg, rend, cbeg and cend specified,
     *  initialization with a constant.
     *  @param I,J range of the rows and columns
     *  @param v initial value of the container
     **/
    Array2D( Range const& I, Range const& J, Type const& v) : Base(I, J)
    { LowBase::setValue(v);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    Array2D( Array2D const& T, bool ref=false) : Base(T, ref) {}
    /** Copy constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I,J range of the rows and columns to wrap
     **/
    template<class OtherArray>
    Array2D( IArray2D<OtherArray> const& T, Range const& I, Range const& J)
           : Base(T, I, J) {}
    /** Copy constructor using an expression.
     *  @param T the container to wrap
     **/
    template<class OtherDerived>
    Array2D( ExprBase<OtherDerived> const& T): Base()
    { LowBase::operator=(T);}
    /** Wrapper constructor Contruct a reference container.
     *  @param q pointer on the data
     *  @param I,J range of the rows and columns to wrap
     **/
    Array2D( Type** q, Range const& I, Range const& J) : Base(q, I, J) {}
    /** destructor. */
    inline ~Array2D() {}
    /** operator = : overwrite the Array2D with the right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2D& operator=( Rhs const& T) { return LowBase::operator=(T);}
    /** overwrite the Array2D with T.
     *  @param T the container to copy
     **/
    inline Array2D& operator=( Array2D const& T) { return LowBase::assign(T);}
    /** set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array2D& operator=( Type const& v) { return LowBase::setValue(v);}
    /** Swapping the pos1 row and the pos2 row.
     *  @param pos1,pos2 position of the rows to swap
     **/
    void swapRows( int const& pos1, int const& pos2)
    {
#ifdef STK_BOUNDS_CHECK
      // check conditions
      if (this->beginRows() > pos1)
      STKOUT_OF_RANGE_2ARG(Array2D::swapRows,pos1, pos2,beginRows()>pos1);
      if (this->endRows() <= pos1)
      STKOUT_OF_RANGE_2ARG(Array2D::swapRows,pos1, pos2,endRows()<=pos1);
      if (this->beginRows() > pos2)
      STKOUT_OF_RANGE_2ARG(Array2D::swapRows,pos1, pos2,beginRows()>pos2);
      if (this->endRows() <= pos2)
      STKOUT_OF_RANGE_2ARG(Array2D::swapRows,pos1, pos2,endRows()<=pos2);
#endif
      for (int j=this->beginCols(); j<this->endCols(); j++)
      { std::swap(this->elt(pos1, j), this->elt(pos2, j));}
    }
};

} // namespace STK

#endif
// STK_ARRAY2D_H
