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
 * Project:  Base
 * Purpose:  Define miscenaleous utility functions.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Misc.h
 *  @brief In this file we define miscellaneous utility (templated)
 *  functions.
 **/

#ifndef STK_MISC_H
#define STK_MISC_H

#include <cmath> // for sqrt
#include <cstdlib> // for rand

#include "STK_Integer.h"
#include "STK_Real.h"

namespace STK
{
/** @ingroup Base
 *
 *  templated sign value sign(x) * y:
 *  Type should be an integral type
 *  @param x the sign value
 *  @param y the signed value to return
 **/
template<typename Type>
inline Type sign(Type const& x, Type const& y = Type(1))
{ return( (x<0) ? -y : y); }

/** @ingroup Base
 *
 *  frand() generate a Real uniform number. This is a very basic method
 *  and should only be used when speed is necessary only.
 **/
inline Real frand()
{ return (Real)rand() / (RAND_MAX+1.0);}

/** @ingroup Base
 *  @brief is x a odd number ?
 *
 *  This method return true if the rest of the euclidian division
 *  of x by 2 is 0.
 *  @param x the value to test
 *  @return @c true if x is odd, @c false otherwise
 **/
inline bool isOdd(int const& x)
{ return( (x%2) == 1 ); }

/** @ingroup Base
 *  @brief is x an even number ?
 *
 *  This method return true if the rest of the euclidian division
 *  of x by 2 is 1.
 *  @param x the value to test
 *  @return @c true if x is even, @c false otherwise
 **/
inline bool isEven(int const& x)
{ return( (x%2) == 0 ); }

/** @ingroup Base
 *
 *  Computation of sqrt(x^2 + y^2) without underflow or overflow.
 *  @param x first value
 *  @param y second value
 *  @return the value \f$ \sqrt(x^2 + y^2) \f$
 **/
inline Real norm(Real const& x, Real const& y)
{
  Real absx = std::abs(x), absy = std::abs(y);
  return (absx > absy) ?
    (absx * std::sqrt(Real(1.0+(absy/absx)*(absy/absx))))
  : (absy == 0.0 ? 0.0 : absy * std::sqrt(Real(1.0+(absx/absy)*(absx/absy))));
}

} // namespace STK

#endif /* STK_MISC_H */
