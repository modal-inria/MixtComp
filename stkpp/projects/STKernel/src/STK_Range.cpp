/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2009  Serge Iovleff

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
 * Purpose:  implement the Range class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Range.cpp
 *  @brief In this file we implement the class Range, a range of Index
 *  denoting a sub-vector region.
 **/

#include <algorithm>
#include "../include/STK_Range.h"
#include "../include/STK_Integer.h"

namespace STK
{

/* @brief Read a Range in the form first:last (MATLAB-like form) from
 *  an input stream. The input stream can also be a number (say n).
 *  In this case the range will be n:n. If the range cannot be read the
 *  method return a NA value
 * @param is the input stream
 * @param I the range to set
 **/
istream& operator>> (istream& is, Range& I)
{
  // get current file position
  std::ios::pos_type pos = is.tellg();

  String str;
  // get first number
  std::getline(is, str, _T(':'));
  if (!stringToType(I.begin_, str))
  {
    I = Arithmetic<Range>::NA();
    is.seekg(pos); is.setstate(std::ios::failbit);
    return is;
  }
  // check if the istream is exhausted
  if (is.eof())
  {
    I.end_ = I.begin_+1;
    return is;
  }
  // skip the current char ":"
  is.peek();
  int last;
  if ((is >> last).fail())
  {
    I = Arithmetic<Range>::NA();
    is.seekg(pos); is.setstate(std::ios::failbit);
    return is;
  }
  else { I.end_ =last+1;}
  // ok
  return is;
}

} // namespace STK
