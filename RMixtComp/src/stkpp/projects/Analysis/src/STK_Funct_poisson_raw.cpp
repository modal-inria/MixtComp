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
 * Project:  Analysis
 * Purpose:  implementation of the poisson function
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_poisson_raw.cpp
 *  @brief In this file we implement functions around the poisson 
 *  density.
 **/

#include <cmath>

#include "../include/STK_Const_Math.h"
#include "../include/STK_Funct_gamma.h"
#include "../include/STK_Funct_util.h"
#include "../include/STK_Funct_poisson_raw.h"

namespace STK
{
  
namespace Funct
{
/** @ingroup Analysis
 *  Compute the function:
 *  \f[
 *    p(x, \lambda) = e^{-\lambda} \lambda^x/\Gamma(x+1)
 *  \f]
 *  with good accuracy using the partial deviance.
 *  This is the version for x Real.
 * 
 *  @see http://www.herine.net/stat/software/dbinom.html
 * 
 *  @param x value to evaluate the function
 *  @param lambda value of the parameter
 **/
Real poisson_pdf_raw(Real const& x, Real const& lambda)
{
  // check trivial values
  if (x<0.) return( 0. );
  // if lambda is 0, we have P(X=0) = 1
  if (lambda==0.) return( (x==0.) ? 1. : 0. );
  // special value
  if (x==0.) return( Real(std::exp(-lambda)) );
  // stirling approximation and deviance
  return( std::exp(-gammaLnStirlingError(x)-dev0(x, lambda))/(Const::_SQRT2PI_*std::sqrt(x)));
}

/** @ingroup Analysis
 *  Compute the function:
 *  \f[
 *    p(x, \lambda) = e^{-\lambda} \lambda^x/\Gamma(x+1)
 *  \f]
 *  with good accuracy using the partial deviance.
 *  This is the version for x integer.
 * 
 *  @see http://www.herine.net/stat/software/dbinom.html
 * 
 *  @param x value to evaluate the function
 *  @param lambda value of the parameter
 **/
Real poisson_pdf_raw(int const& x, Real const& lambda)
{ return poisson_pdf_raw(Real(x), lambda);}

/** @ingroup Analysis
 *  Compute the function:
 *  \f[
 *    p(x, \lambda) = e^{-\lambda} \lambda^x/\Gamma(x+1)
 *  \f]
 *  with good accuracy using the partial deviance.
 *  This is the version for x Real.
 *
 *  @see http://www.herine.net/stat/software/dbinom.html
 *
 *  @param x value to evaluate the function
 *  @param lambda value of the parameter
 **/
Real poisson_lpdf_raw(Real const& x, Real const& lambda)
{
  // check trivial values
  if (x<0.) return( -Arithmetic<Real>::infinity() );
  // if lambda is 0, we have P(X=0) = 1
  if (lambda==0.) return( (x==0.) ? 0 : -Arithmetic<Real>::infinity() );
  // special value
  if (x==0.) return( -lambda );
  // stirling approximation and deviance
  return( -gammaLnStirlingError(x)-dev0(x, lambda)-Const::_LNSQRT2PI_-std::log(x)/2.);
}

/** @ingroup Analysis
 *  Compute the function:
 *  \f[
 *    p(x, \lambda) = e^{-\lambda} \lambda^x/\Gamma(x+1)
 *  \f]
 *  with good accuracy using the partial deviance.
 *  This is the version for x integer.
 *
 *  @see http://www.herine.net/stat/software/dbinom.html
 *
 *  @param x value to evaluate the function
 *  @param lambda value of the parameter
 **/
Real poisson_lpdf_raw(int const& x, Real const& lambda)
{ return poisson_lpdf_raw(Real(x), lambda);}

} // namespace Funct

} // namespace STK
