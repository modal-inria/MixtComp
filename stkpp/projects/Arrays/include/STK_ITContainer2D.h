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
#include "Sdk/include/STK_StaticAssert.h"

#include "STK_Traits.h"
#include "STK_Arrays_Util.h"

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
 * in the derived classes have the following definitions for the dimensions:
 * @code
 * Range cols() const;
 * int beginColsImpl() const;
 * int endColsImpl() const;
 * int sizeColsImpl() const;
 * Range rows() const;
 * int beginRowsImpl() const;
 * int endRowsImpl() const;
 * int sizeRowsImpl() const;
 * @endcode
 * and for the accessors the following definitions:
 * @code
 *   Type const elt2Impl( int i, int j) const;
 *   Type& elt2Impl( int i, int j);
 * @endcode
 * for the 2D arrays (@ref Arrays::array2D_, @ref Arrays::square_, @ref Arrays::upper_triangular_
 * and @ref Arrays::lower_triangular_),
 * @code
 *   Type const elt1Impl( int i) const;
 *   Type& elt1Impl( int i);
 * @endcode
 * for the diagonal arrays (@ref Arrays::diagonal_) and the 1D arrays
 * (@ref Arrays::vector_, @ref Arrays::point_)
 * and
 * @code
 *   Type const elt0Impl() const;
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
    inline int beginCols() const { return this->asDerived().beginColsImpl();}
    /**  @return the ending index of the columns */
    inline int endCols() const { return this->asDerived().endColsImpl();}
    /** @return the Horizontal size (the number of column) */
    inline int sizeCols() const { return this->asDerived().sizeColsImpl();}

    /** @return the Vertical range*/
    inline Range rows() const { return this->asDerived().rows();}
    /** @return the index of the first row*/
    inline int beginRows() const { return this->asDerived().beginRowsImpl();}
    /** @return the ending index of the rows*/
    inline int endRows() const { return this->asDerived().endRowsImpl();}
    /** @return the Vertical size (the number of rows) */
    inline int sizeRows() const { return this->asDerived().sizeRowsImpl();}

    /**  @return the index of the first column */
    inline int firstIdxCols() const { return this->asDerived().cols().begin();}
    /** @return the index of the last column */
    inline int lastIdxCols() const { return this->asDerived().cols().lastIdx();}
    /** @return the index of the first row*/
    inline int firstIdxRows() const { return this->asDerived().rows().begin();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return this->asDerived().rows().lastIdx();}

    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return (sizeCols()<=0 || sizeRows()<=0);}

    /** @return the size of the container (the number of rows by the number of columns */
    inline int sizeArray() const { return sizeRows()*sizeCols();}
    /** @return a constant reference on element (i,j) of the 2D container
     *  @param i, j indexes of the row and of the column
     **/
    inline Type const elt(int i, int j) const
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, endCols() <= j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return a constant reference on the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const operator()(int i, int j) const
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2DBase::elt, i, j, endCols() <= j);}
#endif
      return this->asDerived().elt2Impl(i,j);
    }
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const at(int i, int j) const
    {
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, beginRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, beginCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return this->asDerived().elt2Impl(i, j);
    }
    /** @return the constant ith element
     *  @param i index of the ith element
     **/
    inline Type const elt(int i) const { return this->asDerived().elt1Impl(i);}
    /** @return the ith element
     *  @param i index of the ith element
     **/
    inline Type const operator[](int i) const { return this->asDerived().elt1Impl(i);}
    /** @return safely the constant ith element
     *  @param i index of the element
     **/
    Type const at(int i) const
    {
      if (this->asDerived().begin() > i)
      { STKOUT_OF_RANGE_1ARG(ITContainer::at, i, begin() > i);}
      if (this->asDerived().end() <= i)
      { STKOUT_OF_RANGE_1ARG(ITContainer::at, i, end() <= i);}
      return this->asDerived().elt1Impl(i);
    }
    /** @return a constant reference on the number */
    inline Type const elt() const { return this->asDerived().elt0Impl();}
    /** @return a constant reference on the number */
    inline Type const operator()() const { return this->asDerived().elt0Impl();}
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
    inline int begin() const { return this->asDerived().beginRowsImpl();}
    /** @return the ending index of the square container. */
    inline int end() const { return this->asDerived().endRowsImpl();}
    /** @return the size of the rows and columns of the container. */
    inline int size() const { return this->asDerived().sizeRowsImpl();}

    /** @return the first index of the square container. */
    inline int firstIdx() const { return this->asDerived().rows().begin();}
    /** @return the last index of the square container. */
    inline int lastIdx() const { return this->asDerived().rows().lastIdx();}

    /** compute the range of the stored elements in a column. */
    inline Range rangeRowsInCol(int) const { return this->asDerived().rows();}
    /** compute the range of the stored elements in a row. */
    inline Range rangeColsInRow(int) const { return this->asDerived().rows();}

    Type const trace() const
    {
      Type sum = 0.0;
      for (int k = begin(); k< end(); k++) sum += this->elt(k, k);
      return sum;
    }
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
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, beginRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, beginCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return (j>i) ? Type() : this->asDerived().elt2Impl(i, j);
    }
    /** @return the Range of the effectively stored elements in the column @c icol.
     *  @param icol the number of the column to compute the range
     **/
    inline Range rangeRowsInCol( int const& icol) const
    { return Range(icol, this->lastIdxRows(), 0);}
    /** compute the range of the effectively stored elements in the row @c irow.
     *  @param irow the index of the row
     **/
    inline Range rangeColsInRow( int const& irow) const
    { return Range(this->beginCols(), std::min(irow, this->lastIdxCols()), 0);}
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
     *  @param i, j indexes of the row and of the column
     **/
    Type const at(int i, int j) const
    {
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, beginRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxRows() < i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, beginCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdxCols() < j);}
      return (i>j) ? Type() : this->asDerived().elt2Impl(i, j);
    }
    /** @return the range of the effectively stored elements in the row @c irow.
     *  @param irow the index of the row we want to compute the range
     **/
    inline Range rangeColsInRow( int irow) const
    { return Range(irow, this->lastIdxCols(), 0);}
    /** compute the range of the effectively stored elements in the col
     *  icol.
     *  @param icol the number of the column to compute the range
     **/
    inline Range rangeRowsInCol( int icol) const
    { return Range(this->beginRows(), std::min(icol, this->lastIdxRows()), 0);}
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
    inline Range range() const { return this->asDerived().rows();}
    /** @return the first index of the diagonal container. */
    inline int begin() const { return this->asDerived().beginRowsImpl();}
    /** @return the ending index of the diagonal container. */
    inline int end() const { return this->asDerived().endRowsImpl();}
    /** @return the size of the rows and columns of the container. */
    inline int size() const { return this->asDerived().sizeRowsImpl();}

    /** @return the first index of the diagonal container. */
    inline int firstIdx() const { return this->asDerived().rows().begin();}
    /** @return the last index of the square container. */
    inline int lastIdx() const { return this->asDerived().rows().lastIdx();}
    /** @return the first diagonal element */
    inline Type const front() const { return elt(begin());}
    /** @return the last diagonal element */
    inline Type const back() const { return elt(begin());}
    /** @return safely the constant element (i, j).
     *  @param i index of the row
     *  @param j index of the col
     **/
    Type const at(int i, int j) const
    {
      if (this->begin() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, begin() > i);}
      if (this->lastIdx() < i)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdx() < i);}
      if (this->begin() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, begin() > j);}
      if (this->lastIdx() < j)
      { STKOUT_OF_RANGE_2ARG(ITContainer::at, i, j, lastIdx() < j);}
      return (i==j) ? this->asDerived().elt2Impl(i, i) : Type();
    }
    /** @return safely the constant ith diagonal element.
     *  @param i index of the diagonal element
     **/
    inline Type const at(int i) const { return at(i, i);}
    /** @return the Range of the column pos. */
    inline Range rangeRowsInCol(int pos) const { return Range(pos,1);}
    /** @return the Range of the row pos. */
    inline Range rangeColsInRow(int pos) const { return Range(pos,1);}
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
    inline int begin() const { return this->asDerived().beginRowsImpl();}
    /**  @return the ending index of the elements */
    inline int end() const  { return this->asDerived().endRowsImpl();}
    /**  @return the size of the vector */
    inline int size() const  { return this->asDerived().sizeRowsImpl();}

    /**  @return the index of the last element */
    inline int lastIdx() const  { return this->asDerived().range().lastIdx();}
    /** @return the index of the column of the vector */
    inline int colIdx() const { return this->cols().begin();}

    /** @return the first element */
    inline Type const front() const { return this->elt(begin());}
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
    inline int begin() const { return this->asDerived().beginColsImpl();}
    /**  @return the ending index of the elements */
    inline int end() const  { return this->asDerived().endColsImpl();}
    /**  @return the size of the container */
    inline int size() const  { return this->asDerived().sizeColsImpl();}

    /** @return the index of the row of the point */
    inline int rowIdx() const { return this->rows().begin();}
    /**  @return the index of the last element */
    inline int lastIdx() const  { return this->asDerived().cols().lastIdx();}

    /** @return the first element */
    inline Type const front() const { return this->elt(begin());}
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
 *  @brief Interface class for 2D containers.
 *
 *  The TContainer2D class is the base class for all two-dimensional containers.
 *  A two-dimensional container is defined by an horizontal range of index
 *  for the columns and a vectical range of index for the rows.
 **/
