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
 * Project:  Base
 * Purpose:  Define the fundamental type Real.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Real.h
 *  @brief In this file we define the fundamental type Real.
 **/

#ifndef STK_REAL_H
#define STK_REAL_H

#include <map>
#include "STK_String.h"

#ifdef IS_RTKPP_LIB
#include "Rtkpp/include/STK_RcppTraits.h"
#endif

namespace STK
{
// forward declarations
template<typename Type> struct Arithmetic;
template<typename Type> struct IdTypeImpl;

#ifdef STKREALAREFLOAT
/**  @ingroup Base
  *  @brief STK fundamental type of Real values.
  *
  *  The type Real is defined for the numerical computation and the
  *  internal representation of the continuous variables. By default it is the
  *  @c double, but it can be overridden at compile-time by enabling
  *  the STKREALAREFLOAT macro.
  **/
typedef  float Real;

#else
/**  @ingroup Base
  *  @brief STK fundamental type of Real values.
  *
  *  The type Real is defined for the numerical computation and the
  *  internal representation of the continuous variables. By default it is the
  *  double type, but it can be overridden at compile-time, if needed.
  **/
typedef  double Real;

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
#ifdef IS_RTKPP_LIB
  enum
  {
    Rtype_ = hidden::RcppTraits<Real>::Rtype_
  };
#endif
  /** True if the type has a representation for a "Not Available."    */
  static const bool hasNA = true;
  /** @return a Non Available (NA) special number using the quiet_NaN method. */
  static inline Real NA() throw() { return std::numeric_limits<Real>::quiet_NaN();}
  /** We are using the quiet_NaN representation for NA numbers. */
  static const bool has_quiet_NaN = false;
  /** Test if x is a Non Available (NA) special number.
   *  @param x the Integer number to test.
   **/
  static inline bool isNA(Real const& x) throw() { return !(x==x);}
  /** @return @c true if x is  infinite.
   *  @param x the Real to test.
   **/
  static inline bool isInfinite(Real const& x) throw()
  { return ( (x < -std::numeric_limits<Real>::max())||(x > std::numeric_limits<Real>::max()));}
  /** @return @c true if x is  finite.
   *  @param x the Real to test.
   **/
  static inline bool isFinite(Real const& x) throw() { return (!isNA(x) && !isInfinite(x));}
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
  static Base::IdType returnType() { return(Base::real_);}
};

#ifdef IS_RTKPP_LIB
/** @ingroup Arithmetic
 *   @brief Specialization for const Real (needed by Rcpp).
 *
 *  We are using the quiet_NaN() representation of the underlying
 *  Type for representing NA (not available) numbers.
 */
template<>
struct Arithmetic<const Real>
{
  enum
  {
    Rtype_ = hidden::RcppTraits<Real>::Rtype_
  };
  /** True if the type has a representation for a "Not Available."    */
  static const bool hasNA = true;
  /** @return a Non Available (NA) special number using the quiet_NaN method. */
  static inline Real NA() throw() { return std::numeric_limits<Real>::quiet_NaN();}
  /** We are using the quiet_NaN representation for NA numbers. */
  static const bool has_quiet_NaN = false;
  /** Test if x is a Non Available (NA) special number.
   *  @param x the Integer number to test.
   **/
  static inline bool isNA(Real const& x) throw() { return !(x==x);}
  /** @return @c true if x is  infinite.
   *  @param x the Real to test.
   **/
  static inline bool isInfinite(Real const& x) throw()
  { return ( (x < -std::numeric_limits<Real>::max())||(x > std::numeric_limits<Real>::max()));}
  /** @return @c true if x is  finite.
   *  @param x the Real to test.
   **/
  static inline bool isFinite(Real const& x) throw() { return (!isNA(x) && !isInfinite(x));}
};

/** @ingroup RTTI
 *  @brief Specialization of the IdTypeImpl for the Type const Real (needed by Rcpp).
 *
 *  Return the IdType of a Real.
 **/
template<>
struct IdTypeImpl<const Real>
{
  /** Give the IdType of the type Real.*/
  static Base::IdType returnType() { return(Base::real_);}
};

#endif // IS_RTKPP_LIB

/** @ingroup Base
 *  @brief Convert a String to a Real.
 *  @param str the String we want to convert
 *  @return the Real
 **/
Real stringToReal( String const& str);

/** @ingroup Base
 *  Convert a String to a Real using a map.
 *  @param str the String we want to convert
 *  @param mapping the mapping between the string and the real
 *  @return the real represented by the String @c str. If the string
 *  does not match any known name, the @c NA value is returned.
 **/
Real stringToReal( String const& str, std::map<String, Real> const& mapping);

/** @ingroup Base
 *  Convert a Real to a String.
 *  @param value the Real we want to convert
 *  @param f format, by default write every number in decimal
 *  @return the string associated to this value.
 **/
String realToString( Real const& value, std::ios_base& (*f)(std::ios_base&) = std::dec);

/** @ingroup Base
 *  Convert an int to a String.
 *  @param str the String we want to convert
 *  @param mapping the mapping between the Real and the String
 *  @return the string associated to this type.
 **/
String realToString( Real const& str, std::map<Real, String> const& mapping);

/** @ingroup Base
 *  @brief specialization for Real
 *  @param str the String to convert
 *  @return The value to get from the String
 **/
template<>
inline Real stringToType<Real>( String const& str)
{ return stringToReal(str);}

/** @ingroup Base
 *  @brief specialization for Real
 *  @param value The Real to convert to String
 *  @param f format, by default write every number in decimal
 **/
template<>
inline String typeToString<Real>( Real const& value, std::ios_base& (*f)(std::ios_base&))
{ return realToString(value, f);}

} // namespace STK

#endif /*STK_REAL_H*/
