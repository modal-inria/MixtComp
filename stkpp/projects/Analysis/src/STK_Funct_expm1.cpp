/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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
 * Purpose:  implementation of exp(x)-1
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/


#include <cmath>
// for int and Real
#include "STKernel/include/STK_Misc.h"

#include "../include/STK_Funct_util.h"
// for _LN2_
#include "../include/STK_Const_Math.h"

namespace STK
{
namespace Funct
{
/** @ingroup Analysis
 *  Compute the function
 *  \f[
 *     exp(x)-1.
 *  \f]
 *  with accuracy using it's first order Taylor serie when x is near 0.
 *  Improve the result using a Newton Raphson step.
 **/
Real expm1(Real const& x)
{
  // small values of x
  if (std::abs(x) < Const::_LN2_)
  {
    // a + 1 != 1 -> use compute Taylor serie else use first order
    // Taylor expansion
    if (std::abs(x) > Arithmetic<Real>::epsilon())
    {
      // Compute the taylor series S = x + (1/2!) x^2 + (1/3!) x^3 + ...
      Real term = x, sum =x, n =1.;
      do
      {
        sum += (term *= x/(++n));
      }
      while (std::abs(term) > std::abs(sum) * Arithmetic<Real>::epsilon()) ;
      
      return sum ;
    }
    else return (x / 2. + 1.) * x;
  }
  else return exp(double(x))-1.;
}

} // namespace Funct

} // namespace STK
