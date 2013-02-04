/*--------------------------------------------------------------------*/
/*     Copyright (C) 2007  Serge Iovleff

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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  Base
 * Purpose:  Define the Runtime Type Identification (RTTI) classes.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_IdTypeImpl.h
 *  @brief In this file we define the Runtime Type Identification (RTTI)
 *  classes.
 **/

#ifndef STK_RTTI_H
#define STK_RTTI_H

namespace STK
{
/** @ingroup Base
 *  @defgroup RTTI Runtime Type Identification.
 *  
 *  These classes allow to handle the Runtime type identification (RTTI)
 *  problem and are useful when working with heterogeneous data.
 **/

/** @ingroup RTTI 
 *  @brief Id for the Type of a variable.
 * 
 *  IdType is an enum for the type used in the statistical variables :
 *  - Char for letter variables
 *  - String for string (id) variables.
 *  - Real for quantitative variables,
 *  - int for qualitative variables,
 *  - Binary for binary (0,1) variables,
 *  - Sign for sign (-1, 1) variables,
 *  - NotAvailable for not available variables
 **/
enum IdType
{
  unknown_ =0,  ///< Unknown type
  character,   ///< character type
  real_,        ///< Quantitative type
  integer_,    ///< Qualitative type
  binary_,      ///< Binary type {0, 1}
  signof,       ///< Sign type {-1, 1}
  string,      ///< String type
  notavailable ///< Missing type
};

/** @ingroup RTTI
 *  @brief Implementation of the return type.
 * 
 *  This object will be specialized for runtime type identification
 *  (RTTI). Default is unknown_ type.
 **/
template<typename TYPE>
struct IdTypeImpl
{
  /** Give the IdType of the variable.
   * @return the unknown_ type
   **/
  static IdType returnType()
  { return(unknown_);}
};

} // namespace STK

#endif /* STK_RTTI_H */
