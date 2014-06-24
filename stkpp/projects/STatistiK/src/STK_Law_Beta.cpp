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

/** @file STK_Law_Bernoulli.cpp
 *  @brief In this file we implement the Beta class.
 **/

#include "../include/STK_Law_Beta.h"
#include "../include/STK_Law_Gamma.h"
#include "Analysis/include/STK_Funct_betaRatio.h"

namespace STK
{
namespace Law
{
/* Ctor
 */
Beta::Beta( const Real& alpha, const Real& beta)
          : IUnivLaw<Real>(String(_T("Beta")))
          , alpha_(alpha)
          , beta_(beta)
{
  // check parameters
  if (  Arithmetic<Real>::isNA(alpha) || Arithmetic<Real>::isNA(beta)
     || alpha <= 0.0 || beta <= 0.0
     )
  STKDOMAIN_ERROR_2ARG("Beta::Beta",alpha,beta,"argument error");
}

/* Dtor
 */
Beta::~Beta()
{}

/*  Generate a pseudo Beta random variate.
 */
Real Beta::rand() const
{
  // trivial case 
  if (!Arithmetic<Real>::isFinite(alpha_)) return 1.0;
  if (!Arithmetic<Real>::isFinite(beta_)) return 0.0;
  // general case
  Real g1 = Law::Gamma::rand(alpha_, 1.);
  return g1/(g1+Law::Gamma::rand(beta_, 1.));
}

/*
 *  Generate a pseudo Beta random variate with the specified parameters.
 *  (static)
 */
Real Beta::rand( const Real& a, const Real& b)
{
  // check parameters
  if (  Arithmetic<Real>::isNA(a)
     || Arithmetic<Real>::isNA(b)
     || a <= 0.0
     || b <= 0.0
     )
    STKDOMAIN_ERROR_2ARG("Beta::rand",a,b,"argument error");
  // trivial case 
  if (!Arithmetic<Real>::isFinite(a)) return 1.0;
  if (!Arithmetic<Real>::isFinite(b)) return 0.0;
  // general case
  Real g1 = Gamma::rand(a, 1.);
  return g1/(g1+Gamma::rand(b, 1.));
}

/*
 *  Give the value of the pdf at x.
 */
Real Beta::pdf( const Real& x) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(x)) return Arithmetic<Real>::NA();
  // trivial case
  if (Arithmetic<Real>::isInfinite(x)) return 0.0;
  // compute result
  return 0.;
}

/*
 * Give the value of the log-pdf at x.
 */
Real Beta::lpdf( const Real& x) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(x)) return Arithmetic<Real>::NA();
  // check parameter
  if (Arithmetic<Real>::isInfinite(x))
    return -Arithmetic<Real>::infinity();

  // compute result
  return 0.;
}

/*
 * The cumulative distribution function at t.
 */
Real Beta::cdf( const Real& t) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(t)) return Arithmetic<Real>::NA();
  // check parameter
  return (Arithmetic<Real>::isInfinite(t)) ? (t < 0.) ? 0.0 : 1.0
                                           :  Funct::betaRatio(alpha_,beta_,t, false);
}
    
/*
 * The inverse cumulative distribution function at p.
 */
Real Beta::icdf( const Real& p) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(p)) return Arithmetic<Real>::NA();

  // check parameter
  if ((p > 1.) || (p < 0.))
    throw domain_error("Beta::icdf(p) "
                       "argument error");
 // trivial cases
 if (p == 0.) return 0.;
 if (p == 1.) return 1.;

  // result 
  return 0.;
}

} // namespace Law

} // namespace STK
