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
 * Purpose:  implementation of log(x+1)
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_log1p.cpp
 *  @brief In this file we implement the function \f$ \log(1+x) \f$
 **/

#include <cmath>

#include "STKernel/include/STK_Exceptions.h"

#include "../include/STK_Algo.h"
#include "../include/STK_ISerie.h"
#include "../include/STK_Funct_util.h"

namespace STK
{
namespace Funct
{
/** @ingroup Analysis
 *  This Serie computes
 *  \f[
 *    \frac{x^n}{n+2}
 *  \f]
 *  for n >= 0.
 */
class Serielog1p : public ISerie<Serielog1p>
{
  public:
    inline Serielog1p( Real const& x): x_(x)
                       , n_(2), xpown_(1.)
    { ;}

    inline Real firstImpl() const
    { return 1./n_;}

    inline Real nextImpl() const
    {
      // n
      ++n_;
      // x^n
      xpown_ *= x_;
      // x^n/(n+2)
      return xpown_/n_;
    }
  private:
    const   Real& x_;
    mutable Real n_;
    mutable Real xpown_;

};

/** @ingroup Analysis
 *  When |x| is less than 0.375 the function use a Taylor serie
 *  otherwise the default log function is used.
 **/
Real log1p(Real const& x)
{
  // trivial values
  if (x == 0.)  return 0.;
  if (x == -1.) return(-Arithmetic<Real>::infinity());
  // check domain
  if (x  < -1)
     throw domain_error("Funct::log1p(x) "
                        "Negative Real argument");
  // small values
  if (std::abs(x) < .375)
  {
    // create functor and compute the alternate serie
    Serielog1p f(x);
    return x * (1. - x * sumAlternateSerie(f));
  }
  // large values : compute directly the result
  return log(1. + x);
}

} // namespace Funct

} // namespace STK
