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
 * Project:  stkpp::stkernel
 * created on: 14 sept. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Constants.h
 *  @brief In this file we define the main constant which will be used through
 *  the STK++ project.
 **/

#ifndef STK_CONSTANTS_H
#define STK_CONSTANTS_H

#include "STK_String.h"

#ifndef STKBASEARRAYS
/** This value means that the default range for a vector or the rows/columns of
 *  a matrix is the value given by this constant.
 **/
#define STKBASEARRAYS 0
#endif

namespace STK
{
/** @ingroup STKernel
 *  This value represents the base index of the containers created in STK++.
 **/
const int baseIdx = STKBASEARRAYS;

/** @ingroup STKernel
 *  This value means that an integer is not known at compile-time, and that
 *  instead the value is stored in some runtime variable. This is the same value
 *  that the value used for representing NA Integers when Integer is int.
 **/
const int UnknownSize = INT_MAX;
//const int UnknownSize = STK::Arithmetic<int>::NA();

/** @ingroup STKernel
 * This value means that when we unroll loops we go until MaxUnroll */
const int MaxUnroll = 100;

/** @ingroup STKernel
 * This value means that when we unroll loops we go until MaxUnrollSquareRoot */
const int MaxUnrollSquareRoot = 10;

/** @ingroup STKernel
 * @brief Representation of a New Line String. */
static const String STRING_NL      = _T("\n");
/** @ingroup STKernel
 * @brief  Representation of a blank value. */
static const String STRING_BLANK  = _T(" ");

} // namespace STK


#endif /* STK_CONSTANTS_H */