template<int SizeRows_, int SizeCols_>
class IContainer2D
{
  public:
    /** Type of the Range for the rows */
    typedef TRange<SizeRows_> RowRange;
    /** Type of the Range for the columns */
    typedef TRange<SizeCols_> ColRange;

  protected:
    /** Default constructor. cols_ = 1:0 and rows_ = 1:0. */
    inline IContainer2D() : rows_(), cols_() {}
    /** Constructor with specified ranges
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline IContainer2D( RowRange const& I, ColRange const& J) : rows_(I), cols_(J) {}
    /** Copy constructor
     *  @param T the container to copy
     **/
    inline IContainer2D( IContainer2D const& T) : rows_(T.rows_), cols_(T.cols_) {}
    /** destructor. **/
    inline ~IContainer2D() {}

    /** @return the range of the columns */
    inline Range cols() const { return Range(beginColsImpl(), sizeColsImpl());}
    /** @return the range of the columns */
    inline ColRange colsImpl() const { return cols_;}
    /** @return the index of the first column */
    inline int beginColsImpl() const { return cols_.begin();}
    /** @return the ending index of the columns */
    inline int endColsImpl() const { return cols_.end();}
    /** @return the number of column */
    inline int sizeColsImpl() const { return cols_.size();}

    /** @return the range of the rows */
    inline Range rows() const { return Range(beginRowsImpl(), sizeRowsImpl());}
    /** @return the range of the rows */
    inline RowRange rowsImpl() const { return rows_;}
    /** @return the index of the first row */
    inline int beginRowsImpl() const { return rows_.begin();}
    /** @return the ending index of rows */
    inline int endRowsImpl() const { return rows_.end();}
    /** @return the number of rows */
    inline int sizeRowsImpl() const { return rows_.size();}

