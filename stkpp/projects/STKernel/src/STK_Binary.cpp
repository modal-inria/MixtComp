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
 * Project:  stkpp::STKernel::Base
 * Purpose:  Implement the fundamental type Binary.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Binary.cpp
 *  @brief In this file we implement the Binary utilities methods.
 **/

#include "../include/STK_Binary.h"

namespace STK
{

/* @ingroup stream
 *  @brief Overloading of the ostream << for the type Binary.
 *  @param os the output stream
 *  @param value the value to send to the stream
 **/
ostream& operator << (ostream& os, Binary const& value)
{
  return Arithmetic<Binary>::isNA(value) ? (os <<  stringNa)
                                         : (os << static_cast<int>(value));
}

/*  Overloading of the istream >> for the type Binary. */
istream& operator >> (istream& is, Binary& value)
{
  // get current file position
  std::ios::pos_type pos = is.tellg();
  int res;
  // try to read an integer
  if (!(is >> res).fail())
  {
    switch (res)
    {
      case 0:
        value = zero_;
        break;
      case 1:
        value = one_;
        break;
      default:
        value = binaryNA_;
        is.clear(); is.seekg(pos); is.setstate(std::ios::failbit);
        break;
    }
  }
  else
  { value = binaryNA_;}
  return is;
}

/* @ingroup Base
 *  Convert a String to a Binary.
 *  @param str the String we want to convert
 *  @return the Binary represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Binary stringToBinary( String const& str)
{
  if (toUpperString(str) == toUpperString(_T("0"))) return zero_;
  if (toUpperString(str) == toUpperString(_T("1"))) return one_;
  return binaryNA_;
}

/* @ingroup Base
 *  Convert a String to a Binary using a map.
 *  @param type the String we want to convert
 *  @param mapping the mapping between the string and the Binary
 *  @return the Binary represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Binary stringToBinary( String const& type, std::map<String, Binary> const& mapping)
{
  std::map<String, Binary>::const_iterator it=mapping.find(type);
  return (it == mapping.end()) ? binaryNA_ : it->second;
}

/* @ingroup Base
 *  Convert a TypeRegression to a String.
 *  @param type the type of Binary we want to convert
 *  @return the string associated to this type.
 **/
String binaryToString( Binary const& value, std::ios_base& (*f)(std::ios_base&))
{
  if (Arithmetic<Binary>::isNA(value)) return stringNa;
  ostringstream os;
  os << f << static_cast<int>(value);
  return os.str();
}

/* @ingroup Base
 *  Convert a TypeRegression to a String.
 *  @param value the type of Binary we want to convert
 *  @param mapping the mapping between the Binary and the String
 *  @return the string associated to this type.
 **/
String binaryToString( Binary const& value, std::map<Binary, String> const& mapping)
{
  std::map<Binary, String>::const_iterator it=mapping.find(value);
  return (it == mapping.end()) ? Arithmetic<String>::NA() : it->second;
}


} // namespace STK
