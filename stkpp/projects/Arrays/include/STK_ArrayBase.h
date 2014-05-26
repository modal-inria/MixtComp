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
 * This class is the base that is inherited by all objects (matrix, vector,
 * point and expression). The common API for these objects is contained in
 * this class.
 *
 *  @tparam Derived is the derived type, e.g., a matrix, vector, point type or
 *  an expression.
 **/
template<class Derived>
class ArrayBase : public ExprBase<Derived>
{
  public:
    typedef ExprBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

    enum
    {
      structure_ = hidden::Traits<Derived>::structure_,
      orient_    = hidden::Traits<Derived>::orient_,
      sizeRows_  = hidden::Traits<Derived>::sizeRows_,
      sizeCols_  = hidden::Traits<Derived>::sizeCols_,
      storage_   = hidden::Traits<Derived>::storage_
    };

  protected:
    /** Default constructor */
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
    void setValue(Type const& value);
    /** @return a copy of @c rhs inside @c this object.
     *  If the ranges of @c this and @c rhs are not exactly the same, the assign
     *  method will call the resize method on this.
     *
     *  @note If @c this is a reference, it cannot be resized and thus an
     *  exception will be thrown.
     **/
    template<class Rhs> Derived& assign(ExprBase<Rhs> const& rhs);

    /** @return the matrix or vector obtained by setting this constant*/
    inline Derived& operator=( Type const& rhs)
    { this->setValue(rhs); return this->asDerived();}
    /** @return the matrix or vector obtained by evaluating this expression */
    inline Derived& operator=( Derived const& rhs)
    { return this->assign(rhs);}
    /** @return the matrix or vector obtained by evaluating this expression */
    template<typename Rhs>
    inline Derived& operator=( ExprBase<Rhs> const& rhs)
    { return this->assign(rhs.asDerived());}
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

/** @ingroup Arrays
 *  @brief Interface class for homogeneous 2D containers which can be
 *  part of an expression and handle their own storage (like IArray2D derived
 *  classes).
 *  @sa IArray2DBase, Const::Identity
 **/
template < class Derived, int SizeRow_ = hidden::Traits<Derived>::sizeRows_
                        , int SizeCol_ = hidden::Traits<Derived>::sizeCols_>
class ITArrayBase : protected TContainer2D<SizeRow_, SizeCol_>
                  , public ArrayBase<Derived>
{
  protected:
    /** Type of the Base container */
    typedef TContainer2D<SizeRow_, SizeCol_ > Base2D;
    /** Type of the Base container */
    typedef ArrayBase<Derived> Base;
    /** Default constructor. Default values are cols=(1:0) and rows=(1:0). */
    inline ITArrayBase() : Base2D(), Base() {}
    /** constructor with specified Range.
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline ITArrayBase( Range const& I, Range const& J) : Base2D(I, J), Base() {}
    /** Copy constructor.
     *  @param T the container to copy
     **/
    inline ITArrayBase( ITArrayBase const& T) : Base2D(T), Base() {}
    /** destructor. */
    inline ~ITArrayBase() {}

  public:
    /**@return the Horizontal range */
    inline Range cols() const { return Base2D::cols();}
    /** @return the index of the first column */
    inline int const& firstIdxCols() const { return Base2D::firstIdxCols();}
    /**  @return the index of the last column */
    inline int lastIdxCols() const { return Base2D::lastIdxCols();}
    /** @return the Horizontal size (the number of column) */
    inline int sizeColsImpl() const { return Base2D ::sizeColsImpl();}
    /** @return the Vertical range */
    inline Range rows() const { return Base2D::rows();}
    /** @return the index of the first row */
    inline int firstIdxRows() const { return Base2D::firstIdxRows();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return Base2D::lastIdxRows();}
    /** @return the Vertical size (the number of rows) */
    inline int sizeRowsImpl() const { return Base2D::sizeRowsImpl();}
    /**  @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return Base2D::empty();}
};

#undef MAKE_BINARY_OPERATOR

} // namespace STK

#endif /* STK_ARRAYBASE_H_ */
