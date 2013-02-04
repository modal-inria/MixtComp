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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::hidden::TContainer
 * created on: 26 nov. 2011
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_ITContainer2D.h
 *  @brief In this file we define the Interface class ITContainer2D.
 **/

#ifndef STK_ITCONTAINER2D_H
#define STK_ITCONTAINER2D_H

#include "../../Sdk/include/STK_Traits.h"
#include "../../STKernel/include/STK_Exceptions.h"
#include "../../STKernel/include/STK_Macros.h"

#include "STK_ITContainer.h"
#include "STK_IContainer2D.h"

namespace STK
{

/** @ingroup Arrays
 *  @brief Interface class for homogeneous 2D containers whcih cannot be
 *  expression (like allocators)
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
 * The pseudo virtual function defined in this interface and to implement
 * in the derived class have the following definitions:
 * @code
 *   Type& elt( int const& i, int const& j);
 *   Type const& elt( int const& i, int const& j) const;
 *   Col col( int const& j) const;
 *   SubCol col( Range const& I, int const& j) const;
 *   Row row( int const& i) const;
 *   SubRow row( int const& i, Range const& J) const;
 *   SubArray sub( Range const& I, Range const& J) const;
 *   void shift( int const& rbeg, int const& cbeg);
 *   void resize( Range const& I, Range const& J);
 * @endcode
 *
 * A @c Column is a class that allow to access to (or a part of) a
 * column of the @c Derived class
 *
 * A @c Row is a class that allow to access to (or a part of) a
 * a row of the @c Derived class.
 *
 * ITContainer2D is a general interface that can be used as a contract in the
 * class and methods using two dimensional allocators.
 *
 * @sa CAllocator
 **/
template <class Derived>
class ITContainer2D : public IContainer2D
                    , public ITContainer<Derived, (Arrays::Structure)hidden::Traits<Derived>::structure_>
{
  protected:
    /** Type of the Base container */
    typedef ITContainer<Derived, (Arrays::Structure)hidden::Traits<Derived>::structure_ > Base;
    /** Default constructor. Default values are cols=(1:0) and rows=(1:0). */
    inline ITContainer2D() : IContainer2D(), Base() {}
    /** constructor with specified Range.
     *  @param I the vertical range
     *  @param J the horizontal range
     **/
    inline ITContainer2D( Range const& I, Range const& J) : IContainer2D(I, J), Base() {}
    /** Copy constructor.
     *  @param T the container to copy
     **/
    inline ITContainer2D( ITContainer2D const& T) : IContainer2D(T), Base() {}
    /** destructor. */
    inline ~ITContainer2D() {}

  public:
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::Row Row;
    typedef typename hidden::Traits<Derived>::Col Col;
    typedef typename hidden::Traits<Derived>::SubRow SubRow;
    typedef typename hidden::Traits<Derived>::SubCol SubCol;
    typedef typename hidden::Traits<Derived>::SubArray SubArray;

    /**@return the Horizontal range */
    inline Range const& cols() const { return IContainer2D::cols();}
    /** @return the index of the first column */
    inline int const& firstIdxCols() const { return IContainer2D::firstIdxCols();}
    /**  @return the index of the last column */
    inline int const& lastIdxCols() const { return IContainer2D::lastIdxCols();}
    /** @return the Horizontal size (the number of column) */
    inline int const& sizeCols() const { return IContainer2D::sizeCols();}
    /** @return the Vertical range */
    inline Range const& rows() const { return IContainer2D::rows();}
    /** @return the index of the first row */
    inline int firstIdxRows() const { return IContainer2D::firstIdxRows();}
    /** @return the index of the last row */
    inline int const& lastIdxRows() const { return IContainer2D::lastIdxRows();}
    /** @return the Vertical size (the number of rows) */
    inline int const& sizeRows() const { return IContainer2D::sizeRows();}
    /**  @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return IContainer2D::empty();}
    /** access to one column.
     *  @param j index of the column
     *  @return a Vertical container containing the column @c j of the Container
     **/
    inline Col col(int const& j) const
    { return this->asDerived().col(j);}
    /** access to many columns.
     *  @param J range of the index of the cols
     *  @return a 2D container containing the Container in the Horizontal range @c J
     **/
    inline SubArray col(Range const& J) const
    { return this->asDerived().col(J);}
    /** Operator () : access to one row.
     *  @param i index of the row
     *  @return an Horizontal container containing the row @c i of the Container
     **/
    inline Row row(int const& i) const
    { return this->asDerived().row(i);}
    /** access to many rows.
     *  @param I range of the index of the rows
     *  @return a 2D container containing the Container in the vertical range @c I
     **/
    inline SubArray row(Range const& I) const
    { return this->asDerived().row(I);}
    /**  return the column j.
     *  @param j index of the column
     **/
    inline Col atCol(int const& j) const
    {
      if (this->firstIdxCols() > j)
      { STKOUT_OF_RANGE_1ARG(ITContainer2D::atCol, j, firstIdxCols() > j);}
      if (this->lastIdxCols() < j)
      { STKOUT_OF_RANGE_1ARG(ITContainer2D::atCol, j, lastIdxCols() < j);}
      return this->asDerived().col(j);
    }
    /** @return the row i.
     *  @param i the index of the row
     **/
    inline Row atRow(int const& i) const
    {
      if (this->firstIdxRows() > i)
      { STKOUT_OF_RANGE_1ARG(ITContainer2D::atRow, i, firstIdxRows() > i);}
      if (this->lastIdxRows() < i)
      { STKOUT_OF_RANGE_1ARG(ITContainer2D::at, i, lastIdxRows() < i);}
      return this->asDerived().row(i);
    }
    /** Shift the ranges of indexes of the container.
     * @param rbeg the first index for the rows of the container
     * @param cbeg the first index for the columns of the container
     **/
    inline void shift( int const& rbeg, int const& cbeg)
    { this->asDerived().shift(rbeg, cbeg);}
    /** @return the resized container.
     * @param I the new range of index for the rows of the container
     * @param J the new range of index for the columns of the container
     **/
    inline Derived& resize( Range const& I, Range const& J)
    { return this->asDerived().resize(I, J);}
};

} // namespace STK

#endif
// STK_ITCONTAINER2D_H
