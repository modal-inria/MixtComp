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

/** @file STK_IArray2D.h
 *  @brief Interface base class for the Array1D, this is an internal header file,
 *  included by other Containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class like Array2D, except if you want to create your own
 *  Container Class.
 **/

#ifndef STK_IARRAY2D_H
#define STK_IARRAY2D_H

#include "STK_IArray2DBase.h"

namespace STK
{

/** @ingroup Arrays
  * @brief Interface base class for two-dimensional arrays.
  *
  * A IArray2D is a specialized interface class for two-dimensional
  * containers stored in columns. All derived class from @c IArray2D
  * access to the column using a @c Type* ptr.
  *
  * All memory is allocated there using the @c rangeRowsInCol method.
  *
  * @tparam Derived is the name of the class implementing an @c IArray2D.
  * @sa Array2D, @sa Array2DDiagonal, @sa Array2DLowerTriangular,
  * @sa Array2DUpperTriangular, @sa Array2DPoint, @sa Array2DVector;
  * @sa Array2DSquare.
 **/
template < class  Derived  >
class IArray2D : public IArray2DBase< typename hidden::Traits<Derived>::Type*, Derived>
{
  public:
    /** type of the Base Container Class. */
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef IArray2DBase< Type*, Derived> Base;

    typedef typename hidden::Traits<Derived>::Row Row;
    typedef typename hidden::Traits<Derived>::Col Col;
    typedef typename hidden::Traits<Derived>::SubRow SubRow;
    typedef typename hidden::Traits<Derived>::SubCol SubCol;
    typedef typename hidden::Traits<Derived>::SubArray SubArray;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;

