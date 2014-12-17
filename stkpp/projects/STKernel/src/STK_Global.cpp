/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * created on: 23 juin 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Global.cpp
 *  @brief In this file we instanciate the global variables of the STK++ library.
 **/
#include "../include/STK_String.h"

namespace STK
{
/* @ingroup Base
  * @brief Representation of a Not Available value.
  *
  * By default we represent a Not Available value of any type as a "." (like in
  * (SAS(R))) for the end-user. This value can be overloaded at runtime.
  * @note if the value is modified at runtime, the value of @c stringNaSize
  * have to be modified accordingly. It is safer to use the @ref setStringNa
  * function.
  **/
String stringNa  = String(_T("."));

/* @ingroup Base
  * @brief Size (in number of Char) of a Not Available value.
  * We represent a Not Available value of any type as a "." (like in
  * (SAS(R))) for the end-user.
  **/
int stringNaSize  = 1;

}


