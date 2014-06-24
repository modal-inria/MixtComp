/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * created on: 26 nov. 2011
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ITContainer2D.h
 *  @brief This is an internal header file, included by other
 *  Containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class like Array2D, except if you want to create your own
 *  Container Class.
 **/

#ifndef STK_ITCONTAINER2D_H
#define STK_ITCONTAINER2D_H

#include "Sdk/include/STK_Macros.h"
#include "Sdk/include/STK_IRecursiveTemplate.h"

#include "STK_Traits.h"

#include "STK_Arrays_Util.h"
#include "STK_IContainer2D.h"

namespace STK
{

namespace hidden
{
/** @ingroup hidden
 * classes inheriting NoAssignOperator should not generate a default operator=.
 **/
class NoAssignOperator
{ private:
    NoAssignOperator& operator=(const NoAssignOperator&);
};

} // hidden namespace

/** @ingroup Arrays
 * @brief Interface base class for 2D containers.
 *
 * Use the curious recursive template paradigm : the template
 * parameter @c Derived is the name of the class that
 * implements the interface ITContainer.
 * For example
 * @code
 * template<class Type>
 * class Derived : public ITContainer< Derived<Type> >
 * {...}
 * @endcode
 *
 * The ITContainer2DBase class is the base class for all two-dimensional containers.
 * A two-dimensional container is defined by an horizontal range of index
 * for the columns and a vertical range of index for the rows.
 *
 * The pseudo virtual function defined in this interface and to implement
 * in the derived classes have the following definitions:
 * @code
 *   Type& elt2Impl( int i, int j);
 *   Type const elt2Impl( int i, int j) const;
 * @endcode
 * for the 2D (@ref Arrays::array2D_, @ref Arrays::square_, @ref Arrays::upper_triangular_
 * and @ref Arrays::lower_triangular_) arrays,
 * @code
 *   Type& elt1Impl( int i);
 *   Type const elt1Impl( int i) const;
 * @endcode
 * for the diagonal (@ref Arrays::diagonal_) arrays
 * and the 1D (@ref Arrays::vector_, @ref Arrays::point_)
 * arrays, and
 * @code
 *   Type& elt0Impl();
 * @endcode
 * for the @ref Arrays::number_ arrays.
 **/
template <class Derived>
class ITContainer2DBase  : public IRecursiveTemplate<Derived>, hidden::NoAssignOperator
{
  public:
    typedef IRecursiveTemplate<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** Default constructor */
    inline ITContainer2DBase() : Base() {}
    /** destructor. **/
    inline ~ITContainer2DBase() {}

  public:
    /** @return the Horizontal range */
    inline Range cols() const { return this->asDerived().cols();};
    /**  @return the index of the first column */
    inline int firstIdxCols() const { return this->asDerived().cols().firstIdx();}
    /** @return the index of the last column */
    inline int lastIdxCols() const { return this->asDerived().cols().lastIdx();}
    /** @return the Horizontal size (the number of column) */
    inline int sizeCols() const { return this->asDerived().sizeColsImpl();}
    /** @return the Vertical range*/
    inline Range rows() const { return this->asDerived().rows();}
    /** @return the index of the first row*/
    inline int firstIdxRows() const { return this->asDerived().rows().firstIdx();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return this->asDerived().rows().lastIdx();}
    /** @return the Vertical size (the number of rows) */
    inline int sizeRows() const { return this->asDerived().sizeRowsImpl();}
    /** @return the size of the container (ten number of rows by the number of columns */
    inline int sizeArray() const { return sizeRows()*sizeCols();}
    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return (cols().empty() || rows().empty());}
    /** @return the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type& elt(int i, int j)
    {
#ifdef STK_BOUNDS_CHECK
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, lastIdxRows() < i);}
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, lastIdxCols() < j);}
#endif
    return this->asDerived().elt2Impl(i,j);
}
    /** @return a constant reference on element (i,j) of the 2D container
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt(int i, int j) const
    {
#ifdef STK_BOUNDS_CHECK
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, lastIdxRows() < i);}
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, lastIdxCols() < j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return a reference on the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type& operator()(int i, int j)
    { return this->asDerived().elt2Impl(i,j);}
    /** @return a constant reference on the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const operator()(int i, int j) const
    { return this->asDerived().elt2Impl(i,j);}
    /** @return safely the element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type& at(int i, int j)
    {
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const at(int i, int j) const
    {
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return this->asDerived().elt2Impl(i, j);
    }
    /** @return a constant reference on the ith element for vector_, point_ and diagonal_ containers
     *  @param i index of the ith element
     **/
    inline Type const elt(int const& i) const { return this->asDerived().elt1Impl(i);}
    /** @return a constant reference on the ith  element
     *  @param i index of the ith element
     **/
    inline Type const operator[](int const& i) const { return this->asDerived().elt1Impl(i);}
    /** @return safely the constant jth element
     *  @param i index of the element
     **/
    Type const at(int i) const
    {
      if (this->asDerived().firstIdx() > i)
      { STKOUT_OF_RANGE_1ARG(ITContainer::at, i, firstIdx() > i);}
      if (this->asDerived().lastIdx() < i)
      { STKOUT_OF_RANGE_1ARG(ITContainer::at, i, lastIdx() < i);}
      return this->asDerived().elt1Impl(i);
    }
    /** @return a constant reference on the number */
    inline Type const elt() const { return this->asDerived().elt0Impl();}
    /** @return a constant reference on the number */
    inline Type const operator()() const { return this->asDerived().elt0Impl();}
    /** @return the ith element for vector_, point_ and diagonal_ containers
     *  @param i index of the ith element
     **/
    inline Type& elt(int const& i) { return this->asDerived().elt1Impl(i);}
    /** @return the element ith element
     *  @param i index of the ith element
     **/
    inline Type& operator[](int const& i) { return this->asDerived().elt1Impl(i);}
    /** @return the number */
    inline Type& elt() { return this->asDerived().elt0Impl();}
    /** @return the number */
    inline Type& operator()() { return this->asDerived().elt0Impl();}
};

