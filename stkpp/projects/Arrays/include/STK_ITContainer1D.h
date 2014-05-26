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
 * Project:  stkpp::hidden::TContainer
 * Purpose:  Define the Interface 1D templated Container class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_ITContainer1D.h
 *  @brief in this file we define the interface class for ITContainer1D.
 **/

#ifndef STK_ITContainer2D1D_H
#define STK_ITContainer2D1D_H


#include "../../Sdk/include/STK_IRecursiveTemplate.h"
#include "../../Sdk/include/STK_Macros.h"

#include "STK_Traits.h"
#include "STK_IContainer1D.h"

namespace STK
{
/** @ingroup Arrays
 *  @brief Interface base class for homogeneous 1D containers.
 *
 * The ITContainer1D class is the templated base class for all
 * homogeneous one-dimensional containers containing element of type @c Type
 * where Type is note necessarily a scalar. Even if it is essentially a
 * not oriented (row or column) container, it have been defined as a container
 * with one column and many columns.
 *
 * Implement the curious recursive template paradigm : the template
 * parameter @c Derived is the name of the class that
 * implements @c ITContainer1D. For example
 * <code>
 * template<class Type>
 * class Derived : public ITContainer1D< Derived<Type> >
 * {...}
 * </code>
 *
 * The pseudo virtual function defined in this interface have the
 * following definition:
 * @code
 *   Type& elt1Impl(int const& pos);
 *   Type const& elt1Impl(int const& pos) const;
 *   SubVector subImpl(range const& I) const;
 *   void shiftImpl(int const& beg);
 *   Derived& resizeImpl(Range const& I);
 * @endcode
 * All these methods have to be implemented in the Derived class.
 *
 * @note The constant getter elt(pos) have to return a reference as we are using
 * derived class for storing any kind of data.
 **/
template <class Derived>
class ITContainer1D : public IContainer1D, public IRecursiveTemplate<Derived>
{
  protected:
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;

    /** Default constructor: empty List1D */
    inline ITContainer1D() : IContainer1D() {}
    /** constructor with a specified range.
     *  @param I : the range of the container
     **/
    inline ITContainer1D( Range const& I): IContainer1D(I) {}
    /** Copy constructor
     *  @param T the container to copy
     **/
    inline ITContainer1D( ITContainer1D const& T): IContainer1D(T) {}
    /** destructor. */
    inline ~ITContainer1D() {}

  public:
    /** @return the Horizontal range (1 column) */
    inline Range cols() const { return Range(1);};
    /** @return the Vertical range*/
    inline Range rows() const { return IContainer1D::range();}
//    /**  @return the index of the first element */
//    inline int firstIdx() const { return IContainer1D::firstIdx();}
//    /**  @return the index of the last element*/
//    inline int lastIdx() const  { return IContainer1D::lastIdx();}
//    /** @return the size of the container */
//    inline int size() const  { return IContainer1D::size();};
    /** @return the size of the container in number of columns */
    inline int sizeCols() const  { return 1;};
    /** @return the size of the container in number of rows */
    inline int sizeRows() const  { return IContainer1D::size();};
//    /** @return the range of the container */
//    inline Range const& range() const  { return IContainer1D::range();}
    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return range().empty();}
    /** @return the ith element for vector_, point_ and diagonal_ containers
     *  @param i index of the ith element
     **/
    inline Type& elt(int const& i) { return this->asDerived().elt1Impl(i);}
    /** @return a constant reference on the ith element for vector_, point_ and diagonal_ containers
     *  @param i index of the ith element
     **/
    inline Type const& elt(int const& i) const { return this->asDerived().elt1Impl(i);}
    /** @return the element ith element
     *  @param i index of the ith element
     **/
    inline Type& operator[](int const& i) { return elt(i);}
    /** @return a constant reference on the ith  element
     *  @param i index of the ith element
     **/
    inline Type const& operator[](int const& i) const { return elt(i);}
    /** @return safely the jth element
     *  @param i index of the element
     **/
    Type& at(int i)
    {
      if (this->asDerived().firstIdx() > i)
      { STKOUT_OF_RANGE_1ARG(ITContainer1D::at, i, firstIdx() > i);}
      if (this->asDerived().lastIdx() < i)
      { STKOUT_OF_RANGE_1ARG(ITContainer1D::at, i, lastIdx() < i);}
      return elt(i);
    }
    /** @return safely the constant jth element
     *  @param i index of the element
     **/
    Type const& at(int i) const
    {
      if (this->asDerived().firstIdx() > i)
      { STKOUT_OF_RANGE_1ARG(ITContainer1D::at, i, firstIdx() > i);}
      if (this->asDerived().lastIdx() < i)
      { STKOUT_OF_RANGE_1ARG(ITContainer1D::at, i, lastIdx() < i);}
      return elt(i);
    }

    /** @return a reference on the first element. */
    inline Type& front() { return elt(firstIdx());}
    /** @return a constant reference on the first element */
    inline Type const& front() const { return elt(firstIdx());}
    /** @return a reference on the last element */
    inline Type& back() { return elt(lastIdx());}
    /** @return a constant reference on the last element */
    inline Type const& back() const { return elt(lastIdx());}
    /** Access to many elements.
     *  @param I the range of the elements
     *  @return A container with the elements in the range I of this
     **/
    inline SubVector sub(Range const& I) const { return this->asDerived().subImpl(I);}
    /**  @param beg the index of the first column to set */
    inline void shift(int const& beg) { this->asDerived().shiftImpl(beg);}
    /** @return the resized container.
     *  @param I the range of the container
     **/
    inline Derived& resize(Range const& I) { return this->asDerived().resizeImpl(I);}
};

} // namespace STK

#endif
// STK_ITCONTAINER1D_H
