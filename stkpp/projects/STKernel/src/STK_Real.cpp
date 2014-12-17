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
 * Purpose:  Implement the fundamental type Real.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Real.cpp
 *  @brief In this file we implement the conversion functions for the type Real.
 **/

#include "../include/STK_Real.h"

namespace STK
{

/* @ingroup Base
 *  Convert a String to a Real.
 *  @param type the String we want to convert
 *  @return the Real represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Real stringToReal( String const& str)
{
  istringstream is(str);
  Real x;
  is >> Proxy<Real>(x);
  return x;
}

/* @ingroup Base
 *  Convert a String to a Real using a map.
 *  @param type the String we want to convert
 *  @param mapping the mapping between the string and the Real
 *  @return the Real represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Real stringToReal( String const& str, std::map<String, Real> const& mapping)
{
  std::map<String, Real>::const_iterator it=mapping.find(str);
  return (it == mapping.end()) ? Arithmetic<Real>::NA() : it->second;
}

/* @ingroup Base
 *  Convert a Real to a String.
 *  @param type the type of Real we want to convert
 *  @return the string associated to this type.
 **/
String realToString( Real const& value, std::ios_base& (*f)(std::ios_base&))
{
  if (Arithmetic<Real>::isNA(value)) return stringNa;
  ostringstream os;
  os << f << value;
  return os.str();
}

/* @ingroup Base
 *  Convert an Real to a String.
 *  @param type the type of Real we want to convert
 *  @param mapping the mapping between the Real and the String
 *  @return the string associated to this type.
 **/
String realToString( Real const& type, std::map<Real, String> mapping)
{
  std::map<Real, String>::const_iterator it=mapping.find(type);
  if (it == mapping.end())  return stringNa;
  return it->second;
}


} // namespace STK
