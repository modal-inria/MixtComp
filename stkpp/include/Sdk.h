/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  stkpp::Sdk
 * Purpose:  main include file for Sdk project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file Sdk.h
 *  @brief This file include all the other header files of the project Sdk.
 *
 * @defgroup Sdk Software Development Kit.
 * @brief The Sdk project propose a set of high level interfaces, template for
 * meta-programming and macros that are used throughout the STK++ projects.
 *
 * In Sdk we define the pure Interface classes than can be used throughout the
 * STK++ whole project. The aim is to unified the syntax and the treatment
 * realized by the statistical methods.
 **/

#ifndef SDK_H
#define SDK_H

/* Macros. */
#include "../projects/Sdk/include/STK_Macros.h"
/* Static Assert. */
#include "../projects/Sdk/include/STK_StaticAssert.h"
/* Templates. */
#include "../projects/Sdk/include/STK_MetaTemplate.h"

/* Interface for all classes using the curious recursive template paradigm.*/
#include "../projects/Sdk/include/STK_IRecursiveTemplate.h"

/* Interface for all runners classes */
#include "../projects/Sdk/include/STK_IRunner.h"

#endif  /* SDK_H */
