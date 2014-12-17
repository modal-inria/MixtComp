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
 *  @brief In this file we implement the conversion functions for the type Integer.
 **/

#include "../include/STK_Integer.h"

namespace STK
{

/* @ingroup Base
 *  Convert a String to a Integer using a map.
 *  @param str the String we want to convert
 *  @param mapping the mapping between the string and the Integer
 *  @return the Integer represented by the String @c str. if the string
 *  does not match any known name, the @c unknown_ str is returned.
 **/
Integer stringToInt( String const& str, std::map<String, Integer> const& mapping)
{
  std::map<String, Integer>::const_iterator it=mapping.find(str);
  return (it == mapping.end()) ? Arithmetic<Integer>::NA() : it->second;
}

/* @ingroup Base
 *  Convert an Integer to a String.
 *  @param value the Integer we want to convert
 *  @param mapping the mapping between the Integer and the String
 *  @return the string associated to this Integer.
 **/
String intToString( Integer const& value, std::map<Integer, String> const& mapping)
{
  std::map<Integer, String>::const_iterator it=mapping.find(value);
  if (it == mapping.end())  return stringNa;
  return it->second;
}


} // namespace STK
