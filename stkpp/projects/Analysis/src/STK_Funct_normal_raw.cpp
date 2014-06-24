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
 Cephes Math Library Release 2.2:  June, 1992
 Copyright 1984, 1987, 1988, 1992 by Stephen L. Moshier
 Direct inquiries to 30 Frost Street, Cambridge, MA 02140
 */

/*
 * Project:  Analysis
 * Purpose:  Implementation of the normal functions
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_normal_raw.cpp
 *  @brief In this file we implement the normal functions using the
 * cephes Math Library.
 **/
#include <cmath>
#include "STKernel/include/STK_Misc.h"

#include "../include/STK_Const_Math.h"
#include "../include/STK_Funct_raw.h"

namespace STK
{
namespace Funct
{

/** @ingroup Analysis
 *  compute the probability distribution function of
 *  the normal density
 *   \f[
 *   \mathrm{\phi}(x) = \frac{1}{\sqrt{2\pi}} e^{-x^2/2}
 *  \f]
 *  @param x value to evaluate the function
 **/
Real normal_pdf_raw(Real const& x)
{
  return Const::_1_SQRT2PI_ * exp(-0.5 * x * x);
}

/** @ingroup Analysis
 *  Compute the cumulative distribution function of
 *  the normal density
 *   \f[
 *   \mathrm{\Phi}(x)
 *         = \frac{1}{\sqrt{2\pi}} \int_{-\infty}^{x} e^{-t^2/2} dt
 *         =  ( 1 + \mathrm{erf}(z) ) / 2
 *         =  \mathrm{erfc}(z) / 2
 *  \f]
 * where \f$ z = x/sqrt(2) \f$. Computation use functions
 * @code erf @endcode and @code erfc @endcode.
 * 
 *  @param x value to evaluate the function
 **/
Real normal_cdf_raw(Real const& x)
{
  Real t = x * Const::_1_SQRT2_;
  Real z = std::abs(t);

  if ( z < Const::_1_SQRT2_) return 0.5 + 0.5 * erf(t);
  Real y = 0.5 * erfc(z);
  return ( t > 0) ? 1. - y : y;
}

} // namespace Funct
  
} // namespace STK
