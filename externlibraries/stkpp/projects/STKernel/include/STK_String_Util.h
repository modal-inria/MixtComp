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
 * Project:  stkpp::STKernel::Base
 * Purpose:  Define miscellaneous utility functions for Strings.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_String_Util.h
 *  @brief In this file we define miscellaneous utilities
 *  functions for Strings and the global String used for handling the NA values.
 **/

#ifndef STK_STRING_UTIL_H
#define STK_STRING_UTIL_H

#include "STK_String.h"

namespace STK
{
/** @ingroup Base
 *  @brief convert a String to Type
 *
 *  This method return true if the String s could be converted into
 *  a correct Type t.
 *  http://www.codeguru.com/forum/showpost.php?p=678440&postcount=1
 *  http://c.developpez.com/faq/cpp/?page=strings#STRINGS_is_type
 *
 *  The conversion is successful if it does not remain
 *  Char inside the String.
 *  The operator >> have been overloaded for each base type in order to
 *  return a NA value if fail.
 *
 *  @param t The value to get from the String
 *  @param s the String to convert
 *  @param f flags
 *  @return @c true if the conversion succeed, @c false otherwise
 **/
template <class Type>
bool stringToType( Type  &t
                 , String const& s
                 , std::ios_base& (*f)(std::ios_base&) = std::dec
                 )
{
  istringstream iss(s);
  Proxy<Type> wrapper(t);
  bool flag1 = (iss >> f >> wrapper).fail();
  iss >> std::ws;
  // ok if the conversion success and the String is exhausted
  return ( !flag1 && iss.eof() );
}

/** @ingroup Base
 *  @brief convert a String to Type
 *
 *  This method return the String s converted into a correct Type t.
 *  http://www.codeguru.com/forum/showpost.php?p=678440&postcount=1
 *  http://c.developpez.com/faq/cpp/?page=strings#STRINGS_is_type
 *
 *  @param s the String to convert
 *  @param f flags
 *  @return The value to get from the String
 **/
template <class Type>
Type stringToType( String const& s
                 , std::ios_base& (*f)(std::ios_base&) = std::dec
                 )
{
  istringstream iss(s);
  Type t;
  Proxy<Type> wrapper(t);
  bool flag1 = (iss >> f >> wrapper).fail();
  iss >> std::ws;
  // ok if the conversion success and the String is exhausted
  if ( flag1 || !iss.eof() ) return Arithmetic<Type>::NA();
  return t;
}

/** @ingroup Base
 *  @brief convert a Type to String
 *
 *  This method return the Type t into a String s.
 *  @see http://www.codeguru.com/forum/showpost.php?p=678440&postcount=1
 *  @see http://c.developpez.com/faq/cpp/?page=strings#STRINGS_convertform
 *
 *  @param t The value to convert to String
 *  @param f flags
 **/
template <class Type>
String typeToString( Type const& t
                   , std::ios_base& (*f)(std::ios_base&) = std::dec
                   )
{
  ostringstream oss;
  oss << f << ConstProxy<Type>(t);
  return oss.str();
}

/** @ingroup Base
 *  @brief Overloading of the operator << for the type Type using a
 *  constant Proxy. All output stream should use a ConstProxy in
 *  a STK application. For the enumerated types, we have also to define
 *  the standard output.
 * @param os the output stream
 * @param output the value to send to the stream
 **/
template <class Type>
ostream& operator << (ostream& os, const ConstProxy<Type>& output)
{ return Arithmetic<Type>::isNA(output)
       ? (os <<  STRING_NA) : (os << static_cast<Type const &>(output));
}


/** @ingroup Base
 *  @brief Overloading of the operator >> for the type Type using a
 *  Proxy. All input stream should use a Proxy in a STK application.
 *  For the enumerated and String types, we have to overload the method.
 *  Due to the instruction
 *  @code
 *   is >> buff
 *  @endcode
 *  this operator will only work for the fundamental C/C++ types. For the other
 *  types, the operator
 *  @code
 *  operator >> (istream& is, Type& input);
 *  @endcode
 *  have to be implemented.
 *  @param is the input stream
 *  @param input the value to get from the stream
 **/
template <class Type>
istream& operator >> (istream& is, Proxy<Type>& input)
{
  Type buff;
  // get current position in the stream
  typename std::ios::pos_type pos = is.tellg();

  // If the standard Conversion failed
  if ((is >> buff).fail())
  {
    is.seekg(pos);
    // clear failbit state and eofbit state if necessary
    is.clear(is.rdstate() & ~std::ios::failbit);
    if (is.eof()) { is.clear(is.rdstate() & ~std::ios::eofbit);}
    // in all case input is a NA object
    input = Arithmetic<Type>::NA();
    // Create a String buffer
    Char Lbuff[STRING_NA_SIZE+1];
    is.getline(Lbuff, STRING_NA_SIZE+1);
    // if we don't get a NA String, rewind stream
    if (!(STRING_NA.compare(Lbuff) == 0)) { is.seekg(pos);}
  }
  else
  { input = buff;}
  // return the stream
  return is;
}

/** @ingroup Base
 *  @brief convert the characters of the String to upper case
 *
 *  @param s The String to convert
 *  @return the string converted to upper case
 **/
String& toUpperString( String& s);

/** @ingroup Base
 *  @brief convert the characters of the String to upper case
 *
 *  @param s The String to convert
 *  @return a String in upper case
 **/
String toUpperString( String const& s);

} // namespace STK

#endif /* STK_STRING_UTIL_H */
