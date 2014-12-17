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
 * Purpose:  Define the List1D class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_List1D.h
  * @brief This is an implementation of the Interface ITContainer1D.
 **/

#ifndef STK_LIST_H
#define STK_LIST_H

#include "Arrays/include/STK_IContainerRef.h"
#include "Arrays/include/STK_ITContainer1D.h"

#include "STK_Cell.h"

namespace STK
{
template< class Type> class List1D;


namespace hidden
{
/** @ingroup hidden
 *  Specialization of the Traits class for List1D class.
 **/
template<class Type_>
struct Traits< List1D<Type_> >
{
  typedef Type_          Type;
  typedef List1D<Type_> Row;
  typedef List1D<Type_> Col;
  typedef List1D<Type_> SubRow;
  typedef List1D<Type_> SubCol;
  typedef List1D<Type_> SubArray;
  typedef List1D<Type_> SubVector;
  enum
  {
    structure_ = Arrays::point_,
    orient_    = Arrays::by_row_,
    sizeCols_  = UnknownSize,
    sizeRows_  = 1,
    storage_ = Arrays::dense_ // always dense
  };
};


} // namespace hidden

/** @ingroup DManager
  * @brief Templated One dimensional Horizontal List.
  *
  * A List1D is an implementation of the Interface ITContainer1D for list
  * of homogeneous objects.
  **/
template<class Type_>
class List1D : public ITContainer1D< List1D<Type_> >, public IContainerRef
{
  public:
    typedef typename hidden::Traits< List1D <Type_> >::Type Type;
    enum
    {
      structure_ = hidden::Traits< List1D <Type_> >::structure_,
      orient_    = hidden::Traits< List1D <Type_> >::orient_,
      sizeRows_  = hidden::Traits< List1D <Type_> >::sizeRows_,
      sizeCols_  = hidden::Traits< List1D <Type_> >::sizeCols_,
      storage_   = hidden::Traits< List1D <Type_> >::storage_
    };
    typedef CellHo<Type> Cell;
    typedef ITContainer1D< List1D<Type> > Base;

