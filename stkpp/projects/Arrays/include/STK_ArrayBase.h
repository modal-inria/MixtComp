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
 * created on: 13 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ArrayBase.h
 *  @brief In this file we define the base class for Arrays and Expressions
 **/

#ifndef STK_ARRAYBASE_H
#define STK_ARRAYBASE_H

#include "STK_ExprBase.h"

namespace STK
{

/** @ingroup Arrays
 *  @brief base class for templated evaluation expressions and visitors.
 *
 * This class is the base that is inherited by all containers storing
 * values (matrix, vector, point). Expressions are not arrays.
 *  The common API for these objects is
 * contained in this class.
 *
 *  @tparam Derived is the derived type, e.g., a matrix, vector, point type or
 *  an expression.
 **/
template< class Derived>
class ArrayBase :  public ExprBase<Derived>
{
  public:
    typedef ExprBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::Row Row;
    typedef typename hidden::Traits<Derived>::Col Col;
    typedef typename hidden::Traits<Derived>::SubRow SubRow;
    typedef typename hidden::Traits<Derived>::SubCol SubCol;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;
    typedef typename hidden::Traits<Derived>::SubArray SubArray;

    enum
    {
      structure_ = hidden::Traits<Derived>::structure_,
      orient_    = hidden::Traits<Derived>::orient_,
      sizeRows_  = hidden::Traits<Derived>::sizeRows_,
      sizeCols_  = hidden::Traits<Derived>::sizeCols_,
      storage_   = hidden::Traits<Derived>::storage_
    };

  protected:
    /** Default constructor. Default values are cols=(1:0) and rows=(1:0). */
    inline ArrayBase() : Base() {}
    /** destructor */
    inline ~ArrayBase() {}

  public:
    /** Visit the container using a visitor. @sa ArrayBaseApplier
     *  @param visitor the visitor to apply
     **/
    template<typename Visitor>
    void apply(Visitor& visitor);
    /** set random values to this using a uniform law. @sa apply(), ones(), zeros() */
    void randUnif();
    /** set random values to this using a standard gaussian law. @sa apply(), ones(), zeros() */
    void randGauss();
    /** set one to this using a Visitor. @sa apply(), setValue(), zeros() */
    void ones();
    /** set zero to this using a Visitor. @sa apply(), ones(), setValue()*/
    void zeros();

    /** set a value to this container. @sa apply(), ones(), zeros()
     *  @param value the value to set
     **/
    Derived& setValue(Type const& value);
    /** @return a copy of @c rhs inside @c this object.
     *  If the ranges of @c this and @c rhs are not exactly the same, the assign
     *  method will call the resize method on this.
     *
     *  @note If @c this is a reference, it cannot be resized and thus an
     *  exception will be thrown.
     **/
    template<class Rhs> Derived& assign(ExprBase<Rhs> const& rhs);

    /** @return the matrix or vector obtained by setting this constant*/
    inline Derived& operator=( Type const& rhs) { return setValue(rhs);}
    /** @return the matrix or vector obtained by evaluating this expression */
    inline Derived& operator=( Derived const& rhs) { return assign(rhs);}
    /** @return the matrix or vector obtained by evaluating this expression */
    template<typename Rhs>
    inline Derived& operator=( ExprBase<Rhs> const& rhs) { return assign(rhs.asDerived());}
    /** Adding a Rhs to this. */
    template<typename Rhs>
    Derived& operator+=( ExprBase<Rhs> const& other);
    /** substract a Rhs to this. */
    template<typename Rhs>
    Derived& operator-=( ExprBase<Rhs> const& other);
    /** Adding a constant to this. */
    Derived& operator+=( Type const other);
    /** Substract a constant to this. */
    Derived& operator-=( Type const other);
    /** product of this by a constant. */
    Derived& operator*=( Type const other);
    /** dividing this by a constant. */
    Derived& operator/=( Type const other);

