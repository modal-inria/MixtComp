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
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IArray1D.h
 *  @brief Interface base class for the Array1D, this is an internal header file,
 *  included by other Containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class like Array1D, except if you want to create your own
 *  Container Class.
 **/

#ifndef STK_IARRAY1D_H
#define STK_IARRAY1D_H

#include "STK_AllocatorBase.h"
#include "STK_ITContainer1D.h"

namespace STK
{

/** @ingroup Arrays
 *  @brief Templated one dimensional Array.
 * 
 * An IArray1D is a templated one column container implementing the interface
 * base class ITContainer1D.
 **/
template<class Derived >
class IArray1D  : public ITContainer1D<Derived>
                , public AllocatorBase<typename hidden::Traits<Derived>::Type>
{
  protected:
    typedef typename hidden::Traits<Derived>::Type Type;
    /** Type for the Base reference Class of a 1D Array. */
    typedef AllocatorBase<Type> Allocator;
    /** Type for the IArray1D Class. */
    typedef ITContainer1D< Derived > Base;
    /** Default constructor. */
    IArray1D() : Base(), Allocator(Arrays::evalSizeCapacity(0)), capacity_(Arrays::evalSizeCapacity(0))
    {}
    /** constructor with a specified Range.
      *  @param I range of the container
     **/
    IArray1D( Range const& I): Base(I), Allocator(Arrays::evalRangeCapacity(I))
            , capacity_(this->sizeData())
    {}
    /** Misc constructor with first and last, initialization with a constant.
     *  @param I range of the container
     *  @param v initial value of the container
     **/
    IArray1D( Range const& I, Type const& v) : Base(I)
            , Allocator(Arrays::evalRangeCapacity(I))
            , capacity_(this->sizeData())
    { for (int i=this->begin(); i<this->end(); i++) this->data(i) = v;}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    IArray1D( const IArray1D &T, bool ref =false) : Base(T)
            , Allocator(T, ref)
            , capacity_(0)
    {
      if (!ref)
      { init1D(T.range());
        for (int j=this->begin(); j<this->end(); j++) this->data(j) = T.data(j);
      }
    }
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I range of the data to wrap
     **/
    IArray1D( IArray1D const& T, Range const& I) : Base(I)
            , Allocator(T, true)
            , capacity_(0)
     {}
    /** destructor: allocated memory is liberated by AllocatorBase base class.*/
    ~IArray1D() {}

  public:
    /** access to one element.
     *  @param pos index of the element
     **/
    inline Type& elt1Impl(int pos) { return this->data(pos);}
    /** access to one element const.
     *  @param pos index of the const element
     **/
    inline Type const& elt1Impl(int pos) const { return this->data(pos);}
    /** access to many elements.
     *  @param J the range of the elements
     **/
    inline Derived subImpl(Range const& J) const
    {
      if ((J.begin()<this->begin()))
      { STKOUT_OF_RANGE_1ARG(IArray1D::sub,J,J.begin()<begin());}
      if ((J.end()>this->end()))
      { STKOUT_OF_RANGE_1ARG(IArray1D::sub,J,J.end()>end());}
      return Derived(this->asDerived(), J);
    }
    /** New beginning index for the object.
     *  @param beg the index of the first column to set
     **/
    void shiftImpl(int const& beg =1)
    {
      // compute increment
      int inc = beg - this->begin();
      if (inc == 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray1D::shift,beg,cannot operate on references);}
        // translate cols_
        this->incRange(inc);
        // translate data
        this->shiftData(beg);
    }
    /**  Resize the container.
     * - call @c shift
     * - call @c pushBack if there will be more elements
     * - call @c popBack if three will be less elements
     * @param I the range to set to the List1D
     **/
    inline Derived& resizeImpl(Range const& I)
    {
      // check if there is something to do
      if ( this->range() == I) return this->asDerived();
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray1D::resize,I,cannot operate on references);}
      // translate
      this->shift(I.begin());
      // compute number of elements to delete or add
      const int inc = I.lastIdx() - this->lastIdx();
      // adjust size of the container
      if (inc > 0) this->pushBack(inc);  // more elements
      else         this->popBack(-inc);  // less elements
      return this->asDerived();
    }
    /** @return the maximum possible number of elements without
     *  reallocation. */
    inline int capacity() const { return capacity_;}
    /** reserve internal memory for at least size elements.
     *  @param size number of elements to reserve
     **/
    void reserve(int const& size)
    {
      // nothing to do
      if (size < this->capacity()) return;
      // is this structure a ptr ?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray1D::reserve,size,cannot operate on references);}
      Allocator::realloc(Range(this->begin(), size));
      // if no alloc error update size
      this->setCapacity(size);
    }
    /** Clear the object. Memory is liberated and the
     *  range of the Container is set to 0:-1 or 1:0 (@see baseIdx).
     **/
    void clear()
    {
      if (this->isRef()) return;   // Nothing to do for ref
      this->freeMem();  // Free Mem
      this->setRange(); // Set dimension to default
    }
    /** move T to this.
     *  @note : T is not modified but just set as a reference of the data it was responsible.
     *  @param T the container to move.
     **/
     void move(Derived const& T)
     {
       if (this->asPtrDerived() == &T) return;
       if (!this->isRef()) { freeMem();}
       // move Allocator part
       Allocator::move(T);
       // Set IContainer1D part
       this->setRange(T.range());
     }
    /** Add n Elts to the container.
     *  @param n number of elements to add
     **/
    void pushBack( int const& n=1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray1D::pushBack,n,cannot operate on references);}
      // If the container is empty : create it
      if (this->empty())
        this->initialize(Range(this->begin(), n));
      else
        this->insertElt(this->lastIdx()+1, n);
    }
    /** Delete last elts of the container.
     *  @param n number of elts to delete
     **/
    void popBack(int const& n = 1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(IArray1D::popBack,n,cannot operate on reference);}
      // check if there is enough elts to erase
      if (this->size()<n)
      { STKOUT_OF_RANGE_1ARG(Array1D::popBack,n,size() < n);}
      // update range
      this->decLast(n);
      // if there is no more elts
      if (this->size() == 0) this->freeMem();
    }
    /** Delete n elements at the pos index to the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
    **/
    void erase(int pos, int const& n=1)
    {
      // if n==0 nothing to do
      if (n<=0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray1D::erase,pos, n,cannot operate on reference);}
      // check bounds
      if (this->begin() > pos)
      { STKOUT_OF_RANGE_2ARG(IArray1D::erase,pos, n,begin() > pos);}
      if (this->lastIdx() < pos)
      { STKOUT_OF_RANGE_2ARG(IArray1D::erase,pos, n,lastIdx() < pos);}
      if (this->lastIdx() < pos+n-1)
      { STKOUT_OF_RANGE_2ARG(IArray1D::erase,pos, n,lastIdx() < pos+n-1);}
      // translate remaining elts
      const int last = this->lastIdx()-n;
      for (int k=pos; k<=last; k++) this->data(k)  = this->data(k+n);
      // update dimensions
      this->decLast(n);
      // if there is no more cols, free mem
      if (this->size() == 0) this->freeMem();
    }
    /** Insert n elts at the position pos of the container. The bound
     *  end_ should be modified at the very end of the insertion as pos
     *  can be a reference to it.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    void insertElt( int pos, int const& n =1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(IArray1D::insertElt,pos,n,cannot operate on references);}
      // check indices
      if (this->begin() > pos)
      { STKOUT_OF_RANGE_2ARG(IArray1D::insertElt,pos, n,begin() > pos);}
      if (this->lastIdx()+1 < pos)
      { STKOUT_OF_RANGE_2ARG(IArray1D::insertElt,pos, n,lastIdx()+1 < pos);}
      // allocate, if necessary, the mem for the elts
      if (this->capacity() < this->size()+n)
      {
        // temporary empty container
        IArray1D Taux;
        // save elts
        exchange(Taux);
        // compute range of the container after insertion
        Range range(Taux.range());
        range.incLast(n);
        // initialize
        try
        {
          this->init1D(range);
        }
        catch (runtime_error & error)   // if an error occur
        {
          exchange(Taux); // restore elts
          throw error;      // and send again the Exception
        }
        // reset initial stored in range
        this->setRange(Taux.range());
        // copy first elts
        for (int k=this->begin(); k<pos; k++) this->data(k) = Taux.data(k);
        // translate and copy last elts
        for (int k=this->lastIdx(); k>=pos; k--) this->data(k+n) = Taux.data(k);
      }
      else // enough space -> shift the last elts
      { for (int k=this->lastIdx(); k>=pos; k--) this->data(k+n) = this->data(k);}
      // update range_
      this->incLast(n);
    }
    /** Insert element @c v in the range @c I of the Array.
     *  @param I range of the index where to insert elements
     *  @param v the value to insert
     **/
    void insert( Range const& I, Type const& v)
    {
      this->insertElt(I.begin(), I.size());
      for (int i=I.begin(); i<=I.lastIdx(); i++) this->data(i) = v;
    }
    /** STL compatibility : push front an element.
     *  @param v value to append
     **/
    inline void push_front(Type const& v)
    { insert(Range(this->begin(), 0), v);}

    /** STL compatibility : append an element v.
     *  @param v value to append
     **/
    inline void push_back(Type const& v)
    {
      this->pushBack();
      this->back() = v;
    }
    /** Swapping the pos1 elt and the pos2 elt.
     *  @param pos1 position of the first elt
     *  @param pos2 position of the second elt
     **/
    void swap(int pos1, int pos2)
    {
      if (this->begin() > pos1)
      { STKOUT_OF_RANGE_2ARG(IArray1D::swap,pos1,pos2,begin()>pos1);}
      if (this->lastIdx() < pos1)
      { STKOUT_OF_RANGE_2ARG(IArray1D::swap,pos1,pos2,lastIdx()<pos1);}
      if (this->begin() > pos2)
      { STKOUT_OF_RANGE_2ARG(IArray1D::swap,pos1,pos2,begin()>pos2);}
      if (this->lastIdx() < pos2)
      { STKOUT_OF_RANGE_2ARG(IArray1D::swap,pos1,pos2,lastIdx()<pos2);}
      // swap
      std::swap(this->data(pos1), this->data(pos2));
    }
    /** exchange this Container with T.
     *  @param T the Array to exchange with T
     **/
    void exchange(IArray1D &T)
    {
      // exchange AllocatorBase part
      Allocator::exchange(T);
      // exchange ITContainer1D part
      Base::exchange(T);
      // swap IArray1D part
      std::swap(capacity_, T.capacity_);
    }
    /** overwrite @c this with @c src.
     *  @note If the size match, @c this is not resized, and in this case,
     *  the method take care of the possibly of overlapping.
     *  @param src the container to copy
     **/
    Derived& copy( IArray1D const& src)
    {
      // Resize if necessary.
      if ( this->sizeRows() != src.sizeRows() ) { this->resize(src.rows());}
      // Copy without overlapping
      if (src.beginRows()>this->beginRows())
      {
        for(int iSrc=src.begin(), iDst=this->begin(); iSrc<src.end(); iSrc++, iDst++)
        { this->elt(iDst) = src.elt(iSrc);}
        return this->asDerived();
      }
      // src.beginRows()<this->beginRows()
      for(int iSrc=src.lastIdx(), iDst=this->lastIdx(); iSrc!=src.begin(); iSrc--, iDst--)
      { this->elt(iDst) = src.elt(iSrc);}

      return this->asDerived();
    }

  protected:
    /** Set the maximum possible number of elements without
     *  reallocation.
     *  @param capacity capacity of the container
     **/
    inline void setCapacity(int const& capacity =0)
    { capacity_ = capacity;}
    /** function for memory allocation and initialization.
     *  This method will free all allocated memory owned by this
     *  container before initialization.
     *  @param I range of the container
     **/
    void initialize(Range const& I)
    {
      // check if there is memory allocated
      this->clear();
      // if we initialize the memory the container is no more a reference
      this->setRef(false);
      // try to allocate memory
      this->init1D(I);
      // set the range of the container if init1D is successful
      this->setRange(I);
    }
    /** function for memory allocation and initialization.
     *  The range is not set in this method. If an
     *  error occur, we set the range of the container to default.
     *  @param I range of the container
     **/
    void init1D(Range const& I)
    {
      // compute the size necessary (can be 0)
      int size = Arrays::evalSizeCapacity(I.size());
      // try to allocate memory
      try
      {
        // initialize Elts
        this->malloc(Range(I.begin(), size));
      }
      catch (runtime_error const& error)   // if an error occur
      {
        // set default capacity (0)
        setCapacity();
        // set default range
        this->setRange();
        // throw the error
        throw error;
      }
      // set new capacity if no error occur
      setCapacity(size);
    }
    /** Method for memory deallocation. Memory is liberated and the
     *  range of the Container is set to begin:begin-1.
     **/
    void freeMem()
    {
      if (this->isRef()) return;   // Nothing to do for ref
      this->free1D();              // free the elts
    }
    /** Method for memory deallocation. If the derived class
     *  use indirection, we have to free the mem if necessary prior
     *  to this method. The beginning of the Container is not modified
     **/
    void free1D()
    {
      // Nothing to do for ref
      if (this->isRef()) return;
      // free allocated memory
      this->free();
      // set capacity to default
      this->setCapacity();
      // set range of the Cols to default
      this->setRange(Range(this->begin(), -1));
    }
  private:
    /** capacity of the array. */
    int capacity_;
};

} // namespace STK

#endif // STK_IARRAY1D_H
