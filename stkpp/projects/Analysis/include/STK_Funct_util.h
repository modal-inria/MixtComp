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
 * Purpose:  Usual mathematical functions
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_util.h
 *  @brief In this file we declare usual Real functions.
 **/

#ifndef STK_FUNCT_USUAL_H
#define STK_FUNCT_USUAL_H

// for Real and int
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Funct
{
/** @ingroup Analysis
 *  @brief compute the partial deviance \f$g_0(x) = x\log(x)+ 1 - x\f$.
 */
Real g0(Real const& x);

/** @ingroup Analysis
 *  @brief compute the partial deviance
 *  \f$ d_0(a,b) = a\log(a/b)+ b - a \f$.
 **/
Real dev0(Real const& a, Real const& b);

/** @ingroup Analysis
 *  @brief compute the function \f$ \log(1+x) \f$.
 *  @param x value to evaluate the function
 **/
Real log1p(Real const& x);

/** @ingroup Analysis
 *  @brief compute the function \f$ \exp(x)-1 \f$.
 **/
Real expm1(Real const& x);

/** @ingroup Analysis
 *  @brief Polynomial evaluator.
 *  Evaluate the quantity
 *  \f[
 *      P(x) = P[0] x^n  +  P[1] x^(n-1)  + \ldots  +  P[n]
 *  \f]
 *  @note Coefficients are stored in reverse order.
 *
 * In the interest of speed, there are no checks for out
 * of bounds arithmetic.
 *
 */
template<int N>
inline Real evalPolynomial( Real x, Real* P)
{ return P[N] +  x * evalPolynomial<N-1>(x,P);}
template<>
inline Real evalPolynomial<0>( Real x, Real* P)
{ return P[0];}

} // namespace Funct

} // namespace STK

#endif // STK_FUNCT_USUAL
