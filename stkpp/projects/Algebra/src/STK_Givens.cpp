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
 * Project:  Algebra
 * Purpose:  Implement Givens methods for Algebra classes.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Givens.cpp
 *  @brief This file implement the Givens methods used by the Algebra
 *  classes.
 **/

#include "../include/STK_Givens.h"
#include "STKernel/include/STK_Misc.h"

namespace STK
{
/*
 * Compute the Givens rotation in order to eliminate the coefficient z.
 **/
Real compGivens( Real const& y, Real const& z, Real& cosinus, Real& sinus)
{
  // trivial case
  if (z == 0)
  { 
    sinus   = 0.0;
    cosinus = 1.0;
    return y;
  }
  // compute Givens rotation avoiding underflow and overflow
  if (std::abs(z) > std::abs(y))
  { 
    Real aux = y/z;
    Real t   = sign(z, sqrt(1.0+aux*aux)); 
    sinus    = 1.0/t;
    cosinus  = sinus * aux;
    return t*z;
  }        
  else
  {
    Real aux = z/y;
    Real t   = sign(y, sqrt(1.0+aux*aux));
    cosinus  = 1.0/t;
    sinus    = cosinus * aux;
    return t*y;
  }
}

} // namespace STK
