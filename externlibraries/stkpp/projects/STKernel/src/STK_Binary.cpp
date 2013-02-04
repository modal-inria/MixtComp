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
 * Project:  stkpp::STKernel::Base
 * Purpose:  Implement the fundamental type Binary.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_Binary.cpp
 *  @brief In this file we implement the operator>> for the type Binary.
 **/

#include "../include/STK_Binary.h"

namespace STK
{

/* @ingroup stream
 *  @brief Overloading of the ostream << for the type Binary.
 *  @param os the output stream
 *  @param output the value to send to the stream
 **/
ostream& operator << (ostream& os, const Binary& output)
{
  return Arithmetic<Binary>::isNA(output)
         ? (os <<  STRING_NA) : (os << static_cast<int>(output));
}

/*  Overloading of the istream >> for the type Binary. */
istream& operator >> (istream& is, Proxy<Binary>& input)
{
  // get current file position
  std::ios::pos_type pos = is.tellg();
  // try to read a discrete value
  int buff;
  // failed to read a discrete value
  if ((is >> buff).fail())
  {
    is.seekg(pos);
    // clear failbit and eofbit state if necessary
    is.clear(is.rdstate() & ~std::ios::failbit);
    if (is.eof()) is.clear(is.rdstate() & ~std::ios::eofbit);
    // Try to read a NA value, in all case input is a NA object
    input = Arithmetic<Binary>::NA();
    Char Lbuff[STRING_NA_SIZE+1];
    is.getline(Lbuff, STRING_NA_SIZE+1);
    // if we don't get a NA String, rewind stream
    if (!(STRING_NA.compare(Lbuff) == 0)) { is.seekg(pos); }
  }
  else
    switch (buff)
    {
      case 0:
        input = zero_;
        break;
      case 1:
        input = one_;
        break;
      default:
        input = binaryNA_;
        break;
    }
  return is;
}

/* @ingroup Base
 *  Convert a String to a Binary.
 *  @param type the String we want to convert
 *  @return the Binary represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Binary stringToBinary( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("0"))) return zero_;
  if (toUpperString(type) == toUpperString(_T("1"))) return one_;
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
  if (it == mapping.end())  return binaryNA_;
  return it->second;
}

/* @ingroup Base
 *  Convert a TypeRegression to a String.
 *  @param type the type of Binary we want to convert
 *  @return the string associated to this type.
 **/
String binaryToString( Binary const& type)
{
  if (type == zero_)  return String(_T("0"));
  if (type == one_) return String(_T("1"));
  return STRING_NA;
}

/* @ingroup Base
 *  Convert a TypeRegression to a String.
 *  @param type the type of Binary we want to convert
 *  @param mapping the mapping between the Binary and the String
 *  @return the string associated to this type.
 **/
String binaryToString( Binary const& type, std::map<Binary, String> mapping)
{ return mapping.find(type)->second;}


} // namespace STK