    /** @return the index of the first column */
    inline int firstIdxCols() const { return cols_.begin();}
    /** @return the index of the last column */
    inline int lastIdxCols() const { return cols_.lastIdx();}
    /** @return the index of the first row */
    inline int firstIdxRows() const { return rows_.begin();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return rows_.lastIdx();}

    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return (cols_.empty() || rows_.empty());}

    /** Set the first index of the rows and columns.
     *  @param rbeg the first index of the rows
     *  @param cbeg the first index of the columns
     **/
    inline void shift( int rbeg, int cbeg) { rows_.shift(rbeg); cols_.shift(cbeg);}
    /** Set the ranges of the container.
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline void setRanges(RowRange const& I = RowRange(), ColRange const& J = ColRange())
    { rows_ = I; cols_ =J;}
    /** Set the range of the number of rows.
     *  @param I the range of the rows number
     **/
    inline void setRows( RowRange const& I = RowRange()) { rows_ = I;}
    /** Set the first index of the rows.
     *  @param beg the first index of the rows
     **/
    inline void shiftFirstIdxRows( int beg) { rows_.shift(beg);}
    /** Increment the range of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incRangeRows( int inc) { rows_.inc(inc);}
    /** Increment the first index of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incFirstIdxRows( int inc) { rows_.incFirst(inc);}
    /** Decrement the first index of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decFirstIdxRows( int dec) { rows_.decFirst(dec);}
    /** Increment the end of the number of rows.
     *  @param inc the increment to apply
     **/
    inline void incLastIdxRows( int inc) { rows_.incLast(inc);}
    /** Decrement the end of the number of rows.
     *  @param dec the decrement to apply
     **/
    inline void decLastIdxRows( int dec) { rows_.decLast(dec);}

    /** Set the range of the columns.
     * @param J the range of the cols number
     **/
    inline void setCols( ColRange const& J = ColRange()) { cols_ = J;}
    /** Shift the first index of the columns to beg.
     *  @param beg the new first index
     **/
    inline void shiftbeginCols( int beg) { cols_.shift(beg);}
    /** Increment the range of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incRangeCols( int inc) { cols_.inc(inc);}
    /** increment the first index of the number of columns.
     *  @param inc the increment to apply
     **/
    inline void incbeginCols( int inc) { cols_.incFirst(inc);}
    /** Decrement the first index of the columns.
     *  @param dec the decrement to apply
     **/
    inline void decbeginCols( int dec) { cols_.decFirst(dec);}
    /** Increment the last index of the columns.
     *  @param inc the increment to apply
     **/
    inline void incLastIdxCols( int inc)  { cols_.incLast(inc);}
    /** Decrement the last index of the columns.
     *  @param dec the decrement to apply
     **/
    inline void decLastIdxCols( int dec) { cols_.decLast(dec);}
    /** exchange this container with T
     * @param T the container to exchange with this
     **/
     inline void exchange(IContainer2D& T)
     {
       std::swap(T.rows_, this->rows_ );
       std::swap(T.cols_, this->cols_ );
     }
  private:
    /** Vertical range : Range of the indexes for the rows. */
    RowRange rows_;
    /** Horizontal range : Range of the indexes for the columns. */
    ColRange cols_;
};

