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
 * Purpose:  Define the fundamental type int.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Integer.h
 *  @brief In this file we define the fundamental type Integer.
 **/

#ifndef STK_INTEGER_H
#define STK_INTEGER_H

#include "STK_String_Util.h"
#include <map>

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

/** @ingroup Base
 *  extract an int from an input stream. If the extraction failed, value is
 *  set to a NA value.
 *  @param is the stream
 *  @param value the value extracted from the stream.
 *  @return the current stream. If the extraction failed, the stream is unmodified.
 **/
istream& streamToInt(istream& is, int& value);

/** @ingroup Base
 *  Convert a String to an int.
 *  @param type the String we want to convert
 *  @return the int represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
int stringToInt( String const& type);

/** @ingroup Base
 *  Convert a String to an int using a map.
 *  @param type the String we want to convert
 *  @param mapping the mapping between the string and the Int
 *  @return the Int represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
int stringToInt( String const& type, std::map<String, int> const& mapping);

/** @ingroup Base
 *  Convert a Int to a String.
 *  @param type the type of int we want to convert
 *  @return the string associated to this type.
 **/
String intToString( int const& type);

/** @ingroup Base
 *  Convert an int to a String.
 *  @param type the type of int we want to convert
 *  @param mapping the mapping between the Int and the String
 *  @return the string associated to this type.
 **/
String intToString( int const& type, std::map<int, String> mapping);

/** @ingroup Base
 *  @brief specialization for int
 *  @param s the String to convert
 *  @return The value to get from the String
 **/
template<>
inline int stringToType<int>( String const& s)
{ return stringToInt(s);}

/** @ingroup Base
 *  @brief specialization for int
 *  @param t The Int to convert to String
 *  @param f flag, by default write every number in decimal
 **/
template<>
inline String typeToString<int>( int const& t, std::ios_base& (*f)(std::ios_base&))
{ return intToString(t);}

} // namespace STK

#endif /*STK_INTEGER_H*/
