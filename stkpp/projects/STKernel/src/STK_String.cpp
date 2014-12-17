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
 * Project:  STKernel::Base
 * Purpose:  Define miscenaleous utility functions for Strings.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_String.cpp
 *  @brief In this file we implement miscellaneous utilities functions for Strings.
 **/

#include "../include/STK_String.h"

namespace STK
{

/* @ingroup Base
  * @brief Set a new value to the na String representation and modify
  * stringNaSize accordingly.
  **/
void setStringNa(String const& na)
{ stringNa = na; stringNaSize = na.size();}


/* @ingroup Base
 *  @brief convert a character of the String to uppercase
 *
 *  @param s The String to convert
 **/
String& toUpperString( String& s)
{
  // iterate along the String
  for (String::iterator it = s.begin(); it != s.end(); ++it)
  { *it = std::toupper(*it);}
  // return upper cased string
 return s;
}

/* @ingroup Base
 *  @brief convert a character of the String to uppercase
 *
 *  @param s The String to convert
 **/
String toUpperString( String const& s)
{
  String res(s);
  toUpperString(res);
  return res;
}


} // namespace STK

