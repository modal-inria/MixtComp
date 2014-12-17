
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
 * Project:  stkpp::DManager
 * created on: 18 juil. 2011
 * Purpose:  Utilites constants and method when importing data.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Import_Util.h
 *  @brief In this file we define the main constant and method needed when
 *  importing data.
 **/

#ifndef STK_IMPORT_UTIL_H
#define STK_IMPORT_UTIL_H

#include "STKernel/include/STK_String.h"

namespace STK
{

namespace Import
{
  /** @ingroup DManager
   *  define the type of import we want to perform. */
  enum TypeImport
  {
    /// try to convert the columns in numeric values and let the others as string
    unknown_ =0,
    /// try to convert the columns in numeric values and let the others as string
    numeric_,
    /// conserve only the columns with numeric values
    only_numeric_,
    /// copy the columns
    string_,
    /// convert using an array of conversion.
    directed_,
    /// try to convert with the most appropriate format the columns. NOT implemented
    intelligent_
  };

  /** @ingroup DManager
   *  Convert a String to a TypeImport.
   *  @param type the String we want to convert
   *  @return the TypeImport represented by the String @c type. if the string
   *  does not match any known name, the @c unknown_ type is returned.
   **/
  TypeImport stringToTypeImport( String const& type);

  /** convert a TypeImport to a String.
   *  @param type the type of import we want to use
   *  @return the string associated to this type.
   **/
  String typeImportToString( TypeImport const& type);
}

}

#endif /* STK_IMPORT_UTIL_H */
