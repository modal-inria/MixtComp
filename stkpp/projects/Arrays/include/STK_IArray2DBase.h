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
 * Purpose:  Define the Interface for the Array classes.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IArray2DBase.h
 *  @brief Interface base class for the Array2D classes, this is an internal
 *  header file, included by other containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class like Array2D, except if you want to create your own
 *  Container Class.
 **/

#ifndef STK_IARRAY2DBASE_H
#define STK_IARRAY2DBASE_H

#include "STK_ArrayBase.h"

#include "STK_ExprBaseProduct.h"
#include "STK_ExprBaseDot.h"
#include "STK_ExprBaseVisitor.h"

#include "STK_ArrayBaseApplier.h"
#include "STK_ArrayBaseAssign.h"
#include "STK_ArrayBaseInitializer.h"

#include "STK_Array1D.h"

namespace STK
{
/** @ingroup Arrays
 *  @brief Templated interface base class for two-dimensional arrays.
 *
 * A IArray2DBase is an interface class for two-dimensional Arrays
 * stored in columns and having flexible dimensions. It is possible
 * to add, remove easily columns and rows to the Derived class.
 *
 * Each column has a Range stored in the array @c rangeCols_ and a
 * capacity stored in the array @c capacityCols_. It should be worth
 * noting that we should have
 * @code
 *   (rangeCols_[j].size() <= capacityCols_[j]) == true;
 *   (rangeCols_[j].isIn(this->rows()) == true;
 * @endcode
 *
 * @tparam PTRCOL is the type of the ptr of column in a two-dimensional
 * array: for exemple @c TYPE*, @c Array1D<TYPE>*, @c DBACCESS*....
 * @tparam Derived is the name of the class that implements @c IArray2DBase.
 **/
template < class PTRCOL, class Derived
         , int SizeRow_ = hidden::Traits<Derived>::sizeRows_
         , int SizeCol_ = hidden::Traits<Derived>::sizeCols_>
class IArray2DBase :  protected IContainer2D<SizeRow_, SizeCol_>, public ArrayBase<Derived>
{
  public:
    /** Type for the IContainer2D base Class. */
    typedef IContainer2D<SizeRow_, SizeCol_ > Base2D;
    /** Type for the Base Class. */
    typedef AllocatorBase<PTRCOL> Allocator;
    /** type of the Base Container Class. */
    typedef ArrayBase<Derived> Base;

    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::Row RowVector;
    typedef typename hidden::Traits<Derived>::Col ColVector;
    typedef typename hidden::Traits<Derived>::SubRow SubRowVector;
    typedef typename hidden::Traits<Derived>::SubCol SubColVector;
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
    /** Default constructor */
    IArray2DBase(): Base2D(), Base()
                  , allocator_()
                  , capacityCols_()
                  , rangeCols_()
                  , capacityHo_(0)
    { mallocHo(this->cols());}
    /** constructor with specified ranges
     *  @param I range of the Rows
     *  @param J range of the columns
     **/
    IArray2DBase( Range const& I, Range const& J)
                : Base2D(I, J), Base()
                , allocator_()
                , capacityCols_()
                , rangeCols_()
                , capacityHo_(0)
    { mallocHo(this->cols());}
    /** Copy constructor If we want to wrap T, the main ptr will be wrapped
     *  in AllocatorBase class. If we want to copy  T, AllocatorBase is
     *  initialized to default values.
     *  @param T the container to copy
     *  @param ref true if we wrap T
     **/
    IArray2DBase( IArray2DBase const& T, bool ref =false)
                : Base2D(T), Base()
                , allocator_(T.allocator_, ref)
                , capacityCols_(T.capacityCols_)
                , rangeCols_(T.rangeCols_)
                , capacityHo_(T.capacityHo_)
    { if (!ref) mallocHo(this->cols());}
    /** constructor by reference, ref_=1.
     *  @param T the container to copy
     *  @param I range of the Rows to wrap
     *  @param J range of the column to wrap
     **/
    template<class Rhs>
    IArray2DBase( IArray2DBase<PTRCOL, Rhs> const& T, Range const& I, Range const& J)
                : Base2D(I, J), Base()
                , allocator_(T.allocator(), true)
                , capacityCols_(T.capacityCols(), J)
                , rangeCols_(J)
                , capacityHo_(J.size())
    {
      for (int j=J.begin(); j<J.end(); j++)
      { // compute available wrapped range of the column j
        rangeCols_[j] = Range::inf(I, T.rangeCols()[j]);
      }
    }
    /** Wrapper constructor We get a reference of the data.
     *  @param q pointer on data
     *  @param I range of the Rows to wrap
     *  @param J range of the columns to wrap
     **/
    IArray2DBase( PTRCOL* q, Range const& I, Range const& J)
                : Base2D(I, J), Base()
                , allocator_(q, J, true)
                , capacityCols_(J, I.size())
                , rangeCols_(J, I)
                , capacityHo_(0)
    {}
    /** destructor. Allocated horizontal memory (the array with the pointers
     *  on the columns) is liberated by the Allocator.
     **/
    ~IArray2DBase() {}

