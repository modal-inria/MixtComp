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
 * Project:  stkpp:stkernel::base
 * Purpose:  Define the fundamental type Integer.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Integer.h
 *  @brief In this file we define the fundamental type Integer.
 **/

#ifndef STK_INTEGER_H
#define STK_INTEGER_H

#include <map>
#include "STK_String.h"

// for building
#ifdef IS_RTKPP_LIB
#include "Rtkpp/include/STK_RcppTraits.h"
#endif

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
 *  @brief Specialization for Integer (long).
 *
 *  We are using the largest element of the underlying
 *  Type for representing NA (not available) discrete numbers.
 */
template<>
struct Arithmetic<Integer>  : public std::numeric_limits<Integer>
{
#ifdef IS_RTKPP_LIB
  enum
  {
    Rtype_ = hidden::RcppTraits<Integer>::Rtype_
  };
#endif
  /** True if the type has a representation for a "Not Available". */
  static const bool hasNA = true;
  /** Adding a Non Available (NA) special number. */
  static inline Integer NA() throw() { return std::numeric_limits<Integer>::min();}
  /** Test if x is a Non Available (NA) special number.
   *  @param x the Integer number to test.
   **/
  static inline bool isNA(Integer const& x) throw()
  { return (x==std::numeric_limits<Integer>::min());}
  /** We are using the maximal value (positive or negative) of the Integer
    * type for NA values. */
  static inline Integer min() throw() { return std::numeric_limits<Integer>::min() +1; }
  /** @return @c true if x is  infinite : always false for Integer.
   *  @param x the Integer number to test.
   **/
  static inline bool isInfinite(Integer const& x) throw() { return false; }
  /** @return @¢ true if x is  finite : i.e. if x is not a NA value.
   *  @param x the value to test.
   **/
  static inline bool isFinite(Integer const& x) throw() { return (!isNA(x));}
};

/** @ingroup RTTI
 *  @brief Specialization of the IdTypeImpl for the type Integer.
 **/
template<>
struct IdTypeImpl<Integer>
{
  /** Give the IdType of the type Integer. */
  static inline Base::IdType returnType() { return(Base::integer_);}
};

#ifdef IS_RTKPP_LIB
/** @ingroup Arithmetic
 *  @brief Specialization for const Integer (long).
 *
 *  We are using the largest element of the underlying
 *  Type for representing NA (not available) discrete numbers.
 */
template<>
struct Arithmetic<const Integer>
{
  enum
  {
    Rtype_ = hidden::RcppTraits<const Integer>::Rtype_
  };
  /** True if the type has a representation for a "Not Available". */
  static const bool hasNA = true;
  /** Adding a Non Available (NA) special number. */
  static inline Integer NA() throw() { return std::numeric_limits<Integer>::min();}
  /** Test if x is a Non Available (NA) special number.
   *  @param x the Integer number to test.
   **/
  static inline bool isNA(Integer const& x) throw()
  { return (x==std::numeric_limits<Integer>::min());}
  /** We are using the maximal value (positive or negative) of the Integer
    * type for NA values. */
  static inline Integer min() throw() { return std::numeric_limits<Integer>::min() +1; }
  /** @return @c true if x is  infinite : always false for Integer.
   *  @param x the Integer number to test.
   **/
  static inline bool isInfinite(Integer const& x) throw() { return false; }
  /** @return @¢ true if x is  finite : i.e. if x is not a NA value.
   *  @param x the value to test.
   **/
  static inline bool isFinite(Integer const& x) throw() { return (!isNA(x));}
};

/** @ingroup RTTI
 *  @brief Specialization of the IdTypeImpl for the type const Integer (needed by Rcpp).
 **/
template<>
struct IdTypeImpl<const Integer>
{
  /** Give the IdType of the type Integer. */
  static inline Base::IdType returnType() { return(Base::integer_);}
};

#endif /* IS_RTKPP_LIB */

/** @ingroup Base
 *  @brief Convert a String to an Integer.
 *  @param str the String we want to convert
 *  @return the Integer represented by the String @c str. If the string
 *  does not match any known name, the NA value is returned.
 **/
inline Integer stringToInt( String const& str)
{ return stringToType<Integer>(str);}

/** @ingroup Base
 *  Convert a String to an Integer using a map.
 *  @param str the String we want to convert
 *  @param mapping the mapping between the string and the Int
 *  @return the Int represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Integer stringToInt( String const& str, std::map<String, Integer> const& mapping);

/** @ingroup Base
 *  Convert an Integer to a String.
 *  @param value the Integer we want to convert
 *  @param f format, by default write every number in decimal
 *  @return the string associated to this value.
 **/
inline String intToString( Integer const& value, std::ios_base& (*f)(std::ios_base&) = std::dec)
{ return typeToString<Integer>(value,f);}

/** @ingroup Base
 *  Convert an Integer to a String.
 *  @param value the Integer we want to convert
 *  @param mapping the mapping between Integer and String
 *  @return the String associated to this value.
 **/
String intToString( Integer const& value, std::map<Integer, String> const& mapping);

} // namespace STK

#endif /*STK_INTEGER_H*/