/** @ingroup Arrays
 *  @brief Specialized interface class for homogeneous arrays that can be
 *  either a 2D arrays and 1D arrays.
 **/
template < class Derived, int Structure_ = hidden::Traits<Derived>::structure_ >
class ITContainer;

/** @ingroup Arrays
 *  Specialization for array2D_ */
template <class Derived>
class ITContainer<Derived, Arrays::array2D_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

    /** compute the range of the stored elements in a column. */
    inline Range rangeRowsInCol(int) const { return this->asDerived().rows();}
    /** compute the range of the stored elements in a row. */
    inline Range rangeColsInRow(int) const { return this->asDerived().cols();}

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}
};

/** @ingroup Arrays Specialization for square_ */
template <class Derived>
class ITContainer<Derived, Arrays::square_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return the range of the square container. */
    inline Range range() const { return this->asDerived().rows();}
    /** @return the first index of the square container. */
    inline int firstIdx() const { return this->asDerived().firstIdxRows();}
    /** @return the last index of the square container. */
    inline int lastIdx() const { return this->asDerived().lastIdxRows();}
    /** @return the size of the rows and columns of the container. */
    inline int size() const { return this->asDerived().sizeRows();}
    /** compute the range of the stored elements in a column. */
    inline Range rangeRowsInCol(int) const { return this->asDerived().rows();}
    /** compute the range of the stored elements in a row. */
    inline Range rangeColsInRow(int) const { return this->asDerived().rows();}
};

/** @ingroup Arrays
 *  Specialization for lower_triangular_ */
template <class Derived>
class ITContainer<Derived, Arrays::lower_triangular_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const at(int i, int j) const
    {
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return (j>i) ? Type() : this->asDerived().elt2Impl(i, j);
    }
    /** @return the Range of the effectively stored elements in the column @c icol.
     *  @param icol the number of the column to compute the range
     **/
    inline Range rangeRowsInCol( int const& icol) const
    { return Range(icol, this->lastIdxRows(), 0);}
    /** compute the range of the effectively stored elements in the row
     *  irow.
     *  @param irow the number of the row to compute the range
     **/
    inline Range rangeColsInRow( int const& irow) const
    { return Range(this->firstIdxCols(), std::min(irow, this->lastIdxCols()), 0);}
};

/** @ingroup Arrays
 *  Specialization for upper_triangular_ */
template <class Derived>
class ITContainer<Derived, Arrays::upper_triangular_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const at(int i, int j) const
    {
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return (i>j) ? Type() : this->asDerived().elt2Impl(i, j);
    }
    /** @return the range of the effectively stored elements in the row @c irow.
     *  @param irow the index of the row we want to compute the range
     **/
    inline Range rangeColsInRow( int const& irow) const
    { return Range(irow, this->lastIdxCols(), 0);}
    /** compute the range of the effectively stored elements in the col
     *  icol.
     *  @param icol the number of the column to compute the range
     **/
    inline Range rangeRowsInCol( int const& icol) const
    { return Range(this->firstIdxRows(), std::min(icol, this->lastIdxRows()), 0);}
};

