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
 * Purpose:  Declaration of functions around the gamma function
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_gamma.h
 *  @brief In this file we declare functions around the gamma
 *  function.
 **/

#ifndef STK_FUNCT_GAMMA_H
#define STK_FUNCT_GAMMA_H

#include <vector>

#include "STKernel/include/STK_Integer.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Funct
{
/** @ingroup Analysis
 *  @brief This function computes \f$ n! \f$ for int argument.
**/
Real factorial( int const& n);

/** @ingroup Analysis
 *  @brief This function computes \f$ z! \f$ when z is an integer in
 *  a Real format.
**/
Real factorial( Real const& z);

/** @ingroup Analysis
 *  @brief This function computes \f$ \ln(n!) \f$ for int argument.
 **/
Real factorialLn( int const& n);

/** @ingroup Analysis
 *  @brief This function computes \f$ \ln(z!) \f$ when z is an integer
 *  in a Real fromat.
 **/
Real factorialLn( Real const& z);

/** @ingroup Analysis
 *  @brief This function computes the function \f$ \Gamma(z) \f$.
**/
Real gamma( Real const& z);

/** @ingroup Analysis
 *  @brief This function computes the function \f$ \ln(\Gamma(z)) \f$.
**/
Real gammaLn( Real const& z);

/** @ingroup Analysis
 *  @brief Compute the error when we compute  \f$ \ln(\Gamma(z)) \f$
 *  using the Stirling's formula.
 **/
Real gammaLnStirlingError(Real const& z);

/** @ingroup Analysis
 *  @brief Compute the error when we compute  \f$ \ln(\Gamma(z)) \f$
 *  using the Stirling's formula and z is an int.
 **/
Real gammaLnStirlingError(int const& z);

/** @ingroup Analysis
 *  @brief This function computes the n first coefficients of the
 *  Stirling's serie.
 */
void stirlingCoefficients( std::vector<Real>& A);

} // namespace Funct

} // namespace STK

#endif // STK_FUNCT_GAMMA_H