  public:
    /** @return the Vertical range */
    inline Range rows() const { return Base2D::rows();}
    /** @return the index of the first row */
    inline int beginRowsImpl() const { return Base2D::beginRowsImpl();}
    /** @return the ending index of the rows */
    inline int endRowsImpl() const { return Base2D::endRowsImpl();}
    /** @return the number of rows */
    inline int sizeRowsImpl() const { return Base2D::sizeRowsImpl();}

    /**@return the Horizontal range */
    inline Range cols() const { return Base2D::cols();}
    /** @return the index of the first column */
    inline int beginColsImpl() const { return Base2D::beginColsImpl();}
    /**  @return the ending index of columns */
    inline int endColsImpl() const { return Base2D::endColsImpl();}
    /** @return the number of columns */
    inline int sizeColsImpl() const { return Base2D ::sizeColsImpl();}

    /** @return the index of the first column */
    inline int firstIdxCols() const { return Base2D::firstIdxCols();}
    /**  @return the index of the last column */
    inline int lastIdxCols() const { return Base2D::lastIdxCols();}
    /** @return the index of the first row */
    inline int firstIdxRows() const { return Base2D::firstIdxRows();}
    /** @return the index of the last row */
    inline int lastIdxRows() const { return Base2D::lastIdxRows();}