  protected:
    /** Default constructor */
    IArray2D() : Base() {}
    /** Constructor with specified ranges
     *  @param I range of the Rows
     *  @param J range of the Cols
     **/
    IArray2D( Range const& I, Range const& J) : Base(I, J)
    { initializeCols(J);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if we wrap T
     **/
    IArray2D( const IArray2D& T, bool ref =false) : Base(T, ref)
    {
      if (!ref)
      {
        // initialize the Cols and Rows
        initializeCols(T.cols());
        for (int j=T.firstIdxCols(); j<=T.lastIdxCols(); j++)
        { copyColumnForward(T, j, j);}
      }
    }
    /** constructor by reference, ref_=1.
     *  @param T the container to copy
     *  @param I range of the Rows to wrap
     *  @param J range of the Col to wrap
     **/
    template<class OtherArray>
    IArray2D( IArray2D<OtherArray> const& T, Range const& I, Range const& J)
            : Base(T, I, J)
    {}
    /** Wrapper constructor the Container is a ref.
     *  @param q pointer on data
     *  @param I range of the Rows to wrap
     *  @param J range of the Cols to wrap
     **/
    IArray2D( Type** q, Range const& I, Range const& J) : Base(q, I, J) {}
    /** destructor.
     *  free the vertically allocated memory (the columns). The horizontally
     *  allocated memory is handled by the Allocator class.
     **/
    ~IArray2D() { if (!this->isRef()) this->freeCols(this->cols());}

  public:
    /** access to one element.
     *  @param i index of the row
     *  @param j index of the col
     *  @return a reference on the (i,j) element
     **/
    inline Type& elt2Impl( int i, int j)
    { return this->data(j)[i];}
    /** access to one element const.
     *  @param i index of the row
     *  @param j index of the col
     *  @return a constant reference on the (i,j) element
     **/
    inline Type const& elt2Impl( int i, int j) const
    { return this->data(j)[i];}
    /** @return  many elements.
     *  @param J Range of the elements
     **/
    inline SubVector sub(Range const& J) const
    { return SubVector(this->asDerived(), J);}
    /** access to a part of a column.
     *  @param I range of the rows
     *  @param j index of the col
     *  @return a reference in the range I of the column j of this
     **/
    inline SubCol col(Range const& I, int j) const
    { return SubCol( this->asDerived(), Range::inf(I, this->asDerived().rangeRowsInCol(j)), j);}
    /** access to a part of a column.
     *  @param j index of the column
     *  @return a reference in the range I of the column j of this
     **/
    inline Col col( int j) const
    { return Col( this->asDerived(), this->asDerived().rangeRowsInCol(j), j);}
    /** access to a part of a row.
     *  @param i index of the row
     *  @param J range of the columns
     *  @return a reference of the row i.
     **/
    inline SubRow row(int i, Range const& J) const
    { return SubRow( this->asDerived(), Range::inf(J, this->asDerived().rangeColsInRow(i)), i);}
    /** access to a part of a row.
     *  @param i index of the row
     *  @return a reference of the row i.
     **/
    inline Row row( int i) const
    { return Row( this->asDerived(), this->asDerived().rangeColsInRow(i), i);}
    /** access to many columns.
     *  @param J range of the index of the cols
     *  @return a 2D container containing the Container in the Horizontal range @c J
     **/
    inline SubArray col(Range const& J) const { return sub(this->rows(), J);}
    /** access to many rows.
     *  @param I range of the index of the rows
     *  @return a 2D container containing the Container in the vertical range @c I
     **/
    inline SubArray row(Range const& I) const { return sub(I, this->cols());}
    /** access to a sub-array.
     *  @param I range of the rows
     *  @param J range of the columns
     **/
    inline SubArray sub(Range const& I, Range const& J) const
    { return SubArray(this->asDerived(), I, J);}

    /** set a value to the whole container */
    void setValue(Type const& v)
    {
      for (int j=this->firstIdxCols(); j<=this->lastIdxCols(); j++)
      {
        Type* p(this->data(j));
        const int beg(this->rangeCols_[j].firstIdx()), end(this->rangeCols_[j].lastIdx());
        for (int i=beg; i<=end; i++) p[i]= v;
      }
    }
    /** move T to this.
     *  @note : T is not modified but just set as a reference of the data it was responsible.
     *  @param T the container to move.
     **/
     void move(Derived const& T)
     {
       if (this->asPtrDerived() == &T) return;
       if (!this->isRef()) { freeCols(this->cols());}
       // move Base part
       Base::move(T);
     }
    /** clear the object.
     *  This will free all allocated memory and reset all range to Range().
     **/
    void clear()
    {
      // Nothing to do for reference
      if (this->isRef()) return;
      // free the Rows memory
      this->freeMem();
      this->setRanges();
      // initialize if necessary
      this->mallocHo(this->cols());
      initializeCols(this->cols());
    }
    /** New first indexes for the object.
     *  @param rbeg the index of the first row to set
     *  @param cbeg the index of the first column to set
     **/
    void shift( int rbeg, int cbeg)
    {
      // move begin of the columns
      this->shiftFirstIdxCols(cbeg);
      // move begin of the rows
      this->shiftFirstIdxRows(rbeg);
    }
    /** resize the container.
     *
     *  @note The implicit assumption made by this method is that it is easiest
     *  and faster to add column than add rows to the 2D container.
     *
     * @param I the new range for the rows of the container
     * @param J the new range for the columns of the container
     **/
    Derived& resize( Range const& I, Range const& J)
    {
      // check if there is something to do
      if ((this->rows() == I) && (this->cols() == J)) return this->asDerived();
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray2D::resize,I,J,cannot operate on reference);}
      //  translate beg
      this->shift(I.firstIdx(), J.firstIdx());
      // check again if there is something to do
      if ((this->rows() == I) && (this->cols() == J)) return this->asDerived();
      // number of rows and columns to delete or add
      int rinc = I.lastIdx() - this->lastIdxRows();
      int cinc = J.lastIdx() - this->lastIdxCols();

