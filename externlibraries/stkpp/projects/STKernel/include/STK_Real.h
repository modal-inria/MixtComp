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
 * Project:  Base
 * Purpose:  Define the fundamental type Real.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_Real.h
 *  @brief In this file we define the fundamental type Real.
 **/

#ifndef STK_REAL_H
#define STK_REAL_H

#include "STK_Arithmetic.h"
#include "STK_IdTypeImpl.h"

namespace STK
{

#ifndef Real
/** @ingroup Base
  *  @brief STK fundamental type of Real values.
  *
  *  The type Real is defined for the numerical computation and the
  *  internal representation of the continuous variables. By default it is the
  *  double type, but it can be overridden at compile-time, if needed.
  **/
#define Real double
#endif

/** @ingroup Arithmetic
 *   @brief Specialization for Real.
 * 
 *  We are using the quiet_NaN() representation of the underlying
 *  Type for representing NA (not available) numbers.
 */
template<>
struct Arithmetic<Real> : public std::numeric_limits<Real>
{
  /** We are using the quiet_NaN representation for NA numbers. */
  static const bool has_quiet_NaN = false;
  /** @return a Non Avalaible (NA) special number using the quiet_NaN method. */
  static Real NA() throw()
  { return std::numeric_limits<Real>::quiet_NaN();}
  /** True if the type has a representation for a "Not Available."    */
  static const bool hasNA = true;
  /** @return if x is a Non Avalaible (NA) special number
   *  @param x the Real to test.
   **/
  static bool isNA(Real const& x) throw()
  { return !(x==x);}
  /** @return if x is  infinite.
   *  @param x the Real to test.
   **/
  static bool isInfinite(Real const& x) throw()
  { return ( (x < -std::numeric_limits<Real>::max())
           ||(x > std::numeric_limits<Real>::max())
           );
  }
  /** @return if x is  finite.
   *  @param x the Real to test.
   **/
  static bool isFinite(Real const& x) throw()
  { return (!isNA(x) && !isInfinite(x));}
};

/** @ingroup RTTI 
 *  @brief Specialization of the IdTypeImpl for the Type Real.
 * 
 *  Return the IdType of a Real.
 **/
template<>
struct IdTypeImpl<Real>
{
  /** Give the IdType of the type Real.*/
  static IdType returnType()
  { return(real_);}
};
  
} // namespace STK

#endif /*STK_REAL_H*/
