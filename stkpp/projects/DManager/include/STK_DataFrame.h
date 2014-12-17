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
 * Project:  stkpp::DManager
 * Purpose:  Define the DataFrame class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_DataFrame.h
 *  @brief In this file we declare the DataFrame class.
 **/

#ifndef STK_DATAFRAME_H
#define STK_DATAFRAME_H

#include "Arrays/include/STK_ITContainer2D.h"

#include "STK_List1D.h"
#include "STK_IVariable.h"


namespace STK
{

/** @ingroup DManager
  * @brief DataFrame is a List of Variable with the same number of rows.
  *  This is thus also a 2D container.
  *
  * A DataFrame inherit from the class @c List1D and and @c IContainer2D.
  * It is essentially a List, except that each column (the variables)
  * possess the same dimension.
  *
  * Each Cell of the List1D contain a pointer on a Variable.
 **/
class DataFrame : protected List1D<IVariable* >, public IContainer2D<UnknownSize,UnknownSize>
{
  protected:
    /** Type of the Base container */
    typedef IContainer2D<UnknownSize, UnknownSize > Base2D;
    /** Type for the list container. */
    typedef List1D<IVariable*> Base;

  public:
    /** Default Constructor, empty table. */
    DataFrame();
    /** Copy constructor If ref is true, only references of the variables
     *  are copied into the DataFrame.
     *  @param T the DataFrame to copy
     *  @param ref true if we want to wrap the variables of T
     **/
    DataFrame( DataFrame const& T, bool ref = false);
    /** Destructor. */
    virtual ~DataFrame();

    /**@return the range of the columns */
    inline Range cols() const { return Base2D::cols();}
    /** @return the index of the first column */
    inline int beginCols() const { return Base2D::beginColsImpl();}
    /**  @return the ending index of the columns */
    inline int endCols() const { return Base2D::endColsImpl();}
    /** @return the number of columns */
    inline int sizeCols() const { return Base2D ::sizeColsImpl();}

    /** @return the range of the rows */
    inline Range rows() const { return Base2D::rows();}
    /** @return the index of the first row */
    inline int beginRows() const { return Base2D::beginRowsImpl();}
    /** @return the ending index of the rows */
    inline int endRows() const { return Base2D::endRowsImpl();}
    /** @return the number of rows */
    inline int sizeRows() const { return Base2D::sizeRowsImpl();}

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

    /** Clear the object. */
    void clear();
    /** access to an element. Set the method elt as a public method. */
    inline IVariable*& elt(int const& i) { return Base::elt(i);}
    /** access to a constant element. Set the method elt as a public method. */
    inline IVariable* const elt(int const& i) const { return Base::elt(i);}

    /** Operator = : overwrite the DataFrame with T.*/
    DataFrame& operator=(DataFrame const& T);

    /** resize the container:
     * - call @c shift(I.begin(), J.begin()
     * - call @c popBackCols() (@c insertRows()) and/or @c popBackCols()
     *  (@c popBackRows()).
     *  The implicit assumption made by this method is that it is easier and
     *  faster to add column than add rows to the 2D container.
     * @param I the new range for the rows of the container
     * @param J the new range for the columns of the container
     **/
    void resize( Range const& I, Range const& J);
    /** New beginning index for the object.
     *  @param cbeg the index of the first column of the data-frame
     **/
    void shift(int const& cbeg =1);
     /** New beginning index for the object.
      *  @param rbeg the index of the first row of the data-frame
      *  @param cbeg the index of the first column of the data-frame
      **/
    void shift(int const& rbeg, int const& cbeg);
    /** Delete n columns of the container. */
    void popBackCols(int const& n);
    /** Delete n columns at the position of the container.*/
    void eraseCols( int pos, int const& n=1);
    /** Swapping the j1th column and the j2th column.
     *  @param j1 index of the first column to swap
     *  @param j2 index of the second column to swap
     * */
    inline void swapCols(int j1, int j2)
    { Base::swap(j1, j2);}

    /** Dell last rows of the container.
     *  @param n number of rows to delete
     **/
    void popBackRows(int const& n);
    /** Delete n rows at the position @c pos to the container.
     *  @param pos position of the rows to delete
     *  @param  n  number of rows to delete
     **/
    void eraseRows( int pos, int const& n=1);
    /** Insert a Vartiable at the specified position to the container.
     *  @param pos the position in the container
     *  @param V the Variable to insert
     **/
    void insertVariable( int pos, IVariable* const & V);
    /** Append a DataFrame back.
     *  @param V The variable to append to the DataFrame
     **/
    void pushBackVariable( IVariable* const & V);
    /** Append a DataFrame front. */
    inline void pushFrontVariable( IVariable* const & V)
    { insertVariable(beginCols(), V);}
    /** merge this DataFrame with D. */
    void merge( DataFrame const& D);
    /** Insert a DataFrame at the specified position to the container. */
    void insertDataFrame( int pos, DataFrame const& D);
    /** Append a DataFrame back. */
    void pushBackDataFrame( DataFrame const &D);
    /** Append a DataFrame front. */
    inline void pushFrontDataFrame( DataFrame const &D)
    { insertDataFrame(beginCols(), D);}
    /** write a DataFrame to the output stream os. */
    void writeDataFrame( ostream  &os, int const& left, int const& right) const;

  protected:
    /** function for memory deallocation. */
    void freeMem();
    /** function for row memory deallocation. */
    void freeCols();
    /** Add columns to the container. */
    void pushBackCols(int const& n=1);
    /** Insert columns at the specified position to the container. */
    void insertCols( int pos, int const& n=1);
    /** Add n rows to the container.  */
    void pushBackRows(int const& n=1);
    /** Insert n rows at the ith position of the container. */
    void insertRows( int pos, int const& n =1);
};

/** Print a DataFrame.
 * @param s ooutput stream
 * @param V the Dataframe to write
 **/
ostream& operator<< (ostream& s, DataFrame const& V);

} // namespace STK

#endif
//STK_DATAFRAME_H
