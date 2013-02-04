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
 * Project:  stkpp:stkernel::base
 * Purpose:  Define the fundamental type int.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_Integer.h
 *  @brief In this file we define the fundamental type Integer.
 **/

#ifndef STK_INTEGER_H
#define STK_INTEGER_H

#include "STK_Arithmetic.h"
#include "STK_IdTypeImpl.h"

namespace STK
{
/** @ingroup Base
  *  @brief STK fundamental type of discrete values.
  *
  *  The type Interger is defined for the numerical computation and the
  *  internal representation of the discrete variables.
  **/
typedef int Integer ;

/** @ingroup Arithmetic
 *  @brief Specialization for int (long).
 * 
 *  We are using the largest element of the underlying
 *  Type for representing NA (not available) discrete numbers.
 */
template<>
struct Arithmetic<Integer>  : public std::numeric_limits<int>
{
  /** We are using the maximal value of the int Type for NA values. */
  static inline int max() throw()
  { return std::numeric_limits<int>::max() -1; }
  /** Adding a Non Avalaible (NA) special number. */
  static inline int NA() throw()
  { return std::numeric_limits<int>::max();}
  /** True if the type has a representation for a "Not Available". */
  static const bool hasNA = true;
  /** Test if x is a Non Avalaible (NA) special number.
   *  @param x the int number to test.
   **/
  static inline bool isNA(Integer const& x) throw()
  { return (x==std::numeric_limits<int>::max());}
  /** Test if x is  infinite.
   *  @param x the int number to test.
   **/
  static inline bool isInfinite(Integer const& x) throw() { return false; }
  /** Test if x is  finite : i.e. if x is not infinite and
   *   x is not a NA value.
   *  @param x the value to test.
   **/
  static inline bool isFinite(Integer const& x) throw() { return (!isNA(x));}
};

/** @ingroup RTTI 
 *  @brief Specialization of the IdTypeImpl for the Type int.
 **/
template<>
struct IdTypeImpl<Integer>
{
  /** Give the IdType of the type int. */
  static IdType returnType() { return(integer_);}
};
  
} // namespace STK

#endif /*STK_INTEGER_H*/
