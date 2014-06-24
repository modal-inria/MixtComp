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
 * Purpose:  Declaration of functions around the beta function ratio
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_betaRatio.h
 *  @brief In this file we declare functions around the beta
 *  function ratio.
 **/

#ifndef STK_FUNCT_BETARATIO_H
#define STK_FUNCT_BETARATIO_H

#include "STKernel/include/STK_Integer.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Funct
{
/** @ingroup Analysis
 *  @brief Compute the incomplete beta function ratio using the continued
 *  fraction method.
 **/
Real betaRatio_cf( Real const& a, Real const& b, Real x, bool xm1
                 , bool lower_tail = true
                 );
/** @ingroup Analysis
 *  @brief Compute the incomplete beta function ratio I_x(a,b)
 *  using the serie expansion method.
 **/
Real betaRatio_se( Real const& a, Real const& b, Real const& x
                 , bool xm1, bool lower_tail
                 );

/** @ingroup Analysis
 *  @brief Compute the incomplete beta function ratio I_x(a,b)
 *  using the asymptotic expansion method.
 **/
Real betaRatio_ae( Real const& a, Real const& b, Real const& x
                 , bool xm1, bool lower_tail
                 );
/** @ingroup Analysis
 *  @brief Compute the incomplete beta function ratio
 **/
Real betaRatio( Real const& a, Real const& b, Real const& x, bool lower_tail = true);

} // namespace Funct

} // namespace STK
	
#endif /*STK_FUNCT_BETARATIO_H*/