    /**  @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return Base2D::empty();}

    /** @return a constant pointer on the j-th column of the container
     *  @param j the index of the column
     **/
    inline PTRCOL const& data(int j) const { return allocator_.data(j);}
    /** @return a constant pointer on the main pointer of the container */
    inline PTRCOL* const& p_data() const { return allocator_.p_data();}
    /**  @return @c true if the container is empty, @c false otherwise */
    inline bool isRef() const { return allocator_.isRef();}
    /** @return a reference on the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type& operator()(int i, int j) { return this->elt(i,j);}
    /** @return a constant reference on the element (i,j) of the 2D container.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const operator()(int i, int j) const { return this->elt(i,j);}
    /** @param I range of the index of the rows
     *  @param j index of the col
     *  @return a Vertical container containing the column @c j of this
     *  in the range @c I
     **/
    inline SubColVector operator()(Range const& I, int j) const
    { return this->asDerived().col(I, j);}
    /** @param i index of the row
     *  @param J index of the col
     *  @return an Horizontal container containing the row @c i of this
     *  in the range @c J
     **/
    inline SubRowVector operator()(int i, Range const& J) const
    { return this->asDerived().row(i, J);}
    /** @param I range of the index of the rows
     *  @param J range of the index of the cols
     *  @return a 2D container containing this in the range @c I, @c J
     **/
    inline SubArray operator()(Range const& I, Range const& J) const
    { return this->asDerived().sub(I, J);}
    /** @return the column j.
     *  @param j index of the column
     **/
    inline ColVector atCol(int j) const
    {
      if (this->beginCols() > j)
      { STKOUT_OF_RANGE_1ARG(IArray2DBase::atCol, j, beginCols() > j);}
      if (this->endCols() <= j)
      { STKOUT_OF_RANGE_1ARG(IArray2DBase::atCol, j, endCols() <= j);}
      return this->asDerived().col(j);
    }
    /** @return the row i.
     *  @param i the index of the row
     **/
    inline RowVector atRow(int i) const
    {
      if (this->beginRows() > i)
      { STKOUT_OF_RANGE_1ARG(IArray2DBase::atRow, i, beginRows() > i);}
      if (this->endRows() <= i)
      { STKOUT_OF_RANGE_1ARG(IArray2DBase::at, i, lastIdxRows() < i);}
      return this->asDerived().row(i);
    }
    /** @return the allocator. */
    inline Allocator const& allocator() const { return allocator_;}
    /** @return the allocator. */
    inline Allocator& allocator() { return allocator_;}
    /** @return the maximum possible number of columns without reallocation. */
    inline int capacityHo() const { return capacityHo_;}
    /** @return the maximum possible number of rows without reallocation for all Cols. */
    inline const Array1D<int> & capacityCols() const { return capacityCols_;}
    /** @return the capacity of the column @c col.
     *  @param col index of the column we want the capacity
     **/
    inline int capacityCol(int col) const { return capacityCols_[col];}
    /** @return the range of the columns. */
    inline Array1D<Range> const& rangeCols() const { return rangeCols_;}
    /** @return the range of a column.
     *  @param col index of the column we want the range
     **/
    inline Range const rangeCol(int col) const { return rangeCols_[col];}
    /** Reserve memory for the columns.
     *  @param size the size to reserve.
     **/
    void reserveCols(int size)
    {
      if (capacityHo_ >= size) return;
      Range J(this->beginCols(), size);
      // try to allocate memory
      try
      {
        // re-allocate memory for the columns
        allocator_.realloc(J);
        // initialize this->capacityCols_
        capacityCols_.resize(J);
        // initialize this->rangeCols_
        rangeCols_.resize(J);
      }
      catch (runtime_error & error)   // if an error occur
      {
        // set default capacity (0)
        setCapacityHo();
        // set default range
        this->setCols();
        // clear this->capacityCols_
        this->capacityCols_.clear();
        // clear this->rangeCols_
        this->rangeCols_.clear();
        // throw the error
        throw error;
      }
      // set new capacity if no error occur
      setCapacityHo(size);
    }
    /** New beginning index for the columns of the object.
     *  @param cbeg the index of the first column to set
     **/
    void shiftbeginCols(int cbeg)
    {
      // if there is something to do
      if ((cbeg - this->beginCols()) != 0)
      {
        // is this structure just a pointer?
        if (this->isRef())
        { STKRUNTIME_ERROR_1ARG(IArray2DBase::shiftbeginCols,cbeg,cannot operate on references);}
        // translate data
        allocator_.shiftData(cbeg);
        // tranlate capacityCols_
        capacityCols_.shift(cbeg);
        // translate rangeCols_
        rangeCols_.shift(cbeg);
        // adjust dimensions
        Base2D::shiftbeginCols(cbeg);
      }
    }
    /** Swapping the pos1 column and the pos2 column.
     *  @param pos1 position of the first col
     *  @param pos2 position of the second col
     **/
    void swapCols(int pos1, int pos2)
    {
      if (this->beginCols() > pos1)
      { STKOUT_OF_RANGE_2ARG(IArray2D::swapCols,pos1, pos2,beginCols() >pos1);}
      if (this->endCols() <= pos1)
      { STKOUT_OF_RANGE_2ARG(IArray2D::swapCols,pos1, pos2,endCols() <= pos1);}
      if (this->beginCols() > pos2)
      { STKOUT_OF_RANGE_2ARG(IArray2D::swapCols,pos1, pos2,beginCols() >pos2);}
      if (this->endCols() <= pos2)
      { STKOUT_OF_RANGE_2ARG(IArray2D::swapCols,pos1, pos2,endCols() <=pos2);}
      // swap allocator part
      allocator_.swap(pos1, pos2);
      // swap capacityCols_
      std::swap(capacityCols_[pos1], capacityCols_[pos2]);
      // swap rangeCols_
      std::swap(rangeCols_[pos1], rangeCols_[pos2]);
    }
    /** exchange this container with T.
     *  @param T the container to exchange with this
     **/
    void exchange(IArray2DBase &T)
    {
      // swap AllocatorBase part
      allocator_.exchange(T.allocator_);
      // swap IContainer2D part
      Base2D::exchange(T);
      // swap this part
      std::swap(this->capacityHo_, T.capacityHo_);
      capacityCols_.exchange(T.capacityCols_);
      rangeCols_.exchange(T.rangeCols_);
    }
    /** swap two elements: only for vectors an points. */
    inline void swap(int i, int  j) { std::swap(this->elt(i), this->elt(j)); }
    /** Append the container @c other to @c this without copying the data
     *  explicitly. The column of @c other are appended to this and
     *  @c other will become a reference container. Observe that the @c const
     *  keyword is not respected in this method: but it is useful to
     *  define this method even for constant objects. The data in itself are not
     *  altered, the Array2D become a reference on its own data.
     *  @param other the container to merge with this
     **/
    template<class Other>
    void merge(IArray2DBase<PTRCOL, Other> const& other)
    {
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_NO_ARG(IArray2DBase::merge(other),*this is a reference.);}
      // is other just a pointer?
      if (other.isRef())
      { STKRUNTIME_ERROR_NO_ARG(IArray2DBase::merge(other),other is a reference.);}
      // if there is no columns, we can safely modify the vertical range
      if (this->sizeCols() <= 0) this->setRows(other.rows());
      // Are ranges corrects ?
      if (this->rows() != other.rows())
      { STKRUNTIME_ERROR_NO_ARG(IArray2DBase::merge(other),this->rows() != other.rows());}
      // break const reference
      IArray2DBase<PTRCOL, Other>& Tref = const_cast<IArray2DBase<PTRCOL, Other>&>(other);
      // compute horizontal range of the container after insertion
      Range cols(this->cols());
      // compute first index of the first column added
      const int first = cols.lastIdx() + 1;
      // reallocate memory for the columns
      cols.incLast(Tref.sizeCols());
      this->reallocCols(cols);
      this->setCols(cols);
      // align other range
      Tref.shiftbeginCols(first); // easiest like that
      // copy data from other
      for (int j=first; j< cols.end(); j++) { copyColumn(Tref, j, j);}
      // delete and set view on the data
      Tref.allocator().free();
      Tref.allocator().setPtrData(allocator_.p_data(), allocator_.rangeData(), true);
    }
    /** Append the vector @c other to @c this without copying the data
     *  explicitly. @c other is appended to this and
     *  @c other will become a reference container. The data in itself are not
     *  altered, the Array1D become a reference on its own data.
     *  @param other the container to merge with this
     **/
    template<class Other>
    void merge(IArray1D<Other> const& other)
    {
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_NO_ARG(IArray2DBase::merge(IArray1D),*this is a reference.);}
      // is other just a pointer?
      if (other.isRef())
      { STKRUNTIME_ERROR_NO_ARG(IArray2DBase::merge(IArray1D),other is a reference.);}
      // if there is no columns, we can safely modify the vertical range
      if (this->sizeCols() <= 0) this->setRows(other.range());
      // Are ranges corrects ?
      if (this->rows() != other.range())
      { STKRUNTIME_ERROR_NO_ARG(IArray2DBase::merge(IArray1D),this->rows() != other.range());}
      // compute horizontal range of the container after insertion
      Range cols(this->cols());
      // reallocate memory for the columns
      cols.incLast(1);
      this->reallocCols(cols);
      this->setCols(cols);
      // set column
      data(cols.lastIdx()) = other.p_data();
      capacityCols_[cols.lastIdx()] = other.sizeData();
      rangeCols_[cols.lastIdx()] = other.range();
      // set other as reference
      other.setRef(true);
    }
  protected:
    /** allocator of the column data set */
    Allocator allocator_;
    /** capacity of the columns of the container (for each column: number of
     *  available Rows without reallocation in this column)
    **/
    Array1D<int> capacityCols_;
    /** range of the index of the columns of the container. **/
    Array1D<Range> rangeCols_;
    /** @return a pointer on the j-th column of the container
     *  @param j the index of the column
     **/
    inline PTRCOL& data(int j) { return allocator_.data(j);}
    /** set the maximum possible number of columns without reallocation.
     *  @param capacity the maximum number of columns
     **/
    inline void setCapacityHo(int capacity = 0) { capacityHo_ = capacity;}
    /** move T to this
     *  @param T the container to move
     **/
     inline void move(Derived const& T)
     {
       allocator_.move(T.allocator_);
       // move this part
       capacityCols_.move(T.capacityCols_);
       rangeCols_.move(T.rangeCols_);
       capacityHo_ = T.capacityHo_;
       // Set IContainer2D part
       this->setCols(T.cols());
       this->setRows(T.rows());
     }
    /** copy the column pos2 of the container T to the column
     *  pos1 of this. One of the container (either this or T but not both)
     *  have to be a reference otherwise, user will experiment a memory leak.
     *
     *  @param T the container with the column to transfer
     *  @param pos1 index of the column to initialize
     *  @param pos2 the column in the container T to transfer in this
     **/
    template<class Other>
    void copyColumn( IArray2DBase<PTRCOL, Other> const& T, int pos1, int pos2)
    {
      // copy column pos2 of T in pos1 of this
      data(pos1) = T.data(pos2);
      // set capacityCols_
      capacityCols_[pos1] = T.capacityCols()[pos2];
      // set rangeCols_
      rangeCols_[pos1] = T.rangeCols()[pos2];
    }

    /** Transfer the column pos2 of the container T to the column
     *  pos1 of this. Set the column pos2 in T to a default value.
     *  The column pos1 should not exists or should be deleted previously
     *  otherwise user will experiment a memory leak.
     *
     *  @param T the container with the column to transfer
     *  @param pos1 index of the column to initialize
     *  @param pos2 the column in the container T to transfer in this
     **/
    void transferColumn( IArray2DBase& T, int pos1, int pos2)
    {
      // copy column pos2 of T in pos1 of this
      data(pos1) = T.data(pos2);
      // set capacityCols_
      capacityCols_[pos1] = T.capacityCols_[pos2];
      // set rangeCols_
      rangeCols_[pos1] = T.rangeCols_[pos2];
      // set column of T to default
      T.setDefaultCol(pos2);
    }
    /** Method for memory allocation and initialization of the horizontal
     *  range of the container.
     *  The vertical range is not set in this method. If an
     *  error occur, we set the cols_ of the container to default.
     *  @param J horizontal range
     **/
    void mallocHo(Range const& J)
    {
      // compute the size necessary (can be 0)
      int size = Arrays::evalSizeCapacity(J.size());
      // try to allocate memory
      try
      {
        // initialize this->capacityCols_
        capacityCols_.resize(J);
        // initialize this->rangeCols_
        rangeCols_.resize(J);
        // allocate memory for the columns
        allocator_.malloc(Range(J.begin(), size));
      }
      catch (runtime_error & error)   // if an error occur
      {
        // set default capacity (0)
        setCapacityHo();
        // set default range
        this->setCols();
        // clear this->capacityCols_
        capacityCols_.clear();
        // clear this->rangeCols_
        rangeCols_.clear();
        // throw the error
        throw error;
      }
      // set new capacity if no error occur
      setCapacityHo(size);
    }
    /** Method for memory reallocation and initialization of the horizontal
     *  range of the container.
     *  The vertical range is not set in this method. If an
     *  error occur, we set the cols_ of the container to default.
     *  @param J horizontal range
     **/
    void reallocCols(Range const& J)
    {
      // compute the size necessary (can be 0)
      int size = Arrays::evalSizeCapacity(J.size());
      // try to allocate memory
      try
      {
        // allocate memory for the columns
        allocator_.realloc(Range(J.begin(), size));
        // initialize this->capacityCols_
        capacityCols_.resize(J);
        // initialize this->rangeCols_
        rangeCols_.resize(J);
      }
      catch (runtime_error & error)   // if an error occur
      {
        // set default capacity (0)
        setCapacityHo();
        // set default range
        this->setCols();
        // clear this->capacityCols_
        this->capacityCols_.clear();
        // clear this->rangeCols_
        this->rangeCols_.clear();
        // throw the error
        throw error;
      }
      // set new capacity if no error occur
      setCapacityHo(size);
    }

    /** Horizontal Memory deallocation.
     *  This method clear all allocated memory. The range of the columns
     *  is set to (firstCol_:firstCol_-1). The range of the Rows remain
     *  unmodified. If there is allocated memory for the columns, it
     *  should be liberated prior to this method.
     **/
    void freeRows()
    {
      // Nothing to do for reference
      if (this->isRef()) return;
      // free memory allocated in AllocatorBase
      allocator_.free();
      // set capacity size to default
      this->setCapacityHo();
      // set range of the columns to default
      this->setCols(Range(this->beginCols(), 0));
      // set capacityCols_ to default
      capacityCols_.clear();
      // set rangeCols_ to default
      rangeCols_.clear();
    }
  private:
    /** Horizontal capacity of the container (number of available
     *  columns without reallocation)
     **/
    int capacityHo_;
    /** set the default parameters and dimension to a column of the container.
     *  @param col the position of the column to initialize to a default value.
     **/
    inline void setDefaultCol(int col)
    {
      // set column of T to default
      data(col) = 0;
      // set capacityCols_
      this->capacityCols_[col] = 0;
      // set rangeCols_
      this->rangeCols_[col] = Range();
    }
};

} // namespace STK

#endif
// STK_ITARRAY2DBASE_H
