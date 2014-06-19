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
 * Purpose:  Implement the fundamental type Integer.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Integer.cpp
 *  @brief In this file we implement the conversion functions for the type int.
 **/

#include "../include/STK_Integer.h"

namespace STK
{

istream& streamToInt(istream& is, int& value)
{
  // get current file position
  std::ios::pos_type pos = is.tellg();
  // try to read a discrete value
  int buff;
  // failed to read a discrete value
  if ((is >> buff).fail())
  {
    // reset position in the stream
    is.seekg(pos);
    // clear failbit and eofbit state if necessary
    is.clear(is.rdstate() & ~std::ios::failbit);
    if (is.eof()) is.clear(is.rdstate() & ~std::ios::eofbit);
    // Try to read a NA value, in all case value is a NA object
    value = Arithmetic<int>::NA();
    Char* buffer = new Char[stringNaSize()+1];
    is.getline(buffer, stringNaSize()+1);
    // if we don't get a NA String, rewind stream
    if (!(stringNa.compare(buffer) == 0)) { is.seekg(pos); }
    delete[] buffer;
  }
  return is;
}

/* @ingroup Base
 *  Convert a String to a int.
 *  @param type the String we want to convert
 *  @return the int represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
int stringToInt( String const& type)
{
  stringstream ss(type);
  int result;
  return ss >> result ? result : Arithmetic<int>::NA();
}

/* @ingroup Base
 *  Convert a String to a int using a map.
 *  @param type the String we want to convert
 *  @param mapping the mapping between the string and the int
 *  @return the int represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
int stringToInt( String const& type, std::map<String, int> const& mapping)
{
  std::map<String, int>::const_iterator it=mapping.find(type);
  if (it == mapping.end())  return Arithmetic<int>::NA();
  return it->second;
}

/* @ingroup Base
 *  Convert a TypeRegression to a String.
 *  @param type the type of int we want to convert
 *  @return the string associated to this type.
 **/
String intToString( int const& value)
{
  if (Arithmetic<int>::isNA(value)) return stringNa;
  stringstream ss;
  ss << value;
  return ss.str();
}

/* @ingroup Base
 *  Convert an int to a String.
 *  @param type the type of int we want to convert
 *  @param mapping the mapping between the int and the String
 *  @return the string associated to this type.
 **/
String intToString( int const& type, std::map<int, String> mapping)
{
  std::map<int, String>::const_iterator it=mapping.find(type);
  if (it == mapping.end())  return stringNa;
  return it->second;
}


} // namespace STK
