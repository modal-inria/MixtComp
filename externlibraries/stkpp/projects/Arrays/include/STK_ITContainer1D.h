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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::hidden::TContainer
 * Purpose:  Define the Interface 1D templated Container class.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_ITContainer1D.h
 *  @brief Interface class for 1D Container.
 **/

#ifndef STK_ITCONTAINER1D_H
#define STK_ITCONTAINER1D_H

#include "../../Sdk/include/STK_Traits.h"
#include "../../STKernel/include/STK_Exceptions.h"

#include "STK_ITContainer.h"
#include "STK_IContainer1D.h"

namespace STK
{
/** @ingroup Sdk
 *  @brief Interface base class for homogeneous 1D containers.
 *
 * The ITContainer1D class is the templated base class for all
 * homogeneous one-dimensional containers containing element of type @c Type.
 *
 * Implement the curious recursive template paradigm : the template
 * parameter @c Derived is the name of the class that
 * implements @c ITContainer1D. For example
 * <code>
 * template<class Type>
 * class Derived : public ITContainer1D<Type, Derived<Type> >
 * {...}
 * </code>
 *
 * The pseudo virtual function defined in this interface have the
 * following definition:
 * @code
 *   Type& elt(int const& pos);
 *   Type const& elt(int const& pos) const;
 *   Derived elt(Range const& I) const;
 * @endcode
 *
 * All these methods have to be implemented in the Derived class.
 **/
template <class Derived, Arrays::Structure struture_ = (Arrays::Structure)hidden::Traits<Derived>::structure_>
class ITContainer1D;

template <class Derived>
class ITContainer1D<Derived, Arrays::vector_> : public IContainer1D
                                              , public ITContainer<Derived, Arrays::vector_>
{
  protected:
    /** Type of the Base container */
    typedef ITContainer<Derived, Arrays::vector_ > Base;

    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;

    /** Default constructor: empty List1D */
    inline ITContainer1D() : IContainer1D(), Base() {}
    /** constructor with a specified range.
     *  @param I : the range of the container
     **/
    inline ITContainer1D( Range const& I): IContainer1D(I), Base() {}
    /** Copy constructor
     *  @param T the container to copy
     **/
    inline ITContainer1D( ITContainer1D const& T): IContainer1D(T) , Base() {}
    /** destructor. */
    inline ~ITContainer1D() {}

  public:
    /**  @return the index of the first element */
    inline int const& firstIdx() const { return IContainer1D::firstIdx();}
    /**  @return the index of the last element*/
    inline int const& lastIdx() const  { return IContainer1D::lastIdx();}
    /** @return the size of the container */
    inline int const& size() const  { return IContainer1D::size();};
    /** @return the range of the container */
    inline Range const& range() const  { return IContainer1D::range();}
    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return range().empty();}
    /** Access to many elements.
     *  @param I the range of the elements
     *  @return A container with the elements in the range I of this
     **/
    inline SubVector sub(Range const& I) const
    { return this->asDerived().sub(I);}
    /**  @param beg the index of the first column to set */
    void shift(int const& beg) { this->asDerived().shift(beg);}
    /** @return the resized container.
     *  @param I the range of the container
     **/
    inline Derived& resize(Range const& I) { return this->asDerived().resize(I);}
    /** @return the Horizontal range (1 column) */
    inline Range cols() const { return Range(1);};
    /** @return the Vertical range*/
    inline Range rows() const { return IContainer1D::range();}
};

template <class Derived>
class ITContainer1D<Derived, Arrays::point_> : public IContainer1D
                    , public ITContainer<Derived, Arrays::point_>
{
  protected:
    /** Type of the Base container */
    typedef ITContainer<Derived, Arrays::point_ > Base;

    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;

    /** Default constructor: empty List1D */
    inline ITContainer1D() : IContainer1D(), Base() {}
    /** constructor with a specified range.
     *  @param I : the range of the container
     **/
    inline ITContainer1D( Range const& I): IContainer1D(I), Base() {}
    /** Copy constructor
     *  @param T the container to copy
     **/
    inline ITContainer1D( ITContainer1D const& T): IContainer1D(T) , Base() {}
    /** destructor. */
    inline ~ITContainer1D() {}

  public:
    /**  @return the index of the first element */
    inline int const& firstIdx() const { return IContainer1D::firstIdx();}
    /**  @return the index of the last element*/
    inline int const& lastIdx() const  { return IContainer1D::lastIdx();}
    /** @return the size of the container */
    inline int const& size() const  { return IContainer1D::size();};
    /** @return the range of the container */
    inline Range const& range() const  { return IContainer1D::range();}
    /** @return @c true if the container is empty, @c false otherwise */
    inline bool empty() const { return range().empty();}
    /** Access to many elements.
     *  @param I the range of the elements
     *  @return A container with the elements in the range I of this
     **/
    inline SubVector sub(Range const& I) const
    { return this->asDerived().elt(I);}
    /**  @param beg the index of the first column to set */
    void shift(int const& beg) { this->asDerived().shift(beg);}
    /** @return the resized container.
     *  @param I the range of the container
     **/
    inline Derived& resize(Range const& I) { return this->asDerived().resize(I);}

    /** FIXME. @return only the Horizontal range */
    inline Range cols() const { return IContainer1D::range();};
    /** FIXME. @return the Vertical range*/
    inline Range rows() const { return Range(1);}
};

} // namespace STK

#endif
// STK_ITCONTAINER1D_H