/** @ingroup Arrays
 *  Specialization for diagonal_ */
template <class Derived>
class ITContainer<Derived, Arrays::diagonal_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return the range of the diagonal container. */
    inline Range range() const { return this->rows();}
    /** @return the first index of the square container. */
    inline int firstIdx() const { return range().firstIdx();}
    /** @return the last index of the square container. */
    inline int lastIdx() const { return range().lastIdx();}
    /** @return the size of the rows and columns of the container. */
    inline int size() const { return this->asDerived().range().size();}
    /** @return the first diagonal element */
    inline Type& front() { return elt(firstIdx());}
    /** @return the first diagonal element */
    inline Type const front() const { return elt(firstIdx());}
    /** @return the last diagonal element */
    inline Type& back() { return elt(lastIdx());}
    /** @return the last diagonal element */
    inline Type const back() const { return elt(firstIdx());}
//    /** @return safely the element (i, j).
//     *  @param i index of the row
//     *  @param j index of the col
//     **/
//    Type& at(int i, int j)
//    {
//      if (this->firstIdx() > i)
//      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdx() > i);}
//      if (this->lastIdx() < i)
//      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdx() < i);}
//      if (this->firstIdx() > j)
//      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdx() > j);}
//      if (this->lastIdx() < j)
//      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdx() < j);}
//      if (i != j)
//      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, i != j);}
//      return this->asDerived().elt2Impl(i, j);
//    }
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const at(int i, int j) const
    {
      if (this->firstIdx() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdx() > i);}
      if (this->lastIdx() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdx() < i);}
      if (this->firstIdx() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, firstIdx() > j);}
      if (this->lastIdx() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdx() < j);}
      return (i==j) ? this->asDerived().elt2Impl(i, i) : Type();
    }
//    /** @return safely the ith diagonal element.
//     *  @param i index of the diagonal element
//     **/
//    inline Type& at(int i)  { return at(i);}
    /** @return safely the constant ith diagonal element.
     *  @param i index of the diagonal element
     **/
    inline Type const at(int i) const { return at(i, i);}
    /** @return the Range of the column pos. */
    inline Range rangeRowsInCol(int const& pos) const { return Range(pos,1);}
    /** @return the Range of the row pos. */
    inline Range rangeColsInRow(int const& pos) const { return Range(pos,1);}
};

/** @ingroup Arrays
 *  Specialization for vector_ */
template <class Derived>
class ITContainer<Derived, Arrays::vector_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return the range of the container */
    inline Range range() const  { return this->asDerived().rows();}
    /** @return the index of the first element */
    inline int firstIdx() const { return this->asDerived().range().firstIdx();}
    /**  @return the index of the last element */
    inline int lastIdx() const  { return this->asDerived().range().lastIdx();}
    /**  @return the size of the container */
    inline int size() const  { return this->asDerived().range().size();}
    /**  @return the size of the container */
    inline int sizeRows() const  { return this->asDerived().range().size();}
    /** @return the index of the column of the vector */
    inline int colIdx() const { return this->cols().firstIdx();}
    /** @return the first element */
    inline Type& front() { return this->elt(firstIdx());}
    /** @return the first element */
    inline Type const front() const { return this->elt(firstIdx());}
    /** @return the last element */
    inline Type& back() { return this->elt(lastIdx());}
    /** @return the last element */
    inline Type const back() const { return this->elt(lastIdx());}
    /** @return the range of the effectively stored elements in the column. */
    inline Range rangeRowsInCol(int) const { return this->rows();}
    /** @return the range of the effectively stored elements in the row. */
    inline Range rangeColsInRow(int) const { return this->cols();}
};

/** @ingroup Arrays
 *  Specialization for point_ */
template <class Derived>
class ITContainer<Derived, Arrays::point_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return the range of the container */
    inline Range range() const  { return this->asDerived().cols();}
    /** @return the index of the first element */
    inline int firstIdx() const { return this->asDerived().range().firstIdx();}
    /**  @return the index of the last element */
    inline int lastIdx() const  { return this->asDerived().range().lastIdx();}
    /**  @return the size of the container */
    inline int size() const  { return this->asDerived().range().size();}
    /** @return the index of the row of the point */
    inline int rowIdx() const { return this->rows().firstIdx();}
    /** @return the first element */
    inline Type& front() { return this->elt(firstIdx());}
    /** @return the first element */
    inline Type const front() const { return this->elt(firstIdx());}
    /** @return the last element */
    inline Type& back() { return this->elt(lastIdx());}
    /** @return the last element */
    inline Type const back() const { return this->elt(lastIdx());}
    /** @return the range of the effectively stored elements in the column. */
    inline Range rangeRowsInCol(int) const { return this->rows();}
    /** @return the range of the effectively stored elements in the row. */
    inline Range rangeColsInRow(int) const { return this->cols();}
};

