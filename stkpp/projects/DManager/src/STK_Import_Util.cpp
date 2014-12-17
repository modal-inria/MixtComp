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
 * Project:  stkpp::
 * created on: 18 juil. 2011
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Import_Util.cpp
 *  @brief In this file we implement various methods useful for processing
 *  strings when importing data.
 **/
#include "../include/STK_Import_Util.h"
#include "STKernel/include/STK_String.h"

namespace STK
{

namespace Import
{

/* convert a String to a TypeRegression.
 *  @param type the type of regression we want to define
 *  @return the TypeRegression represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
TypeImport stringToTypeImport( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("unknown"))) return unknown_;
  if (toUpperString(type) == toUpperString(_T("numeric")))  return numeric_;
  if (toUpperString(type) == toUpperString(_T("only_numeric"))) return only_numeric_;
  if (toUpperString(type) == toUpperString(_T("string"))) return string_;
  if (toUpperString(type) == toUpperString(_T("directed"))) return directed_;
  if (toUpperString(type) == toUpperString(_T("intelligent"))) return intelligent_;
  return unknown_;
}

/* convert a TypeRegression to a String.
 *  @param type the type of regression we want to convert
 *  @return the string associated to this type.
 **/
String typeImportToString( TypeImport const& type)
{
  if (type == numeric_)  return String(_T("numeric"));
  if (type == only_numeric_) return String(_T("only_numeric"));
  if (type == string_) return String(_T("string"));
  if (type == directed_) return String(_T("directed"));
  if (type == intelligent_) return String(_T("intelligent"));
  return String(_T("unknown"));
}

} // namespace Import

} // namespace STK