      // check if we add columns
      if ((cinc >=0)) // work first on rows as we add columns
      {
        if (rinc < 0)
        { this->popBackRows(-rinc);}  // less rows
        else
        { this->pushBackRows(rinc);} // more rows
        this->pushBackCols(cinc); // add columns
        return this->asDerived();
      }
      // work first on columns as we remove column
      this->popBackCols(-cinc); // remove columns
      if (rinc < 0) this->popBackRows(-rinc); // less rows
      else          this->pushBackRows(rinc); // more rows
      return this->asDerived();
    }
    /** New first index for the object.
     *  @param beg the index of the first element to set
     **/
    void shift( int beg)
    { this->asDerived().shift1D(beg);}
    /** @return the resized row or column vector
     *  @param I the new range for the vector/point
     **/
    Derived& resize( Range const& I)
    {
      return this->asDerived().resize1D(I);
      return this->asDerived();
    }
    /** New first index for the Rfws of the array.
     *  @param rbeg the index of the first row to set
     **/
    void shiftFirstIdxRows( int rbeg)
    {
       // compute increment
     int rinc = rbeg - this->firstIdxRows();
      // if there is something to do
      if (rinc == 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray2D::shiftFirstIdxRows,rbeg,cannot operate on reference);}
      // translate rows_()
      Base::shiftFirstIdxRows(rbeg);
      // For all cols, move begin
      for (int j=this->firstIdxCols(); j<=this->lastIdxCols(); j++)
      { shiftCol(j, this->rangeCols_[j].firstIdx()+rinc);}
    }
    /** Add n Rows to the container.
     *  @param n number of Rows to add
     **/
    void pushBackRows( int n=1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray2D::pushBackRows,n,cannot operate on reference);}
      // If the container have no rows : create its
      if (this->sizeRows() <=0)
      {
        // update the range of the container
        this->incLastIdxRows(n);
        // initialize the container
        this->initializeCols(this->cols());
      }
      else
      {
        // update the range of the rows
        this->incLastIdxRows(n);
        // allocate new Rows for each Col
        for (int j=this->firstIdxCols(); j<=this->lastIdxCols(); j++)
        {
          // compute range from the leaf
          Range range(this->asDerived().rangeRowsInCol(j));
          // if there is no column or the end is less than the container
          // end
          if ((range.size()>0)&&(range.lastIdx()>this->lastIdxRows()-n))
          {
            // if the column is empty create it
            if (this->rangeCols_[j].size()<=0)
            {
              this->initializeCol(j, range);
            }
            else
            {
              // compute position
             int pos(this->lastIdxRows()-n+1);
              // add elts
              insertRowsToCol(j, pos, range.lastIdx() - pos +1);
            }
          }
        }
      }
    }
    /** Insert n Rows at the position pos of the container.
     *  If pos is outside the range of a column, then the
     *  method do nothing.
     *  @param pos index where to insert Rows
     *  @param n number of elements to insert (default 1)
     **/
    void insertRows(int pos, int n =1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray2D::insertRows,pos,n,cannot operate on reference);}
      if (this->firstIdxRows() > pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::insertRows,pos,n,firstIdxRows() > pos);}
      if (this->lastIdxRows()+1 < pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::insertRows,pos,n,lastIdxRows()+1 < pos);}
      // update the range of the rows
      this->incLastIdxRows(n);
      // allocate new Rows for each Col
      for (int j=this->firstIdxCols(); j<=this->lastIdxCols(); j++)
      {
        // check position
        if ( (pos >= this->rangeCols_[j].firstIdx())
           ||(pos <= this->rangeCols_[j].lastIdx()+1)
           )
        { insertRowsToCol(j, pos, n);}
      }
    }
    /** Delete last n latest rows of the container.
     *  @param n number of rows to delete
     **/
    void popBackRows( int n = 1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray2D::popBackRows,n,cannot operate on reference);}
      if (this->sizeRows() < n)
      { STKOUT_OF_RANGE_1ARG(IArray2D::popBackRows,n,sizeRows() < n);}
      this->decLastIdxRows(n);
      // decrease range of each Col
      for (int j=this->firstIdxCols(); j<=this->lastIdxCols(); j++)
        eraseRowsToCol(j, this->lastIdxRows()+1, n);
    }

    /** Delete n Rows at the pos index to the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
    **/
    void eraseRows(int pos, int n=1)
    {
      // if n==0 nothing to do
      if (n<=0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray2D::eraseRows,pos,n,cannot operate on reference);}
      if (this->firstIdxRows() > pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::eraseRows,pos,n,firstIdxRows() > pos);}
      if (this->lastIdxRows() < pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::eraseRows,pos,n,lastIdxRows() < pos);}
      if (this->lastIdxRows() < pos+n-1)
      { STKOUT_OF_RANGE_2ARG(IArray2D::eraseRows,pos,n,lastIdxRows() < pos+n-1);}
      // update each Col
      for (int j=this->firstIdxCols(); j<=this->lastIdxCols(); j++)
        eraseRowsToCol(j, pos, n);
      // update dimensions
      this->decLastIdxRows(n);
    }
    /** Add n Cols to the container.
     *  @param n the number of Cols to add
     **/
    void pushBackCols(int n = 1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray2D::pushBackCols,n,cannot operate on reference);}
      // If the container have no Cols : create its
      if (this->sizeCols() <=0)
      {
        this->incLastIdxCols(n);
        this->mallocHo( this->cols());
        initializeCols( this->cols());
      }
      else // else insert to the end of the container
      { insertCols(this->lastIdxCols()+1, n);}
    }
    /** Insert n Columns at the index pos to the container.
     *  @param pos the position of the inserted Cols
     *  @param n the number of column to insert
     **/
    void insertCols(int pos, int n =1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray2D::insertCols,pos,n,cannot operate on reference);}
      if (this->firstIdxCols() > pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::insertCols,pos,n,firstIdxCols() > pos);}
      if (this->lastIdxCols()+1 < pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::insertCols,pos,n,lastIdxCols()+1 < pos);}
      // compute horizontal range of the container after insertion
      Range range_ho(this->cols());
      range_ho.incLast(n);
      // allocate, if necessary, the mem for the Cols
      if (this->capacityHo() < range_ho.size()) //  not enough space
      {
        // exchange with Taux
        Derived Taux;
        this->exchange(Taux);
        // initialize columns of the container
        try
        {
          this->mallocHo(range_ho);
        }
        catch (Exception & error)   // if an error occur
        {
          this->exchange(Taux);   // restore container
          throw error;        // and send again the exception
        }
        // set the ranges
        this->setCols(range_ho);
        this->setRows(Taux.rows());
        // move first Columns from Taux to this
        for (int k=Taux.firstIdxCols(); k<pos; k++)
          this->transferColumn(Taux, k, k);
        // translate and copy last Cols from Taux to this
        for (int k=Taux.lastIdxCols(); k>=pos; k--)
          this->transferColumn(Taux, k+n, k);
      }
      else // enough space -> shift the last Cols
      {
        Range addRange(this->lastIdxCols()+1, n);
        // insert default capacity for the new Columns
        this->capacityCols_.insert(addRange, 0);
        // insert default range for the new Columns
        this->rangeCols_.insert(addRange, Range());
        // update range_
        this->incLastIdxCols(n);
        // translate data
        for (int k=this->lastIdxCols()-n; k>=pos; k--)
          this->transferColumn( this->asDerived(), k+n, k);
      }
      // initialize the rows for the Cols, this->capacityCols_, this->rangeCols_
      // in the range pos:pos+n-1
      this->initializeCols(Range(pos, n));
    }
    /** Delete last Cols of the container
     *  @param n the number of Cols to delete
     **/
    void popBackCols( int n =1)
    {
      // if n<=0 nothing to do
      if (n<=0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray2D::popBackCols,n,cannot operate on reference);}
      if (this->sizeCols() < n)
      { STKOUT_OF_RANGE_1ARG(IArray2D::popBackCols,n,sizeCol() < n);}
      // delete each col
      this->freeCols(Range(this->lastIdxCols()-n+1, this->lastIdxCols(), 0));
      // update this->capacityCols_
      this->capacityCols_.popBack(n);
      // update this->rangeCols_
      this->rangeCols_.popBack(n);
      // update cols
      this->decLastIdxCols(n);
      // if there is no more Cols
      if (this->sizeCols() == 0) this->freeMem();
    }
    /** Delete n Cols at the specified position of the container.
     *  @param pos the position of the deleted Cols
     *  @param n the number of column to delete
     **/
    void eraseCols(int pos, int n = 1)
    {
      if (n<=0) return; // if n<=0 nothing to do
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray2D::eraseCols,pos,n,cannot operate on reference);}
      if (this->firstIdxCols() > pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::eraseCols,pos,n,firstIdxCols() > pos);}
      if (this->lastIdxCols() < pos)
      { STKOUT_OF_RANGE_2ARG(IArray2D::eraseCols,pos,n,lastIdxCols() < pos);}
      if (this->lastIdxCols() < pos+n-1)
      { STKOUT_OF_RANGE_2ARG(IArray2D::eraseCols,pos,n,lastIdxCols() < pos+n-1);}
      // delete each col
      this->freeCols(Range(pos, n));
      // update cols_
      this->decLastIdxCols(n);
      // shift Cols
      for (int k=pos; k<=this->lastIdxCols(); k++)
        this->data(k) = this->data(k+n);
      // update this->capacityCols_
      this->capacityCols_.erase(pos, n);
      // update this->rangeCols_
      this->rangeCols_.erase(pos, n);
      // if there is no more Cols
      if (this->sizeCols() == 0) this->freeMem();
    }
    /** Update the columns of the container in the specified range.
     *  @param J range of the column to udpate
     **/
    void update(Range const& J)
    {
      if (this->firstIdxCols() > J.firstIdx())
      { STKOUT_OF_RANGE_1ARG(IArray2D::update,J,firstIdxCols() > J.firstIdx());}
      if (this->lastIdxCols() < J.lastIdx())
      { STKOUT_OF_RANGE_1ARG(IArray2D::update,J,lastIdxCols() < J.last);}
      for ( int icol = J.firstIdx(); icol <= J.lastIdx() ; ++icol)
      { update(icol);}
    }

    /** Update the column of the container in the specified position.
     *  @param col index of the column to update
     **/
    void update(int col)
    {
      if (this->firstIdxCols() > col)
      { STKOUT_OF_RANGE_1ARG(IArray2D::update,col,firstIdxCols() > col);}
      if (this->lastIdxCols() < col)
      { STKOUT_OF_RANGE_1ARG(IArray2D::update,col,lastIdxCols() < col);}
      if (this->asDerived().rangeRowsInCol(col) != this->rangeCol(col))
      { resizeCol(col, this->asDerived().rangeRowsInCol(col));}
    }

    /** overwrite @c this with @c src.
     *  @note If the size match, @c this is not resized, and in this case,
     *  the method take care of the possibly of overlapping.
     *  @param src the container to copy
     **/
    Derived& copy( IArray2D const& src)
    {
      // Resize if necessary.
      if ( (this->sizeRows() != src.sizeRows()) ||(this->sizeCols() != src.sizeCols()) )
        this->resize(src.rows(), src.cols());
      // Copy without overlapping
      if (src.firstIdxRows()>=this->firstIdxRows())
      {
        if (src.firstIdxCols()>this->firstIdxCols())
        {
          for ( int jSrc=src.firstIdxCols(), jDst=this->firstIdxCols(); jSrc<=src.lastIdxCols(); jDst++, jSrc++)
          { this->copyColumnForward(src, jDst, jSrc);}
          return this->asDerived();
        }
        for ( int jSrc=src.lastIdxCols(), jDst=this->lastIdxCols(); jSrc>=src.firstIdxCols(); jDst--, jSrc--)
        { this->copyColumnForward(src, jDst, jSrc);}
        return this->asDerived();
      }
      // src.firstIdxRows()<this->firstIdxRows()
      if (src.firstIdxCols()>=this->firstIdxCols())
      {
        for ( int jSrc=src.firstIdxCols(), jDst=this->firstIdxCols(); jSrc<=src.lastIdxCols(); jDst++, jSrc++)
        { this->copyColumnBackward(src, jDst, jSrc);}
        return this->asDerived();
      }
      // src.firstIdxCols()<this->firstIdxCols()
      for ( int jSrc=src.lastIdxCols(), jDst=this->lastIdxCols(); jSrc>=src.firstIdxCols(); jDst--, jSrc--)
      { this->copyColumnBackward(src, jDst, jSrc);}

      return this->asDerived();
    }
    /** merge (by value) the container other with this.
     *  @param other the container to merge to this
     **/
    template<class Other>
    void pushBackCols(ExprBase<Other> const& other)
    {
      // check if the container is empty
      if (this->empty())
      {
        this->asDerived() = other.asDerived();
        return;
      }
      // this is not empty
      if (other.rows() != this->rows())
      { STKRUNTIME_ERROR_NO_ARG(TContainer2D::pushBackCols,range of the rows are different);}
      // if the container is not empty we add the column and copy other inside
     int size = other.cols().size(), first = this->lastIdxCols()+1;
      pushBackCols(size);
      for (int j0= first, j1= other.firstIdxCols(); j1 <= other.lastIdxCols(); ++j0, ++j1)
      {
        for (int i=other.firstIdxRows(); i<=other.lastIdxRows(); i++)
          (*this)(i, j0) = other(i,j1);
      }
    }
    /** Specialization for Array1D. merge (by value) the container other with this
     *  @param other the column to add to this
     **/
    template<class Other>
    void pushBackCols(ITContainer1D<Other> const& other)
    {
      // check if the container is empty
      if (this->empty())
      {
        resize(other.rows(),1);
        for (int i=other.firstIdx(); i<=other.lastIdx(); i++)
          (*this)(i, baseIdx) = other[i];
        return;
      }
      // not empty
      if (other.rows() != this->rows())
      { STKRUNTIME_ERROR_NO_ARG(TContainer2D::pushBackCols(other),other.rows() != rows());}
      // if the container is not empty we add the column and copy other inside
      int size = other.cols().size(), first = this->lastIdxCols()+1;
      pushBackCols(size);
      for (int i=other.firstIdx(); i<=other.lastIdx(); i++)
        (*this)(i, first) = other[i];
    }
    /** set other at the end of this (concatenate). Perform a copy of the
     *  values stored in other to this.
     *  @param other the container to add back
     *  @note the size and the type have to match
     **/
    template<class Other>
    void pushBackRows(ExprBase<Other> const& other)
    {
      // check if the container is empty
      if (this->empty())
      {
        this->asDerived() = other.asDerived();
        return;
      }
      // not empty
      if (other.cols() != this->cols())
        STKRUNTIME_ERROR_NO_ARG(Iarrya2D::pushBackRows,range of the columns are different);
      // add nbRow to existing rows
      int nbRow = other.sizeRows();
      pushBackRows(nbRow);
      for (int j=this->firstIdxCols(); j<= this->lastIdxCols(); ++j)
      {
        // start from the end in order to avoid
        for (int i=this->lastIdxRows(), iOther= other.lastIdxRows(); iOther>=other.firstIdxRows(); --i, --iOther)
        { this->elt(i,j) = other.elt(iOther,j);}
      }
    }

  protected:
    /** copy forward the column @c srcCol of @c src in the column @c dstCol of this. */
    void copyColumnForward(IArray2D const& src, int jDst, int jSrc)
    {
      Type *dp =this->data(jDst), *sp =src.data(jSrc);
      const int tfirst(this->rangeCols_[jDst].firstIdx())
              , sfirst(src.rangeCols_[jSrc].firstIdx())
              , slast (src.rangeCols_[jSrc].lastIdx());
      for ( int it=tfirst, is=sfirst; is<=slast; it++, is++) dp[it] = sp[is];
    }
    /** copy backward the column @c jSrc of @c src in the column @c jDst of this. */
    void copyColumnBackward(IArray2D const& src, int jDst, int jSrc)
    {
      Type *dp =this->data(jDst), *sp =src.data(jSrc);
      const int tlast (this->rangeCols_[jDst].lastIdx())
                  , sfirst(src.rangeCols_[jSrc].firstIdx())
                  , slast (src.rangeCols_[jSrc].lastIdx());

      for ( int it=tlast, is=slast; is>=sfirst; it--, is--) dp[it] = sp[is];
    }

  private:
    /** Function for memory allocation and initialization.
     *  The capacity for the Rows have to be set before calling this method.
     *  @param J vertical range of the Cols to initialize
     **/
    void initializeCols(Range const& J)
    {
      // set the ranges
      if (this->firstIdxCols() > J.firstIdx())
      { STKOUT_OF_RANGE_1ARG(IArray2D::initializeCols,J,firstIdxCols() > J.firstIdx());}
      if (this->lastIdxCols() < J.lastIdx())
      { STKOUT_OF_RANGE_1ARG(IArray2D::initializeCols,J,lastIdxCols() < J.last);}
      for (int j=J.firstIdx(); j<=J.lastIdx(); j++)
      {
        try
        {
          // initialize the column with the range specific to the container
          this->initializeCol(j, this->rangeRowsInCol(j));
        }
        catch (Exception const& error)   // if an error occur
        {
          // free each column allocated
          for (int k=J.firstIdx(); k<j; k++) this->freeCol(k);
          // put default for the other Cols
          for (int k=j; k<=J.lastIdx(); k++) this->data(k) = 0;
          // and throw an exception
          throw error;
        }
      }
    }
    /** @brief internal method for initializing a column.
     *
     *  Method for the the allocation of memory of the col
     *  pos with the given range.
     *  @param pos the index of the column to initialize
     *  @param I   range of the Col
     **/
    void initializeCol(int pos, Range const& I)
    {
      if (I.size() <=0)
      {
        // set default for ptr
        this->data(pos) = 0;
        // set default value for this->capacityCols_[pos]
        this->capacityCols_[pos] = 0;
        // set default value for this->rangeCols_[pos]
        this->rangeCols_[pos] = I;
        // return
        return;
      }
      // compute the size necessary (cannot be 0)
     int size = Arrays::evalSizeCapacity(I.size());
      // try to allocate memory
      try
      {
        this->data(pos) = new Type[size];
      }
      catch (std::bad_alloc & error)  // if an alloc error occur
      {
        // set default for ptr
        this->data(pos) = 0;
        // set default value for this->capacityCols_[pos]
        this->capacityCols_[pos] = 0;
        // set default value for this->rangeCols_[pos]
        this->rangeCols_[pos] = Range();
        // and throw an exception
        STKRUNTIME_ERROR_2ARG(IArray2D::initializeCol,pos, I,memory allocation failed.);
      }
      // increment ptr of the column
      this->data(pos) -= I.firstIdx();
      // set size for this->capacityCols_[pos]
      this->capacityCols_[pos] = size;
      // set value for this->rangeCols_[pos]
      this->rangeCols_[pos] = I;
    }
    /** Memory deallocation.
     *  This method clear all allocated memory. The range of the Cols
     *  is set to (beginHo_:beginHo_-1). The range of the Rows remain
     *  unmodified.
     **/
    void freeMem()
    {
      // Nothing to do for reference
      if (this->isRef()) return;
      // free the Rows memory
      this->freeCols(this->cols());
      // liberate horizontally
      this->freeRows();
    }
    /** vertical memory deallocation.
     *  @param J the range of the columns to liberate.
     **/
    void freeCols(Range const& J)
    {
      if (this->firstIdxCols() > J.firstIdx())
      { STKOUT_OF_RANGE_1ARG(IArray2D::freeCols,J,firstIdxCols() > J.firstIdx());}
      if (this->lastIdxCols() < J.lastIdx())
      { STKOUT_OF_RANGE_1ARG(IArray2D::freeCols,J,lastIdxCols() < J.last);}
      for (int j=J.firstIdx(); j<=J.lastIdx(); j++) { this->freeCol(j);}
    }
    /** @brief Internal method for memory deallocation.
     *  @param col the number of the column to free
     **/
    void freeCol(int col)
    {
      if (this->data(col)) // if there is a column at this position
      {
        // increment the ptr
        this->data(col) += this->rangeCols_[col].firstIdx();
        // delete allocated mem for the column col
        delete [] this->data(col);
        // set default value for ptr
        this->data(col) =0;
        // set default value for this->capacityCols_[col]
        this->capacityCols_[col] = 0;
        // set default value for this->rangeCols_[col]
        this->rangeCols_[col] = Range();
      }
    }
    /** @brief internal method for translating a column.
     *
     *  Method for the the allocation of memory of the column
     *  pos with the given range.
     *  @param col the index of the column to translate
     *  @param beg new begin of the column
     **/
    void shiftCol( int col, int beg)
    {
      // check if there is data
      if (this->data(col)) { this->data(col) -= (beg - this->rangeCols_[col].firstIdx());}
      // translate this->rangeCols_
      this->rangeCols_[col].shift(beg);
    }
    /** @brief Internal method for resizing a column with a specified
     *  range.
     *
     *  This method resize the column @c pos to the desired range
     *  using:
     * - @c shiftCol
     * - either @c popBackRowsToCol or @c pushBackRowsToCol if needed.
     *  @param col index of the column
     *  @param I range to set to the column
    **/
    void resizeCol( int col, Range const& I)
    {
      // check if there is something to do
      if (this->rangeCol(col) == I) return;
      // shift to the desired first index
      shiftCol(col, I.firstIdx());
      // compute difference of size
     int inc = this->rangeCol(col).size() - I.size();
      // nothing to do
      if (inc == 0) return;
      // add row
      if (inc < 0) { pushBackRowsToCol(col, -inc);}
      else         { popBackRowsToCol(col, inc);}
    }
    /** @brief Internal method for inserting rows to a specified column.
     *
     *  Insert n Rows at the position pos to the column column of the
     *  container. No check is done about the index.
     *  @param col column index
     *  @param pos index where to insert Rows
     *  @param n number of elements to insert (default 1)
     **/
    void insertRowsToCol( int col, int pos, int n =1)
    {
      // wrap old Col
      Type* p_oldCol(this->data(col));
      // get vertical range of the Col
      Range oldRange(this->rangeCols_[col]);
      // update range
      this->rangeCols_[col].incLast(n);
      // allocate if necessary the Col
      if (this->capacityCols_[col] < this->rangeCols_[col].size())
      {
        // create new Col
        this->initializeCol(col, this->rangeCols_[col]);
        // if there was data, copy and liberate
        if (p_oldCol)
        {
          // get ptr on the new col
          Type* p_newCol(this->data(col));
          // copy first Elts
          for (int k=oldRange.firstIdx(); k<pos; k++) p_newCol[k] = p_oldCol[k];
          // translate and copy last Elts
          for (int k=oldRange.lastIdx(); k>=pos; k--) p_newCol[k+n] = p_oldCol[k];
          // increment ptr_col
          p_oldCol += oldRange.firstIdx();
          // and free old col
          delete [] p_oldCol;
        }
      }
      else // enough space
      {
        // translate last Elts
        for (int k=oldRange.lastIdx(); k>=pos; k--)
          p_oldCol[k+n] = p_oldCol[k];
      }
    }
    /** @brief Internal method for appending rows to a specified column.
     *
     *  Push back n Rows at the end of the column @c col of the container.
     *  @param col column index
     *  @param n number of elements to append (default 1)
     **/
    void pushBackRowsToCol( int col, int n =1)
    {
      // wrap old Col
      Type* p_oldCol(this->data(col));
      // get vertical range of the Col
      Range oldRange(this->rangeCols_[col]);
      // compute vertical range of the Col after insertion
      this->rangeCols_[col].incLast(n);
      // allocate if necessary the Col
      if (this->capacityCols_[col] < this->rangeCols_[col].size())
      {
        // create new Col
        this->initializeCol(col, this->rangeCols_[col]);
        // ger ptr on the new col
        Type* p_newCol(this->data(col));
        // copy Elts
        for (int k=oldRange.firstIdx(); k<=oldRange.lastIdx(); k++) p_newCol[k] = p_oldCol[k];
        // if there is allocated memory, liberate it
        if (p_oldCol)
        {
          p_oldCol += oldRange.firstIdx();
          delete [] p_oldCol;
        }
      }
    }
    /** @brief Internal method for deleting rows from a specified column.
     *
     *  Delete n Rows at the position pos to the column col of the container.
     *  No check is done about indexes. It is possible to remove data
     *  outside the range of the column. In this case it is assumed
     *  that the data are known and there was no necessity to store
     *  them inside the container (think to a triangular matrix).
     *
     *  @param col index of the Col
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
    **/
    void eraseRowsToCol( int col, int pos, int n=1)
    {
      // check trivial cases
      if (this->rangeCols_[col].lastIdx() < pos) return;
      if (this->rangeCols_[col].firstIdx()> pos+n-1)
      { shiftCol( col, this->rangeCols_[col].firstIdx() - n); return;}
      // find the exisiting rows to delete
      Range rangeDel(pos, n);
      rangeDel.inf(this->rangeCols_[col]);
      if (rangeDel == this->rangeCols_[col]) { freeCol(col); return;}
      // shift data, rangeDel is inside the rang of the column
      Type* p_col(this->data(col));
      for ( int k=rangeDel.firstIdx(), k1=rangeDel.lastIdx()+1; k1<=this->rangeCols_[col].lastIdx(); k++, k1++)
      {  p_col[k]   = p_col[k1];}
      // update size of the range
      this->rangeCols_[col].decLast(rangeDel.size());
      // and shift if necessary
      if (pos < rangeDel.firstIdx())
      { shiftCol( col, this->rangeCols_[col].firstIdx() - (n-rangeDel.size()));}
    }
    /** @brief Internal method for deleting last rows to a specified column.
     *
     *  Delete the  n lastest Rows to the container.
     *
     *  @param col index of the Col
     *  @param n number of elements to delete (default is 1)
    **/
    void popBackRowsToCol( int col, int n=1)
    {
      // check if there is something to do
      if (n <= 0) return;
      // update range
      this->rangeCols_[col].decLast(n);
      // free mem if necessary
      if (this->rangeCols_[col].size()==0) freeCol(col);
    }
};

} // namespace STK

#endif
// STK_IARRAY2D_H
