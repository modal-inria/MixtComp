/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2008  Serge Iovleff

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
 * Project: STatistiK
 * Purpose: Implementation of the Cauchy Distribution
 * Author:  Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_Gamma.cpp
 *  @brief In this file we implement the Gamma distribution.
 **/

#include "../include/STK_Law_Gamma.h"
#include "../include/STK_Law_Exponential.h"
#include "../include/STK_Law_Util.h"

#include "Analysis/include/STK_Funct_poisson_raw.h"
#include "Analysis/include/STK_Funct_gammaRatio.h"

namespace STK
{

namespace Law
{
/* Ctor
 */
Gamma::Gamma(Real const& shape, Real const& scale)
            : IUnivLaw<Real>(String(_T("Gamma")))
            , a_(shape), b_(scale)
{
  // check parameters
  if (  !Arithmetic<Real>::isFinite(a_) || !Arithmetic<Real>::isFinite(b_)
     || a_ <= 0 || b_ <= 0
     )
    STKDOMAIN_ERROR_2ARG(Gamma::Gamma,a_, b_,arguments not valid);
  computeCtes();
}
/** destructor */
Gamma::~Gamma() {}

/** @brief generate a gamma random variate using the G.S algorithm
 *  of Ahrens and Dieter (1974) for 0<a_<1
 *  and the Fishman (1976) algorithm for a_>1.
 **/
Real Gamma::rand() const
{
  // GS algorithm for parameter a_ < 1.
  if (a_ < 1.)
  {
    if(a_ == 0.) return 0.;
    Real x;
    do
    {
      Real p = c_ * generator.randUnif();
      if (p >= 1.)
      {
        x = -std::log((c_ - p) / a_);
        if (generator.randExp() >= (1. - a_) * std::log(x)) break;
      }
      else
      {
        x = std::exp(std::log(p) / a_);
        if (generator.randExp() >= x) break;
      }
    }
    while(1);
    return(x*b_);
  }
  // Exponential law for parameter a_ = 1
  if (a_ == 1.0) return b_*generator.randExp();
  // Marsaglia fast Gamma generator with enhanced squeeze step for a>1
  // initialize constants
  Real v;
  while(1)
  {
    Real x,u;
   // generate gaussian rv in the range (-1/c, infty)
    while( (v = 1. + c_* ( x = generator.randGauss() ) ) <= 0. );
    // cubic normal variate
    v = v*v*v;
    // squeeze step
    if ( (u = generator.randUnif()) < 1.-(x*x)*(x*x)/(108. * d_) ) break;
    // rejection step
    if ( std::log(u) < 0.5*x*x+d_*(1.-v+std::log(v)) ) break;
  }
  return b_*d_*v;
}

/**
 *  compute
 *  \f[
 *   f(x;\alpha,\beta) = \left(\frac{x}{\beta}\right)^{\alpha-1}
 *                   \frac{e^{-x/\beta}}{\beta \, \Gamma(\alpha)}
 *   \ \mathrm{for}\ x > 0 
 *  \f]
 *  where \f$ \alpha > 0 \f$ is the shape parameter
 *  and   \f$ \beta > 0 \f$ is the scale parameter.
 */
Real Gamma::pdf( Real const& x) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(x)) return x;
  // trivial cases
  if (x < 0.) return 0.;
  if (Arithmetic<Real>::isInfinite(x)) return 0.;
  if (x == 0.) return (a_<1) ? Arithmetic<Real>::infinity()
                             : (a_>1.) ?  0. : 1./b_;
  // general case
  return (a_ < 1) ? Funct::poisson_pdf_raw(a_, x/b_) * a_/x
                  : Funct::poisson_pdf_raw(a_-1, x/b_)/b_;
}

/*
 *  Give
 *  \f[
 *   \ln(f(x;k,\theta)) = - x/\theta + (k-1) \ln(x)
 *                      - k \ln(\theta) + \ln(\Gamma(k))
 *  \f]
 */
Real Gamma::lpdf( Real const& x) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(x)) return x;
  // trivial cases
  if (x < 0.) return -Arithmetic<Real>::infinity();
  if (Arithmetic<Real>::isInfinite(x)) return -Arithmetic<Real>::infinity();
  if (x == 0.) return (a_<1) ? Arithmetic<Real>::infinity()
                             : (a_>1.) ?  -Arithmetic<Real>::infinity() : -std::log(b_);
  // general case
  return (a_ < 1) ? Funct::poisson_lpdf_raw(a_, x/b_) +std::log(a_/x)
                  : Funct::poisson_lpdf_raw(a_-1, x/b_) - std::log(b_);
}

