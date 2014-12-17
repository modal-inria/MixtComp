/*--------------------------------------------------------------------*/
/*  Copyright (C) 2004-2007  Serge Iovleff
    
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
 * Project:  Analysis
 * Purpose:  raw mathematical functions
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_raw.h
 *  @brief In this file we declare raw functions.
 * 
 *  raw functions are generic functions that can be used in various part
 *  of the STatistiK project. No test is done about the arguments.
 **/

#ifndef STK_FUNCT_RAW_H
#define STK_FUNCT_RAW_H

#include "STKernel/include/STK_Integer.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Funct
{

/** @ingroup Analysis
 *  @brief Compute the psi function psi(x).
 */
Real psi_raw(Real x);

/** @ingroup Analysis
 *  @brief Compute the error function erf(a)
 */
Real erf_raw(Real const& a);

/** @ingroup Analysis
 *  @brief Compute the complementary error function erfc(a)
 */
Real erfc_raw(Real const& a);

/** @ingroup Analysis
 *  @brief Compute the cumulative distribution function of
 *  the normal density
 */
Real normal_cdf_raw(Real const& x);

/** @ingroup Analysis
 *  @brief compute the probability distribution function of
 *  the normal density
 */
Real normal_pdf_raw(Real const& x);

} // namespace Funct

} // namespace STK

#endif // STK_FUNCT_RAW_H