    /** @return the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type& elt(int i, int j)
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endCols() <= j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return a constant reference on element (i,j) of the 2D container
     *  @param i, j indexes of the row and of the column
     **/
    inline Type const& elt(int i, int j) const
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endCols() <= j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return a reference on the ith element
     *  @param i index of the ith element
     **/
    inline Type& elt(int i)
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived)
      return this->asDerived().elt1Impl(i);
    }
    /** @return the constant ith element
     *  @param i index of the ith element
     **/
    inline Type const& elt(int i) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived)
      return this->asDerived().elt1Impl(i);
    }
    /** @return a reference on the number */
    inline Type& elt()
    {
      STK_STATICASSERT_ZERO_DIMENSION_ONLY(Derived)
      return this->asDerived().elt0Impl();
    }
    /** @return a constant reference on the number */
    inline Type const& elt() const
    {
      STK_STATICASSERT_ZERO_DIMENSION_ONLY(Derived)
      return this->asDerived().elt0Impl();
    }
    /** @return a reference on the ith element
     *  @param i index of the ith element
     **/
    inline Type& operator[](int i) { return this->asDerived().elt1Impl(i);}
    /** @return the ith element
     *  @param i index of the ith element
     **/
    inline Type const& operator[](int i) const { return this->asDerived().elt1Impl(i);}
    /** @return the ith element
     *  @param I range to get
     **/
    inline SubVector operator[](Range const& I) const { return this->asDerived().sub(I);}
    /** @return a reference on the element (i,j) of the 2D container.
     *  @param i, j indexes of the row and of the column
     **/
    inline Type& operator()(int i, int j)
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endCols() <= j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return a constant reference on the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const& operator()(int i, int j) const
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::elt, i, j, endCols() <= j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return safely the element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type& at(int i, int j)
    {
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, endCols() <= j);}
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const& at(int i, int j) const
    {
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, beginRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, lastIdxRows() < i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, beginCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ArrayBase::at, i, j, lastIdxCols() < j);}
      return this->asDerived().elt2Impl(i, j);
    }
    /** @return safely ith element
     *  @param i index of the element
     **/
    Type& at(int i)
    {
      if (this->asDerived().begin() > i)
      { STKOUT_OF_RANGE_1ARG(ArrayBase::at, i, begin() > i);}
      if (this->asDerived().end() <= i)
      { STKOUT_OF_RANGE_1ARG(ArrayBase::at, i, end() <= i);}
      return this->asDerived().elt1Impl(i);
    }
    /** @return safely the constant ith element
     *  @param i index of the element
     **/
    Type const& at(int i) const
    {
      if (this->asDerived().begin() > i)
      { STKOUT_OF_RANGE_1ARG(ArrayBase::at, i, begin() > i);}
      if (this->asDerived().end() <= i)
      { STKOUT_OF_RANGE_1ARG(ArrayBase::at, i, end() <= i);}
      return this->asDerived().elt1Impl(i);
    }
    /** @return a constant reference on the number */
    inline Type const& operator()() const { return this->asDerived().elt0Impl();}
    /** @return the number */
    inline Type& operator()() { return this->asDerived().elt0Impl();}

    /** @return the ith row of this */
    inline Row row(int i) const { return this->asDerived().rowImpl(i);}
    /** @return the jth column of this */
    inline Col col(int j) const { return this->asDerived().colImpl(j);}
    /** @return the ith row of this in the range J */
    inline SubRow row(int i, Range const& J) const
    {
      STK_STATICASSERT_TWO_DIMENSIONS_ONLY(Derived)
      return this->asDerived().rowImpl(i,J);
    }
    /** @return the jth column of this in the range I*/
    inline SubCol col(Range const& I, int j) const
    {
      STK_STATICASSERT_TWO_DIMENSIONS_ONLY(Derived)
      return this->asDerived().colImpl(I, j);
    }
    /** @return the sub array with the rows in the range J */
    inline SubArray row(Range const& I) const
    {
      STK_STATICASSERT_TWO_DIMENSIONS_ONLY(Derived)
      return this->asDerived().rowImpl(I);
    }
    /** @return the sub array with the column in the range J */
    inline SubArray col(Range const& J) const
    {
      STK_STATICASSERT_TWO_DIMENSIONS_ONLY(Derived)
      return this->asDerived().colImpl(J);
    }
    /** @return the sub-array (I,J)*/
    inline SubArray sub(Range const& I, Range const& J) const
    {
      STK_STATICASSERT_TWO_DIMENSIONS_ONLY(Derived)
      return this->asDerived().subImpl(I, J);
    }

    /** @return the sub-vector in the range I*/
    inline SubVector sub(Range const& I) const
    {
      STK_STATICASSERT_VECTOR_ONLY(Derived)
      return this->asDerived().subImpl(I);
    }

    /** @return the first element */
    inline Type& front() { return elt(this->begin());}
    /** @return the last element */
    inline Type& back() { return elt(this->lastIdx());}
    /** @return the first element */
    inline Type const& front() const { return elt(this->begin());}
    /** @return the last element */
    inline Type const& back() const { return elt(this->lastIdx());}
   /** Convenient operator to set the coefficients of a matrix.
      *
      * The coefficients must be provided in the row/column order and exactly
      * match the size of the matrix. Otherwise an exception is throwed.
      */
    ArrayInitializer<Derived> operator<<(Type const& s);

    /** \sa operator<<(Type const&) */
    template<typename Rhs>
    ArrayInitializer<Derived> operator<<(ArrayBase<Rhs> const& other);
};

#undef MAKE_BINARY_OPERATOR

} // namespace STK

#endif /* STK_ARRAYBASE_H_ */
