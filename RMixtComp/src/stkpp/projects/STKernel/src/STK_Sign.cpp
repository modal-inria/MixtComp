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
 * Purpose:  Implement the fundamental type Sign.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Sign.cpp
 *  @brief In this file we implement the fundamental type Sign.
 **/

#include "../include/STK_Sign.h"

namespace STK
{
/* @ingroup stream
 *  @brief Overloading of the ostream << for the type Sign.
 *  @param os the output stream
 *  @param output the value to send to the stream
 **/
ostream& operator << (ostream& os, const Sign& output)
{ return Arithmetic<Sign>::isNA(output)
      ? (os << stringNa) : (os << static_cast<int>(output));
}

/*  Overloading of the istream >> for the type Sign.
 **/
istream& operator >> (istream& is, Sign& input)
{
  int buff;
  // failed to read a discrete value
  if ((is >> buff).fail()) return is;
  switch (buff)
  {
    case -1:
      input = negative_;
      break;
    case 1:
      input = positive_;
      break;
    default:
      input = signNA_;
      break;
  }
  return is;
}

/* @ingroup Base
 *  Convert a String to a Sign.
 *  @param type the String we want to convert
 *  @return the Sign represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Sign stringToSign( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("-1"))) return negative_;
  if (toUpperString(type) == toUpperString(_T("1"))) return positive_;
  return signNA_;
}

/* @ingroup Base
 *  Convert a String to a Sign using a map.
 *  @param type the String we want to convert
 *  @param mapping the mapping between the string and the Sign
 *  @return the Sign represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Sign stringToSign( String const& type, std::map<String, Sign> const& mapping)
{
  std::map<String, Sign>::const_iterator it=mapping.find(type);
  if (it == mapping.end())  return signNA_;
  return it->second;
}

/* @ingroup Base
 *  Convert a Sign to a String.
 *  @param type the type of Binary we want to convert
 *  @return the string associated to this type.
 **/
String signToString( Sign const& type)
{
  if (type == negative_)  return String(_T("-1"));
  if (type == positive_) return String(_T("1"));
  return stringNa;
}

/* @ingroup Base
 *  Convert a Sign to a String.
 *  @param type the type of Binary we want to convert
 *  @param mapping the mapping between the Binary and the String
 *  @return the string associated to this type.
 **/
String signToString( Sign const& type, std::map<Sign, String> mapping)
{ return mapping.find(type)->second;}


} // namespace STK

