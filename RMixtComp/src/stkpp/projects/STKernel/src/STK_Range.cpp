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
#include "../include/STK_String_Util.h"

namespace STK
{

/** Read a Range in the form first:last (MATLAB-like form) from
 * an input stream. The input stream can also be a number (say n). In this case
 * the range will be 1:n. If the range cannot be read the method return the
 * default range 1:0 (Should be NA value, but NA value is not defined
 * for Range).
 * @param is the input stream
 * @param I the range to set
 **/
istream& operator>> (istream& is, Range& I)
{
  String num;
  is >> std::skipws;
  // get first number
  std::getline(is, num, _T(':'));
  // check if the istream is exhausted
  if (is.eof())
  {
    I.first_ = STKBASEARRAYS;
    if (!stringToType(I.last_, num)) I.last_ =STKBASEARRAYS-1;
    return is;
  }
  // otherwise we encounter a ":", thus skip the current char
  if (!stringToType(I.first_, num)) I.first_ =STKBASEARRAYS;
  is.peek();
  if ((is >> I.last_).fail())
  {
    I.first_ =STKBASEARRAYS; I.last_ =STKBASEARRAYS-1;
  }
  return is;
}


} // namespace STK
