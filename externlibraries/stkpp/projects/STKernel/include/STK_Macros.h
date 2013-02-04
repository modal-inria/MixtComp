
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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::STKernel::Base
 * created on: 23 août 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Macros.h
 *  @brief In this file we defien the macro and utilities function for emiting
 *  messages and errors.
 **/


#ifndef STK_MACROS_H
#define STK_MACROS_H

#include "STK_String_Util.h"
#include "STK_Exceptions.h"

/** @ingroup STKernel
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname()\nWhat: error message."
 **/
#define STKERROR_NO_ARG2(Where,Error) \
  String(_T("Error in "))+String(_T(#Where))+String(_T("(")) \
+ String(_T(")\nWhat: "))+String(_T(#Error))

#define STKERROR_NO_ARG(Where, Error) \
  STKERROR_NO_ARG2(Where,  Error)


/** @ingroup STKernel
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKERROR_1ARG2(Where, Arg, Error) \
  String(_T("Error in "))+String(_T(#Where))+String(_T("(")) \
+ STK::typeToString(Arg) \
+ String(_T(")\nWhat: "))+String(_T(#Error))

#define STKERROR_1ARG(Where, Arg, Error) \
  STKERROR_1ARG2(Where, Arg, Error)

/** @ingroup STKernel
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname(arg1, arg2)\nWhat: error message."
 **/
#define STKERROR_2ARG2(Where, Arg1, Arg2, Error) \
  String(_T("Error in "))+String(_T(#Where))+String(_T("(")) \
+ STK::typeToString(Arg1)+String(_T(","))+STK::typeToString(Arg2) \
+ String(_T(")\nWhat: "))+String(_T(#Error))
#define STKERROR_2ARG(Where, Arg1, Arg2, Error) \
  STKERROR_2ARG2(Where, Arg1, Arg2, Error)

/** @ingroup STKernel
 * generate an Error message with no argument. All Error message have the form :
 * "Error in classname::methodname(Arg1, Arg2, Arg3)\nWhat: error message."
 **/
#define STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error) \
  String(_T("Error in "))+String(_T(#Where))+String(_T("(")) \
+ STK::typeToString(Arg1)+String(_T(","))+STK::typeToString(Arg2)+String(_T(","))+STK::typeToString(Arg3)\
+ String(_T(")\nWhat: "))+String(_T(#Error))

#define STKERROR_3ARG(Where, Arg1, Arg2, Arg3, Error) \
  STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error)

/** @ingroup STkernel
 *  throw an out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKOUT_OF_RANGE_1ARG2(Where, Arg, Error) \
  throw out_of_range(STKERROR_1ARG2(Where, Arg, Error))
#define STKOUT_OF_RANGE_1ARG(Where, Arg, Error) \
    STKOUT_OF_RANGE_1ARG2(Where, Arg, Error)

/**  @ingroup STkernel
 * throw an out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKOUT_OF_RANGE_2ARG2(Where, Arg1, Arg2, Error) \
  throw out_of_range(STKERROR_2ARG2(Where, Arg1, Arg2, Error))
#define STKOUT_OF_RANGE_2ARG(Where, Arg1, Arg2, Error) \
    STKOUT_OF_RANGE_2ARG2(Where, Arg1, Arg2, Error)

/**  @ingroup STkernel
 * throw an out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKOUT_OF_RANGE_3ARG2(Where, Arg1, Arg2, Arg3, Error) \
  throw out_of_range(STKERROR_3ARG2(Where, Arg1, Arg2, Arg3, Error))
#define STKOUT_OF_RANGE_3ARG(Where, Arg1, Arg2, Arg3, Error) \
    STKOUT_OF_RANGE_3ARG2(Where, Arg1, Arg2, Arg3, Error)

/**  @ingroup STkernel
 * throw an out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKRUNTIME_ERROR_2ARG2(Where, Arg1, Arg2, Error) \
  throw runtime_error(STKERROR_2ARG2(Where, Arg1, Arg2, Error))
#define STKRUNTIME_ERROR_2ARG(Where, Arg1, Arg2, Error) \
    STKRUNTIME_ERROR_2ARG2(Where, Arg1, Arg2, Error)

/**  @ingroup STkernel
 * throw an out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKRUNTIME_ERROR_1ARG2(Where, Arg, Error) \
  throw runtime_error(STKERROR_1ARG2(Where, Arg, Error))
#define STKRUNTIME_ERROR_1ARG(Where, Arg, Error) \
    STKRUNTIME_ERROR_1ARG2(Where, Arg, Error)

/**  @ingroup STkernel
 * throw an out_of_range exception with one arguments. Error message have the form :
 * "Error in classname::methodname(arg)\nWhat: error message."
 **/
#define STKRUNTIME_ERROR_NO_ARG2(Where, Error) \
  throw runtime_error(STKERROR_NO_ARG2(Where, Error))
#define STKRUNTIME_ERROR_NO_ARG(Where, Error) \
    STKRUNTIME_ERROR_NO_ARG2(Where, Error)


#endif /* STK_MACROS_H */