/*
 * The cumulative distribution function can be expressed in terms of
 * the incomplete gamma function
 * \f[
 * F(x;k,\theta) = \int_0^x f(u;k,\theta)\,du  
 * = \frac{\gamma(k, x/\theta)}{\Gamma(k)}.
 * \f]
 */
Real Gamma::cdf( Real const& t) const
{
  // check NA value
  if (Arithmetic<Real>::isNA(t)) return t;
  // trivial cases
  if (t <= 0.) return 0.;
  if (Arithmetic<Real>::isInfinite(t)) return 1.;
  // general case
  return Funct::gammaRatioP(a_, t/b_);
}
    
/*
 *  Give the quantile t such that the probability of a random variate
 *  is less to t is equal to p.
 */
Real Gamma::icdf( Real const& p) const
{
  STKRUNTIME_ERROR_1ARG(Gamma::icdf,p,not implemented);
  return 0.0;

}

Real Gamma::rand( Real const& a, Real const& b)
{
  // check parameters
  if (  !Arithmetic<Real>::isFinite(a) || !Arithmetic<Real>::isFinite(b)
     || a <= 0 || b <= 0
     )
    STKDOMAIN_ERROR_2ARG(Gamma::rand,a, b,arguments not valid);
  // GS algorithm for parameter a_ < 1.
  if (a < 1.)
  {
    Real c = 1. + a/Const::_E_;
    Real x;
    if(a == 0.) return 0.;
    do
    {
      Real p = c * generator.randUnif();
      if (p >= 1.)
      {
        x = -std::log((c - p) / a);
        if (generator.randExp() >= (1. - a) * std::log(x)) break;
      }
      else
      {
        x = std::exp(std::log(p) / a);
        if (generator.randExp() >= x) break;
      }
    } while(1);
    return b *x;
  }
  // special case
  if (a == 1.0) return Exponential::rand(b);
  // Marsaglia fast Gamma generator with enhanced squeeze step for alpha>1
  // initialize constants
  const Real d = a-1./3.;
  const Real c = 1./sqrt(9.*d);
  // rejection method : main loop
  Real v;
  while(1)
  {
    // auxiliary variables
    Real x,u;
    // generate gaussian rv in the range (-1/c, infty)
    while( (v = 1. + c * ( x = generator.randGauss() ) ) <= 0. ) {}
    // cubic normal variate
    v = v*v*v;
    // squeeze step
    if ( (u = generator.randUnif()) < 1.-(x*x)*(x*x)/(108. * d) ) break;
    // rejection step
    if ( std::log(double(u)) < 0.5*x*x+d*(1.-v+log(v)) ) break;
  }
  return (b*d*v);
}

/*
 *  Give
 *  \f[
 *   \ln(f(x;k,\theta)) = - x/\theta + (k-1) \ln(x)
 *                      - k \ln(\theta) + \ln(\Gamma(k))
 *  \f]
 */
Real Gamma::pdf( Real const& x, Real const& a, Real const& b)
{
  // check NA value
  if (Arithmetic<Real>::isNA(x)) return x;
  // trivial cases
  if (x < 0.) return 0.;
  if (Arithmetic<Real>::isInfinite(x)) return 0.;
  if (x == 0.) return (a<1) ? Arithmetic<Real>::infinity()
                             : (a>1.) ?  0. : 1./b;
  // general case
  return (a < 1) ? Funct::poisson_pdf_raw(a, x/b) * a/x
                  : Funct::poisson_pdf_raw(a-1, x/b)/b;
}

/*
 *  Give
 *  \f[
 *   \ln(f(x;k,\theta)) = - x/\theta + (k-1) \ln(x)
 *                      - k \ln(\theta) + \ln(\Gamma(k))
 *  \f]
 */
Real Gamma::lpdf( Real const& x, Real const& a, Real const& b)
{
  // check NA value
  if (Arithmetic<Real>::isNA(x)) return x;
  // trivial cases
  if (x < 0.) return -Arithmetic<Real>::infinity();
  if (Arithmetic<Real>::isInfinite(x)) return -Arithmetic<Real>::infinity();
  if (x == 0.) return (a<1) ? Arithmetic<Real>::infinity()
                             : (a>1.) ?  -Arithmetic<Real>::infinity() : -std::log(b);
  // general case
  return (a < 1) ? Funct::poisson_lpdf_raw(a, x/b) +std::log(a/x)
                  : Funct::poisson_lpdf_raw(a-1, x/b) - std::log(b);
}

void Gamma::computeCtes() const
{
  if (a_ < 1.)
  {
    c_ = 1. + a_/Const::_E_;
    d_ = c_ * generator.randUnif();
  }
  else
  {
    d_ = a_-1./3.;
    c_ = 1./std::sqrt(9.*d_);
  }
}

} // namespace Law

} // namespace STK
