/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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
 * Purpose:  Define the Reference Container interface class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IContainerRef.h
 *  @brief This is an internal header file, included by other
 *  Containers library headers.
 *
 *  You should not attempt to use it directly but rather used one of the
 *  derived class, except if you want to create your own Container
 *  Class.
 **/

#ifndef STK_ICONTAINERREF_H
#define STK_ICONTAINERREF_H

namespace STK
{
/** @ingroup Arrays
 *  @brief Base class for all referencing containers.
 *
 *  The TRef class is the base class for all containers that
 *  can be referenced. If a container is derived from this class, then
 *  this container can be a reference (a wrapper) on an other container.
 *  A container R is a reference of the container A, if it wrap the
 *  data contained in A. In this case, the boolean @c ref_ is true.
 **/
template <int ref_> struct TRef;

/** @ingroup Arrays his object is never a reference */
template <>
struct TRef<0>
{ inline static bool isRef() { return false;} };

/** @ingroup Arrays This object is always a reference */
template <>
struct TRef<1>
{ inline static bool isRef() { return true;} };

/** @ingroup Arrays This object can be or not be a reference */
template <>
struct TRef<-1>
{
  protected:
    /** Default constructor We have to specify the member ref_.
     *  @param ref : false if this own its own data.
     **/
    inline explicit TRef( bool ref) : ref_(ref) {}
    /** destructor. */
    inline ~TRef() {}

  public:
    /** @return @c true if *this is reference container, @c false otherwise */
    inline bool isRef() const { return ref_;}
    /** swap this with the container T.
     *  @param T the container to swap with this
     **/
    inline void exchange(TRef const& T) { std::swap(ref_, T.ref_);}

    /** Modify the container : can become a reference or the owner of
     *  the data. To use with care if we want to avoid memory leak.
     *  @param ref : false if this own its own data.
     **/
    inline void setRef(bool ref) const { ref_ = ref;}

  private:
    /** Is it a "true" container or a wrapper ?
     *  ref_ should be @c false if this own its own data, @c true otherwise.
     **/
    mutable bool ref_;
};

/** @ingroup Arrays backward compatibility */
typedef TRef<-1> IContainerRef;

} // namespace STK

#endif
// STK_CONTAINERREF_H