    /** Default constructor : empty List. */
    List1D() : Base(Range()), IContainerRef(false)
    { initialize(Range()); }
    /** constructor with specified Range.
     *  @param I range of the container
     **/
    List1D( Range const& I) : Base(I), IContainerRef(false)
    { initialize(I); }
    /** Misc constructor, initialization with a constant.
     *  @param I range of the container
     *  @param v initial value of the container
     **/
    List1D( Range const& I, Type const& v)
          : ITContainer1D<List1D >(I), IContainerRef(false)
    { initialize(I);
      Cell* p1  = p_begin_;
      for ( int j=this->begin(); j<=this->lastIdx(); j++)
      { (*p1) = v;             // overwrite the value of the current cell
        p1    = p1->getRight();   // Goto Right place
      }
    }
    /** Copy constructor
     *  @param T the list to copy
     **/
    List1D( const List1D<Type> &T) : Base(T), IContainerRef(false)
    {
      // initialize container
      initialize(T.range());
      // copy the container
      Cell* p1  = p_begin_;
      Cell* pt1 = T.p_begin_;
      for (int j=T.begin(); j<=T.lastIdx(); j++)
      { (*p1) = pt1->data();   // write the value of the current cell
        p1    = p1->getRight();   // Goto Right
        pt1   = pt1->getRight();  // Goto Right
      }
    }
  protected:
    /** constructor by reference, ref_=1.
     *  This constructor does not copy physically the elements contained
     *  in the Container. The List1D is wrapped by a reference List1D reduced
     *  to the range J.
     *
     *  @param p_first the first cell of the container to wrap
     *  @param p_last the last cell of the container to wrap
     *  @param J range of the data to wrap
     **/
    List1D( Cell* const & p_first, Cell* const & p_last, Range const& J)
          : Base(J), IContainerRef(true)
          , p_begin_(p_first)
          , p_last_(p_last)
    {
      currentPosition_  = this->begin();
      p_current_ = p_first;
    }
  public:
    /** destructor. */
    ~List1D() { if (!this->isRef()) freeMem();}
    /** @return the pointer on the first cell of the list */
    Cell const* const p_begin() const { return p_begin_;}
    /** @return the pointer on the last cell of the list */
    Cell const* const p_lastIdx() const { return p_last_;}
    /** access to one element.
     *  @param pos index of the element
     *  @return a reference on the element @c pos
     **/
     inline Type& elt1Impl(int pos)
    {
      moveCurrentPosition(pos);
      return (p_current_->data());
    }
    /** access to one element const.
     *  @param pos index of the const element
     *  @return a constant reference on the element @c pos
     **/
    inline Type const& elt1Impl(int pos) const
    {
      moveCurrentPosition(pos);
      return (p_current_->data());
    }
    /** access to many elements.
     *  @param J the range of the elements
     *  @return a list with a reference to the elements in the given range
     **/
    inline List1D subImpl(Range const& J) const
    {
      if ((J.begin()<this->begin()))
      { STKOUT_OF_RANGE_1ARG(List1D::subImpl,J,J.begin()<begin());}
      if ((J.lastIdx()>this->lastIdx()))
      { STKOUT_OF_RANGE_1ARG(List1D::subImpl,J,J.lastIdx()>lastIdx());}
      moveCurrentPosition(J.begin());
      Cell* p_first = p_current_;
      moveCurrentPosition(J.lastIdx());
      Cell* p_last = p_current_;
      return List1D(p_first, p_last, J);
    }
    /** New first index for the object.
     *  @param beg new first index of the Container.
     **/
    void shiftImpl(int const& beg)
    {
      if (this->begin() == beg) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(List1D::shift,beg,cannot operate on references);}
      //compute increment
      int inc = beg - this->begin();
      this->incRange(inc);  // update this->range_()
      currentPosition_ += inc;         // update current position
    }
    /** Clear the object. Memory is liberated and the
     *  range of the Container is set to 0:-1.
     **/
    void clear()
    {
      if (this->isRef()) return;   // Nothing to do for ref
      freeMem();        // Free mem
      this->setRange(); // Set to default the dimension
    }
    /** Add n Elts to the container.
     *  @param n number of elements to add
     **/
    void pushBack(int const& n=1)
    {
      // if n==0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(List1D::pushBack,n,cannot operate on references.);}
      // If the container is empty : create it
      if (this->empty())
      { initialize(Range(this->begin(), n));}
      else  // else adjust the beginning and the sizes
      {
        Cell *p1, *p2;            // Auxiliary cells;
        try
        { p1 = new Cell(p_last_);} // Create the end+1 cell
        catch (std::bad_alloc & error)
        { STKRUNTIME_ERROR_1ARG(List1D::pushBack,n,memory allocation failed);}
        // if no error is intercepted
        p_last_->setRight(p1);             // Set the right ending cell
        for (int j=2; j<=n; j++)    // main loop for the other cells
        { try
          { p2 = new Cell(p1);}  // try to allocate memory
          catch (std::bad_alloc & error) // if an alloc error occur
          {
            while ( p1 != p_last_)       // for all cells allocated
            { p2 = p1->getLeft();        // get the cell left
              delete p1;                 // delete the curent cell
              p1 = p2;                   // iterate
            }
            // set the original right side of cend
            p_last_->setRight(p_begin_);
            STKRUNTIME_ERROR_1ARG(List1D::pushBack,n,memory allocation failed);
          } // end catch
          // if no error
          p1->setRight(p2);  // Set the right cell of the current cell
          p1 = p2;           // Set the current cell to the the next cell
        }
        p1->setRight(p_begin_);    // the last cell point on the first cell
        p_begin_->setLeft(p1);     // the first cell point on the last cell
        p_last_ = p1;              // the last cell adress
        this->incLast(n);          // Update size of the container
      }
    }
    /** Insert element @c v in the range @c I of the List1D.
     *  @param I range of the index where to insert elements
     *  @param v the value tu insert
     **/
    void insert( Range const& I, Type const& v)
    {
      insertElt(I.begin(), I.size());
      for (int i=I.begin(); i<=I.lastIdx(); i++)
        this->elt(i) = v;
    }
    /** merge this with other. other will become a reference.
     *  @param other the list to append
     **/
    void merge( List1D const& other)
    {
      if (other.empty()) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_NO_ARG(List1D::merge(other),*this is a reference.);}
      // is T just a pointer?
      if (other.isRef())
      { STKRUNTIME_ERROR_NO_ARG(List1D::merge(other),other is a reference.);}
      // break const reference
      List1D& otherRef = const_cast<List1D&>(other);
      // compute horizontal range of the container after insertion
      Range range(this->range());

