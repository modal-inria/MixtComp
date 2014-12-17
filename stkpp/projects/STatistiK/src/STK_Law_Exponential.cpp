/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::STatistiK::Law
 * created on: 23 janv. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_Exponential.cpp
 *  @brief In this file we implement the exponential law.
 **/

#include "../include/STK_Law_Exponential.h"
#include "../include/STK_Law_Util.h"

namespace STK
{

namespace Law
{
/* Ctor
 */
Exponential::Exponential( Real const& scale)
                        : Base(String(_T("Exponential")))
                        , scale_(scale)
{
  // check parameters
  if ( !Arithmetic<Real>::isFinite(scale) || scale <= 0 )
    STKDOMAIN_ERROR_1ARG(Exponential::Exponential,scale,invalid argument);
}

/* Dtor
 */
Exponential::~Exponential() {}

/*
 *  Generate a pseudo Exponential random variate.
 */
Real Exponential::rand() const { return Law::generator.randExp() * scale_;}

/*
 *  Generate a pseudo Exponential random variate with the specified parameters.
 *  (static)
 */
Real Exponential::rand( Real const& scale)
{
#ifdef STK_DEBUG
  // check parameters
  if ( !Arithmetic<Real>::isFinite(scale) || scale <= 0 )
    STKDOMAIN_ERROR_1ARG(Exponential::pdf,scale,invalid argument);
#endif
  // check parameters
  if ( scale <= 0 )
    STKDOMAIN_ERROR_1ARG(Exponential::rand,scale,invalid argument);
  return generator.randExp() * scale;
}

/*
 *  Give the value of the pdf at x.
 */
Real Exponential::pdf( Real const& x) const
{
  // NA value
  if (Arithmetic<Real>::isNA(x)) return Arithmetic<Real>::NA();
  // trivial cases
  if (x<0) return 0.0;
  if (Arithmetic<Real>::isInfinite(x)) return 0.0;
  // compute result
  return std::exp(-x/scale_) / scale_;
}

/*
 *  Give the value of the pdf at x.
 */
Real Exponential::pdf( Real const& x, Real const& scale)
{
#ifdef STK_DEBUG
  // check parameters
  if ( !Arithmetic<Real>::isFinite(scale) || scale <= 0 )
    STKDOMAIN_ERROR_1ARG(Exponential::pdf,scale,invalid argument);
#endif
  // NA value
  if (Arithmetic<Real>::isNA(x)) return Arithmetic<Real>::NA();
  // trivial cases
  if (x<0) return 0.0;
  if (Arithmetic<Real>::isInfinite(x)) return 0.0;
  // compute result
  return std::exp(-x/scale) / scale;
}

/*
 * Give the value of the log-pdf at x.
 */
Real Exponential::lpdf( Real const& x) const
{
  // NA value
  if (Arithmetic<Real>::isNA(x)) return Arithmetic<Real>::NA();
  // trivial cases
  if (x<0) return -Arithmetic<Real>::infinity();
  if (Arithmetic<Real>::isInfinite(x)) return -Arithmetic<Real>::infinity();
  // compute result
  return (-x / scale_) - log(scale_) ;
}

/*
 * Give the value of the log-pdf at x.
 */
Real Exponential::lpdf( Real const& x, Real const& scale)
{
#ifdef STK_DEBUG
  // check parameters
  if ( !Arithmetic<Real>::isFinite(scale) || scale <= 0 )
    STKDOMAIN_ERROR_1ARG(Exponential::lpdf,scale,invalid argument);
#endif
  // NA value
  if (Arithmetic<Real>::isNA(x)) return Arithmetic<Real>::NA();
  // trivial cases
  if (x<0) return -Arithmetic<Real>::infinity();
  if (Arithmetic<Real>::isInfinite(x)) return -Arithmetic<Real>::infinity();
  // compute result
  return (-x / scale) - log(scale) ;
}

/*
 * The cumulative distribution function at t.
 */
Real Exponential::cdf( Real const& t) const
{
  // NA value
  if (Arithmetic<Real>::isNA(t)) return Arithmetic<Real>::NA();
  // trivial cases
  if (t <= 0.) return 0.0;
  if (Arithmetic<Real>::isInfinite(t)) return 1.0; /* t= +inf */

  return 1.-exp(-t/scale_);
}
    
/*
 * The inverse cumulative distribution function at p.
 */
Real Exponential::icdf( Real const& p) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(p)) return Arithmetic<Real>::NA();

  // check parameter
  if ((p > 1.) || (p < 0.))
    STKDOMAIN_ERROR_1ARG(Exponential::icdf,p,invalid argument);
 // trivial cases
 if (p == 0.)  return 0.0;
 if (p == 1.)  return Arithmetic<Real>::infinity();
  // result 
  return (- scale_ * log(1.-p));
}

} // namespace Law

} // namespace STK