/** @ingroup Arrays
 *
 * @brief Interface class for homogeneous 2D containers which cannot be
 * expression or part of an expression (like allocators).
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
class ITContainer2D : protected IContainer2D<SizeRow_, SizeCol_>
                    , public ITContainer<Derived, hidden::Traits<Derived>::structure_>
{
  protected:
    /** Type of the Base container */
    typedef IContainer2D<hidden::Traits<Derived>::sizeRows_, hidden::Traits<Derived>::sizeCols_ > Base2D;
    /** Type of the Base container */
    typedef ITContainer<Derived, hidden::Traits<Derived>::structure_ > Base;
    typedef typename hidden::Traits<Derived>::Type Type;
    /** Default constructor.*/
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
    inline int beginColsImpl() const { return Base2D::beginColsImpl();}
    /**  @return the ending index of the columns */
    inline int endColsImpl() const { return Base2D::endColsImpl();}
    /** @return the number of columns */
    inline int sizeColsImpl() const { return Base2D::sizeColsImpl();}

    /** @return the Vertical range */
    inline Range rows() const { return Base2D::rows();}
    /** @return the index of the first row */
    inline int beginRowsImpl() const { return Base2D::beginRowsImpl();}
    /** @return the ending index of the rows */
    inline int endRowsImpl() const { return Base2D::endRowsImpl();}
    /** @return the Vertical size (the number of rows) */
    inline int sizeRowsImpl() const { return Base2D::sizeRowsImpl();}

    /** @return the index of the first column */
    inline int firstIdxCols() const { return Base2D::firstIdxCols();}
    /**  @return the index of the last column */
    inline int lastIdxCols() const { return Base2D::lastIdxCols();}
    /** @return the index of the first row */
    inline int firstIdxRows() const { return Base2D::firstIdxRows();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return Base2D::lastIdxRows();}

    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return Base2D::empty();}
    /** @return the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type& elt(int i, int j)
    {
#ifdef STK_BOUNDS_CHECK
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, endCols() <= j);}
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
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, endRows() <= i);}
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_2ARG(ITContainer2D::elt, i, j, endCols() <= j);}
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
};

} // namespace STK

#endif
// STK_ITCONTAINER2D_H
