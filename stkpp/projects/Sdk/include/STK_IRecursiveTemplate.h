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
 * Project:  stkpp::Sdk
 * created on: 7 août 2011
 * Purpose:  Create an interface base class for class using the Recursive
 * template paradigm.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IRecursiveTemplate.h
 *  @brief In this file we define the Interface base class IRecursiveTemplate.
 **/

#ifndef STK_IRECURSIVETEMPLATE_H
#define STK_IRECURSIVETEMPLATE_H

namespace STK
{

/** @ingroup Sdk
 *  @brief Interface base class for all classes implementing the curious
 *  recursive template paradigm.
 *
 *  Implement the curious recursive template paradigm : the template
 *  parameter @c Derived is the type of the leaf class that
 *  implements @c IRecursiveTemplate. A constant reference/pointer
 *  on the derived type can be obtained using the methods
 *  @c asDerived and @c asPtrDerived.
 **/
template<class Derived>
class IRecursiveTemplate
{
  protected:
    /** constructor. */
    inline IRecursiveTemplate() {}
    /** destructor. */
    inline ~IRecursiveTemplate() {}

  public:
    /** static cast : return a reference of this with a cast to the
     *  derived class.
     *  @return a reference on this in the derived type
     **/
    inline Derived& asDerived()
    { return static_cast<Derived&>(*this); }
    /** static cast : return a const reference of this with a cast to
     *  the derived class.
     *  @return a constant reference on this in the derived type
     **/
    inline Derived const& asDerived() const
    { return static_cast<Derived const&>(*this); }
    /** static cast : return a ptr on a @c Derived of this with a cast to the
     *  derived class.
     *  @return a pointer on this in the derived type
     **/
    inline Derived* asPtrDerived()
    { return static_cast<Derived*>(this); }
    /** static cast : return a ptr on a constant @c Derived of this with a cast
     *  to the derived class.
     *  @return a constant pointer on this in the derived type
     **/
    inline Derived const* asPtrDerived() const
    { return static_cast<Derived const*>(this); }
    /** create a leaf using the copy constructor of the Derived class.
     *  @return A pointer on a cloned Derived of @c this
     **/
    inline Derived* clone() const
    { return new Derived(this->asDerived());}
    /** create a leaf using the copy constructor of the Derived class
     *  and a flag determining if the clone is a reference or not.
     *  @sa CArray, Array2D, Array2DVector, Array2DPoint
     *  @param isRef true if the clone object is a reference
     *  @return A pointer on a cloned Derived of @c this
     **/
    inline Derived* clone(bool isRef) const
    { return new Derived(this->asDerived(), isRef);}
};

} // namespace STK

#endif /* STK_IRECURSIVETEMPLATE_H */