/** @ingroup Arrays
 *  Specialization for number_ */
template <class Derived>
class ITContainer<Derived, Arrays::number_> : public ITContainer2DBase<Derived>
{
  public:
    typedef ITContainer2DBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    /** default constructor. */
    inline ITContainer() : Base() {}
    /** destructor. */
    inline ~ITContainer() {}

  public:
    /** @return the range of the effectively stored elements in the column. */
    inline Range rangeRowsInCol(int) const { return this->rows();}
    /** @return the range of the effectively stored elements in the row. */
    inline Range rangeColsInRow(int) const { return this->cols();}
    /** Conversion to scalar */
    inline operator Type const() const {return this->asDerived().elt0Impl();}
};

/** @ingroup Arrays
 *  @brief Interface class for homogeneous 2D containers which cannot be
 *  expression or part of an expression (like allocators).
 *
 * Use the curious recursive template paradigm : the template
 * parameter @c Derived is the name of the class that
 * implements the interface ITContainer2D.
 * For example
 * @code
 * template<class Type>
 * class Derived : public ITContainer2D< Derived<Type> >
 * {...}
 * @endcode
 *
 * @sa CAllocator
 **/
template < class Derived, int SizeRow_ = hidden::Traits<Derived>::sizeRows_
                        , int SizeCol_ = hidden::Traits<Derived>::sizeCols_>
class ITContainer2D : protected TContainer2D<SizeRow_, SizeCol_>
                    , public ITContainer<Derived, hidden::Traits<Derived>::structure_>
{
  protected:
    /** Type of the Base container */
    typedef TContainer2D<hidden::Traits<Derived>::sizeRows_, hidden::Traits<Derived>::sizeCols_ > Base2D;
    /** Type of the Base container */
    typedef ITContainer<Derived, hidden::Traits<Derived>::structure_ > Base;
    /** Default constructor. Default values are cols=(1:0) and rows=(1:0). */
    inline ITContainer2D() : Base2D(), Base() {}
    /** constructor with specified Range.
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline ITContainer2D( Range const& I, Range const& J) : Base2D(I, J), Base() {}
    /** Copy constructor.
     *  @param T the container to copy
     **/
    inline ITContainer2D( ITContainer2D const& T) : Base2D(T), Base() {}
    /** destructor. */
    inline ~ITContainer2D() {}

  public:
    /**@return the Horizontal range */
    inline Range cols() const { return Base2D::cols();}
    /** @return the index of the first column */
    inline int firstIdxCols() const { return Base2D::firstIdxCols();}
    /**  @return the index of the last column */
    inline int lastIdxCols() const { return Base2D::lastIdxCols();}
    /** @return the Horizontal size (the number of column) */
    inline int sizeCols() const { return Base2D ::sizeColsImpl();}
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
//    /** @return the ith element for vector_, point_ and diagonal_ containers
//     *  @param i index of the ith element
//     **/
//    inline Type& elt(int const& i) { return this->asDerived().elt1Impl(i);}
//    /** @return the element ith element
//     *  @param i index of the ith element
//     **/
//    inline Type& operator[](int const& i) { return this->asDerived().elt1Impl(i);}
//    /** @return safely the jth element
//     *  @param i index of the element
//     **/
//    Type& at(int i)
//    {
//      if (this->asDerived().firstIdx() > i)
//      { STKOUT_OF_RANGE_1ARG(ITContainer::at, i, firstIdx() > i);}
//      if (this->asDerived().lastIdx() < i)
//      { STKOUT_OF_RANGE_1ARG(ITContainer::at, i, lastIdx() < i);}
//      return this->asDerived().elt1Impl(i);
//    }
//    /** @return the number */
//    inline Type& elt() { return this->asDerived().elt0Impl();}
//    /** @return the number */
//    inline Type& operator()() { return this->asDerived().elt0Impl();}
};

} // namespace STK

#endif
// STK_ITCONTAINER2D_H
