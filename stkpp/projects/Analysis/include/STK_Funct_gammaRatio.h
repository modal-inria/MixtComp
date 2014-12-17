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
 * Purpose:  Declaration of functions around the gamma function rqtio
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_gammaRatio.h
 *  @brief In this file we declare functions around the gamma
 *  function ratio.
 **/

#ifndef STK_FUNCT_GAMMARATIO_H
#define STK_FUNCT_GAMMARATIO_H

#include "STKernel/include/STK_Integer.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Funct
{
/** @ingroup Analysis
 *  @brief Compute the incomplete gamma functions ratio.
 **/
Real gammaRatio(Real const& a, Real const& x, bool lower_tail);
 
/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio Q(a,x).
 **/
Real gammaRatioQ(Real const& a, Real const& x);
 
/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio P(a,x).
 **/
Real gammaRatioP(Real const& a, Real const& x);
 
} // namespace Funct

} // namespace STK
	
#endif /*STK_FUNCT_GAMMARATIO_H*/
