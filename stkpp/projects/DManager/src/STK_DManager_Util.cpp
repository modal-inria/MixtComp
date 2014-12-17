/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::DManager
 * created on: 12 oct. 2010
 * Purpose:  useful methods for processing strings and i/o streams..
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_DManager_Util.cpp
 *  @brief In this file we implement various methods useful for processing
 *  strings and i/o streams in the DManager project.
 **/
#include "../include/STK_DManager_Util.h"
#include "STKernel/include/STK_Functors.h"

#include <algorithm>

namespace STK
{

namespace DManager
{
/* @ingroup DManager
 *  convert a String to a TypeDataFile.
 *  @param type the String we want to convert
 *  @return the TypeDataFile represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
TypeDataFile stringToTypeDataFile( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("csv")))  return csv_;
  return unknown_;
}

/* @ingroup DManager
 *  convert a TypeDataFile to a String.
 *  @param type the type of data file we want to convert
 *  @return the string associated to this type.
 **/
String TypeDataFileToString( TypeDataFile const& type)
{
  if (type == csv_)  return String(_T("csv"));
  return String(_T("unknown"));
}


/*  @ingroup DManager
 *  @brief check if a string represent a boolean.
 *
 *  A String is a boolean if it is written "TRUE" or "FALSE". There is no need
 *  to use upper case.
 *  @param str the string to check
 *  @return @c true if the String i a boolean, @c false otherwise.
 **/
bool checkStringToBoolean( String const& str)
{
  // is it TRUE ?
  if (toUpperString(str) == _T("TRUE")) { return true;}
  // is it FALSE ?
  if (toUpperString(str) == _T("FALSE")) { return true;}
  // not a bolean string
  return false;
}

/* @ingroup DManager
 *  @brief check if a string represent a boolean.
 *
 *  A String is a boolean if it is written "TRUE" or "FALSE". There is no need
 *  to use upper case.
 *  @param str the string to check
 *  @return @c true if the String is a boolean, @c false otherwise.
 **/
bool StringToBoolean( String const& str)
{
  // is it TRUE ?
  if (toUpperString(str) == _T("TRUE")) { return true;}
  // if it's not true, it's false
  return false;
}


/* remove all occurrences of the char @c c at the beginning and the end
 *  of the string.
 */
void removeCharBeforeAndAfter( String & str, Char c )
{
  // erase first whitespaces
  str.erase(0, str.find_first_not_of(c));
  // erase remaining whitespaces
  size_t found =str.find_last_not_of(c);
  if (found != str.npos)
    str.erase(found+1);
  else
    str.clear(); // str is all whitespace
}

/* Get the current field from the stream.
 *  @param is the stream to treat
 *  @param delimiter the delimiter of the current field
 **/
istream& getField( istream& is, String& value, Char delimiter)
{
  std::getline( is, value, delimiter);
  removeCharBeforeAndAfter(value, CHAR_BLANK);
  removeCharBeforeAndAfter(value, CHAR_TAB);
  return is;
}

/* @ingroup DManager
 *  @return the number of line in an istream.
 *  @param is the stream to parse.
 **/
int nbEndOfLine( istream& is)
{
  char last = CHAR_BLANK;
  TestEndOfLineOp       test(&last);
  int nbLine = std::count_if( std::istreambuf_iterator<Char>( is )
                            , std::istreambuf_iterator<Char>(),
                              test
                            );
  if (last != CHAR_NL) nbLine++;
#ifdef STK_DEBUG
  stk_cout << _T("In nbEndOfLine, nbLine = ") << nbLine << _T("\n");
#endif
 return  nbLine;
}



} // namespace DManager

} // namespace STK


