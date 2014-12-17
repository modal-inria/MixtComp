/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * Project:  stkpp::reduct
 * created on: 11 juil. 2011
 * Purpose:  implement the utilities for the project reduct.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Reduct_Util.cpp
 *  @brief In this file we implement the utilities method of the project reduct.
 **/

#include "../include/STK_Reduct_Util.h"
#include "STKernel/include/STK_String.h"

namespace STK
{

namespace Reduct
{

/* convert a String to a TypeReduction.
 *  @param type the type of reduction we want to define
 *  @return the TypeReduction represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
TypeReduction stringToTypeReduction( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("totalVariance")))  return totalVariance_;
  if (toUpperString(type) == toUpperString(_T("localVariance"))) return localVariance_;
  if (toUpperString(type) == toUpperString(_T("mds"))) return mds_;
  return unknown_;
}

/* convert a TypeReduction to a String.
 *  @param type the type of reduction we want to convert
 *  @return the string associated to this type.
 **/
String typeReductionToString( TypeReduction const& type)
{
  if (type == totalVariance_)  return String(_T("totalVariance"));
  if (type == localVariance_) return String(_T("localVariance"));
  if (type == mds_) return String(_T("mds"));
  return String(_T("unknown"));
}

} // namespace Regress

} // namespace STK