      // merge
      otherRef.p_begin_->setLeft(p_last_);
      otherRef.p_last_->setRight(p_begin_);
      p_last_->setRight(otherRef.p_begin_);
      p_last_ = otherRef.p_last_;
      p_begin_->setLeft(p_last_);

      // compute first index of the first column added
      const int first = range.lastIdx() + 1;
      this->setRange(range.incLast(otherRef.size()));
      otherRef.setRange(Range(first, other.size()));
      otherRef.setRef(true);

      // reset p_current_ to first position
      otherRef.currentPosition_ = first;
      otherRef.p_current_ = otherRef.p_begin_;
    }
    /** Insert n elts at the position pos of the container.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    void insertElt( int pos, int const& n =1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(List1D::insertElt,pos,n,cannot operate on references.);}
      if (this->begin() > pos)
      { STKOUT_OF_RANGE_2ARG(List1D::insertElt,pos,n,begin() > pos);}
      if (this->lastIdx()+1 < pos)
      { STKOUT_OF_RANGE_2ARG(List1D::insertElt,pos,n,lastIdx()+1 < pos);}
      // Move the current position to j
      moveCurrentPosition(pos);
      Cell *p0 = p_current_->getLeft(); // Get the j-1 cell
      Cell *p1 = p0;                    // Auxiliary cell;
      // main loop for the other cells
      for (int j1=1; j1<=n; j1++)
      {
        Cell *p2;        // Auxiliary cell;
        try
        { p2 = new Cell(p1);}  // try to allocate memory
        catch (std::bad_alloc & error) // if an alloc error occur
        { while ( p1 != p0)            // for all cells allocated
          { p2 = p1;                   // get the cell left
            delete p1;                 // delete the curent cell
            p1 = p2->getLeft();        // iterate
          }
          p0->setRight(p_current_);
          STKRUNTIME_ERROR_2ARG(List1D::insertElt,pos,n,memory allocation failed);
        } // catch block
        // if no error is intercepted
        p1->setRight(p2);  // Set the right cell of the current cell
        p1 = p2;           // iterate
      }
      p1->setRight(p_current_);     // the last cell point on the first cell
      p_current_->setLeft(p1);      // the first cell point on the last cell
      if ( pos==this->begin() )     // if the beginning was modified
      { p_begin_ = p0->getRight();} // set new beginning
      this->incLast(n);             // Update the size of the container
      currentPosition_ +=n;         // Update the current position
    }
    /** Delete n last elements of the container.
     *  @param n number of elts to delete
     **/
    void popBack(int const& n=1)
    {
      // if n<=0 nothing to do
      if (n <= 0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_1ARG(List1D::popBack,n,cannot operate on references.);}
      // if there is elts to erase
      if (this->size()<n)
      { STKOUT_OF_RANGE_1ARG(List1D::popBack,n,size() < n);}
      // erase elts with pos = end -n +1
      erase(this->lastIdx() - n +1, n);
    }
    /** Delete n elts at the pos index to the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
    **/
    void erase(int pos, int const& n=1)
    {
      // if n==0 nothing to do
      if (n<=0) return;
      // is this structure just a pointer?
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(List1D::erase,pos, n,cannot operate on references.);}
      // check bounds
      if (this->begin() > pos)
      { STKOUT_OF_RANGE_2ARG(List1D::erase,pos, n,begin()>pos);}
      if (this->lastIdx() < pos)
      { STKOUT_OF_RANGE_2ARG(List1D::erase,pos, n,lastIdx() < pos);}
      if (this->lastIdx() < pos+n-1)
      { STKOUT_OF_RANGE_2ARG(List1D::erase,pos, n,lastIdx() < pos+n-1);}
      // Move the current position to pos
      moveCurrentPosition(pos);
      Cell* p2 = p_current_;  // get pos-th cell
      moveCurrentPositionLeft();       // set current to (pos-1)th position
      // delete n cells
      for (int l=1; l<=n; l++)
      { Cell* p3 = p2->getRight();  // get right cell in p3
        delete p2;                          // delete current cell
        p2 = p3;                            // next
      }
      // If the last column have been erased update p_last_
      if (pos+n-1 == this->lastIdx()) { p_last_ = p_current_;}
      // Update the dimension of the container
      this->decLast(n);
      // If we have erased all cols
      if (this->size() == 0)
      { setDefault();}
      else
      { p2->setLeft(p_current_);        // p2 is the j+n cell
        p_current_->setRight(p2);       // p_current_ is on j-1 cell
        // If the first column has been erased
        if (pos == this->begin())
        { p_begin_  = p2;   // Set the new beg cell
          p_current_ = p2;   // p_current_
          currentPosition_++;       // and current position
        }
      }
    }
    /** Swapping the j1th column and the j2th column.
     *  @param j1 index of the first element to swap
     *  @param j2 index of the second element to swap
     **/
    void swap(int const& j1, int const& j2)
    {
#ifdef STK_BOUNDS_CHECK
      if (j1<this->begin())
      { STKOUT_OF_RANGE_2ARG(List1D::swap,j1, j2,j1<begin());}
      if (j1>this->lastIdx())
      { STKOUT_OF_RANGE_2ARG(List1D::swap,j1, j2,j1>lastIdx());}
      if (j2<this->begin())
      { STKOUT_OF_RANGE_2ARG(List1D::swap,j1, j2,j2<begin());}
      if (j2>this->lastIdx())
      { STKOUT_OF_RANGE_2ARG(List1D::swap,j1, j2,j2>lastIdx());}
#endif
      // get j1th value in aux
      moveCurrentPosition(j1);
      Cell *p1 = p_current_;
      Type aux = p1->data();
      // set j2th value in j1th position
      moveCurrentPosition(j2);
      (*p1) = p_current_->data();
      // set j2th value to aux
      (*p_current_) = aux;
    }
    /** operator = : overwrite the List1D with T.
     *  We resize the object if this and T does not have the same size
     *  but if they have the same size, we don't modify the range
     *  of the object.
     *  @param T the container to copy
     **/
    List1D& operator=(const List1D &T)
    {
      // We have to resize if this and T have not the same size
      // but if they have the same size, we don't scale the index
      if (this->size()!=T.size()) { this->resize(T.range());}

      /* copy without ovelapping.                                     */
      if (this->begin() < T.begin())
      { Cell *p1 = p_begin_, *pt1 = T.p_begin_;
        for (int j=1; j<=this->size(); j++)
        { (*p1) = pt1->data();   // overwrite the value
          p1    = p1->getRight();   // Goto Right for this
          pt1   = pt1->getRight();  // Goto Right for T
        }
      }
      else
      { Cell *p1 = p_last_, *pt1 = T.p_last_;
        for (int j=this->size(); j>=1; j--)
        { (*p1) = pt1->data();   // overwrite the value
          p1    = p1->getLeft();    // Goto Left for this
          pt1   = pt1->getLeft();   // Goto Left for T
        }
      }
      return *this;
    }
     /** Resize the container.
      * - call @c shift
      * - call @c pushBack if there will be more elements
      * - call @c popBack if three will be less elements
      * @param I the range to set to the List1D
      **/
     inline List1D<Type>& resizeImpl(Range const& I)
     {
       // check if there is something to do
       if ( this->range() == I) return *this;
       if (this->isRef())
       { STKRUNTIME_ERROR_1ARG(List1D::resizeImpl,I,cannot operate on references);}
       // translate beg
       shiftImpl(I.begin());
       // compute number of elements to delete or add
       const int inc = I.lastIdx() - this->lastIdx();
       // adjust size of the container
       if (inc > 0) this->pushBack(inc);  // more elements
       else         this->popBack(-inc);  // less elements
       return *this;
     }
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    List1D<Type>& operator=(Type const& v)
    {
      Cell* p1 = p_begin_;
      for (int j=1; j<=this->size(); j++)
      { p1->setData(v);      // overwrite the value of the current cell
        p1    = p1->getRight(); // Goto Right
      }
      return *this;
    }
  protected:
    Cell *p_begin_;       ///< First Element of the List
    Cell *p_last_;        ///< Last Element of the List
    /** Protected function for initialization. */
    void initialize(Range const& I)
    {
       // set new dimensions
      this->setRange(I);
      if (this->empty())
      {
        setDefault(); return;
      }
      // Allocate memory for the cells
      Cell *p1, *p2;        // Auxiliary pointer for cells

      p1 = new Cell();        // pointer on the first cell
      p_begin_ = p1;                     // set the first cell
      // main loop for the other cells
      for (int j=this->begin()+1; j<=this->lastIdx(); j++)
      { try
        { p2 = new Cell(p1);}        // try to allocate memory
        catch (std::bad_alloc & error)       // if an alloc error occur
        { while (p1 != (Cell*)NULL)  // for all cells allocated
          { p2 = p1->getLeft();              // get the cell left
            delete p1;                       // delete the cell
            p1 = p2;                         // and iterate
          }
          // set default
          setDefault();
          this->setRange();
          // and throw the Exception
          STKRUNTIME_ERROR_1ARG(List1D::initialize,I,memory allocation failed);
        }
        // if no error is catched
        p1->setRight(p2);      // Set the right cell
        p1 = p2;               // and iterate
      }
      p_last_ = p1;              // Set the last cell
      p_last_->setRight(p_begin_);  // the last cell point on the first cell
      p_begin_->setLeft(p_last_);   // the first cell point on the last cell
      currentPosition_  = this->begin();    // current position is first position
      p_current_ = p_begin_;            // CurrentPositionent cell is first cell
    }
    /** Protected function for deallocation.*/
    void freeMem()
    {
      if (this->isRef()) return;   // Nothing to do for ref
      Cell *p2, *p1 =p_begin_;   // Auxiliary pointers for cells
      // for all cells
      for (int j=this->begin(); j<=this->lastIdx(); j++)
      { p2 = p1->getRight();               // get the right cell
        delete p1;                         // delete the curent cell
        p1 = p2;                           // and iterate
      }
      setDefault();
    }

  private:
    /** Current position of pointer p_current_ in the List1D */
    mutable int currentPosition_;
    /** Current position pointed in the List1D */
    mutable Cell *p_current_;

    /** set members values to default. */
    void setDefault()
    { p_begin_  = 0;
      p_last_  = 0;
      p_current_ = 0;
      currentPosition_  = this->begin();
    }

    /** Move CurrentPositionent position to left */
    inline void moveCurrentPositionLeft()  const
    {
      p_current_ = p_current_->getLeft();
      currentPosition_--;
    }
    /** move CurrentPositionent position to right */
    inline void moveCurrentPositionRight() const
    {
      p_current_ = p_current_->getRight();
      currentPosition_++;
    }
    /** Move the current position to pos
     *  @param pos the position to move
     **/
    void moveCurrentPosition(int pos) const
    {
      if (pos>currentPosition_)
      {
        if ((pos-currentPosition_) <= (this->lastIdx()-pos))
          for( ;currentPosition_!=pos; ) moveCurrentPositionRight();
        else  // else start from the end
          for( currentPosition_ = this->lastIdx(), p_current_ = p_last_; currentPosition_!=pos; )
            moveCurrentPositionLeft();
      }
      else
      {
        if ((currentPosition_-pos) <= (pos-this->begin()))
          for( ;currentPosition_!=pos; ) moveCurrentPositionLeft();
        else // else start from the beginning
          for( currentPosition_ = this->begin(), p_current_ = p_begin_; currentPosition_!=pos; )
            moveCurrentPositionRight();
      }
    }
 };

/** ostream for List1D.
 *  @param s the output stream
 *  @param V the List1D to write
 **/
template<class Type>
ostream& operator<<(ostream& s, const List1D<Type>& V)
{ return out1D(s,V);}

} // namespace STK

#endif
// STK_LIST_H
