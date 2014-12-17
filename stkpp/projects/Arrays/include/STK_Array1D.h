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
 * created on: 26 nov. 2007
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Array1D.h
  * @brief Implementation of the final class Array1D
 **/

#ifndef STK_ARRAY1D_H
#define STK_ARRAY1D_H

#include "STK_IArray1D.h"

namespace STK
{

template<class Type > class Array1D;


namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for Array1D class.
 **/
template<class _TYPE>
struct Traits< Array1D<_TYPE> >
{
  typedef _TYPE          Type;
  typedef Array1D<_TYPE> Row;
  typedef Array1D<_TYPE> Col;
  typedef Array1D<_TYPE> SubRow;
  typedef Array1D<_TYPE> SubCol;
  typedef Array1D<_TYPE> SubArray;
  typedef Array1D<_TYPE> SubVector;
  enum
  {
    structure_ = Arrays::vector_,
    orient_    = Arrays::by_col_,
    sizeCols_  = 1,
    sizeRows_  = UnknownSize,
    storage_   = Arrays::dense_ // always dense
  };
};

} // namespace hidden

/** @ingroup Arrays
 *  @brief Templated one dimensional Arrays.
 * 
 * An Array1D is a templated non-oriented container (even if the @c Traits
 * struct define it as column oriented) implementing the interface
 * class @c IArray1D.
 *
 * @note It is a final class for the curious recursive paradigm.
 *
 * @tparam the Type of the objects stored in the @c Array1D
 **/
template<class Type >
class Array1D : public IArray1D< Array1D<Type> >
{
  public:
    /** Type for the Array1DBase Class. */
    typedef IArray1D< Array1D<Type> > Base;
    /** Default constructor. */
    Array1D() : Base(){}
    /** constructor with a specified Range
     *  @param I range of the container
     **/
    Array1D( Range const& I) : Base(I) {}
    /** Misc constructor with beg and end, initialization with a constant.
     *  @param I range of the container
     *  @param v initial value of the container
     **/
    Array1D( Range const& I, Type const& v) : Base(I, v) {}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    Array1D( const Array1D &T, bool ref =false) : Base(T, ref) {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I range of the data to wrap
     **/
    Array1D( Array1D const& T, Range const& I) : Base(T, I) {}
    /** Wrapper constructor : the container is a reference of a C-Array.
     *  @param q pointer on data
     *  @param I range of the data
     **/
    Array1D( Type* q, Range const& I) : Base(q, I) {}
    /** destructor: allocated memory is liberated by AllocatorBase base class. */
    ~Array1D() {}
    /** operator = : overwrite the Array1D with t.
     *  We resize the object if this and T does not have the same size
     *  but if they have the same size, we don't modify the range
     *  of the object.
     *  @param T the container to copy
     **/
    inline Array1D<Type>& operator=(Array1D<Type> const& T)
    {
      // check size
      if (this->size()!=T.size()) this->resize(T.range());
      // copy without ovelapping.
      const int first = this->begin(), last = this->lastIdx();
      if (first < T.begin())
      { for (int i=first, j=T.begin(); i<=last; i++, j++) this->elt(i) = T.elt(j);}
      else
      { for (int i=last, j=T.lastIdx(); i>=first; i--, j--) this->elt(i) = T.elt(j);}
      return *this;
    }
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array1D& operator=(Type const& v)
    {
      for (int i=this->begin(); i<=this->lastIdx(); i++) this->elt(i)= v;
      return *this;
    }
};

/** @ingroup Arrays
 *  ostream for Array1D.
 *  @param s the output stream
 *  @param V the Array1D to write
 **/
template<class Type>
ostream& operator<<(ostream& s, const Array1D<Type>& V)
{ return out1D(s,V);}

} // namespace STK

#endif
// STK_ARRAY1D_H
