
/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::Sdk::Base
 * created on: 23 août 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Macros.h
 *  @brief In this file we define the macro and utilities function for
 *  errors messages and errors exceptions.
 **/


#ifndef STK_MACROS_H
#define STK_MACROS_H

#include "STKernel/include/STK_Exceptions.h"

/** @ingroup Sdk
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname()\nWhat: error message."
 **/
#define STKERROR_NO_ARG2(Where,Error) \
  STK::String(_T("Error in "))+STK::String(_T(#Where))+STK::String(_T("(")) \
+ STK::String(_T(")\nWhat: "))+STK::String(_T(#Error))

#define STKERROR_NO_ARG(Where, Error) \
  STKERROR_NO_ARG2(Where,  Error)

/** @ingroup Sdk
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKERROR_1ARG2(Where, Arg, Error) \
  STK::String(_T("Error in "))+STK::String(_T(#Where))+STK::String(_T("(")) \
+ STK::typeToString(Arg) \
+ STK::String(_T(")\nWhat: "))+STK::String(_T(#Error))

#define STKERROR_1ARG(Where, Arg, Error) \
  STKERROR_1ARG2(Where, Arg, Error)

/** @ingroup Sdk
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname(arg1, arg2)\nWhat: error message."
 **/
#define STKERROR_2ARG2(Where, Arg1, Arg2, Error) \
  STK::String(_T("Error in "))+STK::String(_T(#Where))+STK::String(_T("(")) \
+ STK::typeToString(Arg1)+STK::String(_T(","))+STK::typeToString(Arg2) \
+ STK::String(_T(")\nWhat: "))+STK::String(_T(#Error))
#define STKERROR_2ARG(Where, Arg1, Arg2, Error) \
  STKERROR_2ARG2(Where, Arg1, Arg2, Error)

/** @ingroup Sdk
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname(Arg1, Arg2, Arg3)\nWhat: error message."
 **/
#define STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error) \
  STK::String(_T("Error in "))+STK::String(_T(#Where))+STK::String(_T("(")) \
+ STK::typeToString(Arg1)+STK::String(_T(","))+STK::typeToString(Arg2)+STK::String(_T(","))+STK::typeToString(Arg3)\
+ STK::String(_T(")\nWhat: "))+STK::String(_T(#Error))

#define STKERROR_3ARG(Where, Arg1, Arg2, Arg3, Error) \
  STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error)

/** @ingroup Sdk
 *  throw an STK::out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKOUT_OF_RANGE_1ARG2(Where, Arg, Error) \
  throw STK::out_of_range(STKERROR_1ARG2(Where, Arg, Error))
#define STKOUT_OF_RANGE_1ARG(Where, Arg, Error) \
    STKOUT_OF_RANGE_1ARG2(Where, Arg, Error)

/**  @ingroup Sdk
 * throw an STK::out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKOUT_OF_RANGE_2ARG2(Where, Arg1, Arg2, Error) \
  throw STK::out_of_range(STKERROR_2ARG2(Where, Arg1, Arg2, Error))
#define STKOUT_OF_RANGE_2ARG(Where, Arg1, Arg2, Error) \
    STKOUT_OF_RANGE_2ARG2(Where, Arg1, Arg2, Error)

/**  @ingroup Sdk
 * throw an STK::out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKOUT_OF_RANGE_3ARG2(Where, Arg1, Arg2, Arg3, Error) \
  throw STK::out_of_range(STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error))
#define STKOUT_OF_RANGE_3ARG(Where, Arg1, Arg2, Arg3, Error) \
    STKOUT_OF_RANGE_3ARG2(Where, Arg1, Arg2, Arg3, Error)

/**  @ingroup Sdk
 * throw an runtime exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKRUNTIME_ERROR_2ARG2(Where, Arg1, Arg2, Error) \
  throw STK::runtime_error(STKERROR_2ARG2(Where, Arg1, Arg2, Error))
#define STKRUNTIME_ERROR_2ARG(Where, Arg1, Arg2, Error) \
    STKRUNTIME_ERROR_2ARG2(Where, Arg1, Arg2, Error)

/**  @ingroup Sdk
 * throw an runtime exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKRUNTIME_ERROR_1ARG2(Where, Arg, Error) \
  throw STK::runtime_error(STKERROR_1ARG2(Where, Arg, Error))
#define STKRUNTIME_ERROR_1ARG(Where, Arg, Error) \
    STKRUNTIME_ERROR_1ARG2(Where, Arg, Error)

/**  @ingroup Sdk
 * throw an runtime exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKRUNTIME_ERROR_NO_ARG2(Where, Error) \
  throw STK::runtime_error(STKERROR_NO_ARG2(Where, Error))
#define STKRUNTIME_ERROR_NO_ARG(Where, Error) \
    STKRUNTIME_ERROR_NO_ARG2(Where, Error)

/**  @ingroup Sdk
 * throw an domain exception with two arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKDOMAIN_ERROR_2ARG2(Where, Arg1, Arg2, Error) \
  throw STK::domain_error(STKERROR_2ARG2(Where, Arg1, Arg2, Error))
#define STKDOMAIN_ERROR_2ARG(Where, Arg1, Arg2, Error) \
    STKDOMAIN_ERROR_2ARG2(Where, Arg1, Arg2, Error)

/**  @ingroup Sdk
 * throw an domain exception with two arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKDOMAIN_ERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error) \
  throw STK::domain_error(STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error))
#define STKDOMAIN_ERROR_3ARG(Where, Arg1, Arg2, Arg3, Error) \
    STKDOMAIN_ERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error)

/**  @ingroup Sdk
 * throw an domain exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKDOMAIN_ERROR_1ARG2(Where, Arg, Error) \
  throw STK::domain_error(STKERROR_1ARG2(Where, Arg, Error))
#define STKDOMAIN_ERROR_1ARG(Where, Arg, Error) \
    STKDOMAIN_ERROR_1ARG2(Where, Arg, Error)

/**  @ingroup Sdk
 * throw an domain exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKDOMAIN_ERROR_NO_ARG2(Where, Error) \
  throw STK::domain_error(STKERROR_NO_ARG2(Where, Error))
#define STKDOMAIN_ERROR_NO_ARG(Where, Error) \
    STKDOMAIN_ERROR_NO_ARG2(Where, Error)

/**  @ingroup Sdk
 * throw an invalid argument exception with two arguments. Error message have the form :
 * "Error in classname::methodname(arg1, arg2)\nWhat: error message."
 **/
#define STKINVALIDARGUMENT_ERROR_2ARG2(Where, Arg1, Arg2, Error) \
  throw STK::invalid_argument(STKERROR_2ARG2(Where, Arg1, Arg2, Error))
#define STKINVALIDARGUMENT_ERROR_2ARG(Where, Arg1, Arg2, Error) \
    STKINVALIDARGUMENT_ERROR_2ARG2(Where, Arg1, Arg2, Error)

/**  @ingroup Sdk
 * throw an invalid argument exception with one argument. Error message have the form :
 * "Error in classname::methodname(arg1)\nWhat: error message."
 **/
#define STKINVALIDARGUMENT_ERROR_1ARG2(Where, Arg1, Error) \
  throw STK::invalid_argument(STKERROR_1RG2(Where, Arg1, Error))
#define STKINVALIDARGUMENT_ERROR_1ARG(Where, Arg1, Error) \
    STKINVALIDARGUMENT_ERROR_1ARG2(Where, Arg1, Error)

/**  @ingroup Sdk
 * throw an invalid argument exception with no argument. Error message have the form :
 * "Error in classname::methodname(arg1)\nWhat: error message."
 **/
#define STKINVALIDARGUMENT_ERROR_NO_ARG2(Where, Error) \
  throw STK::invalid_argument(STKERROR_NO_ARG2(Where, Error))
#define STKINVALIDARGUMENT_ERROR_NO_ARG(Where, Error) \
    STKINVALIDARGUMENT_ERROR_NO_ARG2(Where, Error)


#endif /* STK_MACROS_H */
