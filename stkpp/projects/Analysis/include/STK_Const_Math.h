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
 * Project:  stkpp::STatistiK::Analysis
 * Purpose:  Main mathematical constants
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Const_Math.h
 *  @brief In this file we give the main mathematical constants.
 * 
 *  Many of them have been obtained using the infinite precision
 *  calculator bc.
 **/

#ifndef STK_CONST_MATH_H
#define STK_CONST_MATH_H

#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Const
{
/// e
static const Real _E_          = 2.718281828459045235360287471353;
/// pi
static const Real _PI_   = 3.141592653589793238462643383279502884197169399375;
 /// gamma
static const Real _EULER_  = 0.57721566490153286060651209008240243104215933594;
/// 2*pi
static const Real _2PI_        = 6.283185307179586476925286766559;
/// pi/2
static const Real _PI_2_       = 1.570796326794896619231321691640;
/// pi/4
static const Real _PI_4_       = 0.785398163397448309615660845820;
/// 1/pi
static const Real _1_PI     = 0.318309886183790671537767526745;
/// 2/pi
static const Real _2_PI_     = 0.636619772367581343075535053490;
/// log10(2)
static const Real _LOG2_       = 0.301029995663981195213738894724;
/// log10(e)
static const Real _LOGE_       = 0.434294481903251827651128918917;
/// ln(2)
static const Real _LN2_        = 0.693147180559945309417232121458;
/// ln(10)
static const Real _LN10_       = 2.302585092994045684017991454684;
/// sqrt(2)
static const Real _SQRT2_      = 1.414213562373095048801688724210;
/// sqrt(3)
static const Real _SQRT3_      = 1.732050807568877293527446341506;
/// sqrt(32)
static const Real _SQRT32_     = 5.656854249492380195206754896838;
/// 1/sqrt(2)
static const Real _1_SQRT2_  = 0.707106781186547524400844362105;
/// sqrt(pi)
static const Real _SQRTPI_     = 1.772453850905516027298167483341;
/// sqrt(2/pi)
static const Real _SQRT2_PI_   = 0.797884560802865355879892119869;
/// sqrt(2pi)
static const Real _SQRT2PI_    = 2.506628274631000502415765284811045253007;
/// 1/sqrt(pi)
static const Real _1_SQRTPI_ = 0.564189583547756286948079451561;
/// 2/sqrt(pi)
static const Real _2_SQRTPI_ = 1.128379167095512573896158903122;
/// 1/sqrt(2pi)
static const Real _1_SQRT2PI_= 0.398942280401432677939946059934;
/// ln(sqrt(pi))
static const Real _LNSQRTPI_   = 0.572364942924700087071713675677;
/// ln(sqrt(2*pi))
static const Real _LNSQRT2PI_ = 0.918938533204672741780329736406;
/// ln(sqrt(pi/2))
static const Real _LNSQRTPI_2_ = 0.225791352644727432363097614947;

/** @ingroup Analysis
 *  @brief compute pi
**/
Real pi();

/** @ingroup Analysis
 *  Compute \f$ \pi/2 \f$ using pi()
 **/
inline Real pi_2() { return pi()/2.0;}

/** @ingroup Analysis
 *  @brief Compute the Euler constant
**/
Real euler();

} // namespace Const

} // namespace STK

#endif // STK_CONSTMATH_H
