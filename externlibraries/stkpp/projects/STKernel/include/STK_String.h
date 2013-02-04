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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  Base
 * Purpose:  Define the fundamental type String.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_String.h
 *  @brief In this file we define the fundamental type String.
 **/

#ifndef STK_STRING_H
#define STK_STRING_H

// C++ headers
#include <string>

// STK headers
#include "STK_Char.h"
#include "STK_Arithmetic.h"
#include "STK_IdTypeImpl.h"
#include "STK_Stream.h"
#include "STK_Proxy.h"

namespace STK
{

/** @ingroup Base
 *  @brief STK fundamental type of a String.
 *
 *  The type String is defined for the internal representation 
 *  of the string variables (strings).
 **/
typedef std::basic_string<Char> String;

/** @ingroup Arithmetic
 *  @brief Specialization for String.
 * 
 * The STK fundamental type String use the empty String to represent
 * NA String.
 */
template<>
struct Arithmetic<String> : public std::numeric_limits<String>
{
  /** Adding a Non Avalaible (NA) special String (the empty String)
   **/
  static String NA() throw()
  { return String();}

  /** True if the type has a representation for a "Not Available."
   **/
  static const bool hasNA = true;

  /** Test if x is a Non Avalaible (NA) String.
   *  @param x the String to test.
   *  @return @c true if @c x is a NA String, @c false otherwise
   **/
  static bool isNA(String const& x) throw()
  { return (x == NA());}
    
  /** Test if x is  infinite.
   *  @param x the String to test.
   *  @return @c false
   **/
  static bool isInfinite(String const& x) throw()
  { return false; }

  /** Test if x is  finite.
   *  @param x the String to test.
   *  @return @c true if @c x not a NA string, @c false otherwise
   **/
  static bool isFinite(String const& x) throw()
  { return (!isNA(x));}
};

/** @ingroup RTTI 
 *  @brief Specialization of the IdTypeImpl for the Type String.
 * 
 *  @return the IdType of a String.
 **/
template<>
struct IdTypeImpl<String>
{
  /** Give the IdType of the type String.
   * @return the IdType string
   **/
  static IdType returnType()
  { return(string);}
};

/** @ingroup Base
  * @brief Representation of a Not Available value.
  *
  * We represent a Not Available value of any type as a "." (like in
  * (SAS(R))) for the end-user.
  **/
static const String STRING_NA      = String(_T("."));

/** @ingroup Base
  * @brief Size (in number of Char) of a Not Available value.
  *
  * We represent a Not Available value of any type as a "." (like in
  * (SAS(R))) for the end-user.
  **/
static const int STRING_NA_SIZE  = 1;

/** @ingroup Base
 *  @brief Overloading of the istream >> for the type String.
 *  @param is the input stream
 *  @param input the value to get from the stream
 **/
inline istream& operator >> (istream& is, Proxy<String>& input)
{
  String buff;
  is >> buff;
  input = (buff  == STRING_NA) ? Arithmetic<String>::NA() : buff;
  return is;
}


} // namespace STK

#endif /*STK_STRING_H*/
