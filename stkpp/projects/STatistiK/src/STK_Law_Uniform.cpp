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

/** @file STK_Law_Uniform.cpp
 *  @brief In this file we implement the uniform law.
 **/

#include "../include/STK_Law_Uniform.h"
#include "../include/STK_Law_Util.h"

namespace STK
{

namespace Law
{

/* constructor. */
Uniform::Uniform( Real a, Real b) : Base(_T("Uniform")), a_(a), b_(b), range_(b_ - a_)
{
  if (range_ <= 0.)
    STKINVALIDARGUMENT_ERROR_2ARG(Uniform::Uniform, a_, b_,invalid parameters);
}
/* constructor. */
Uniform::Uniform( Uniform const& law) : Base(law), a_(law.a_), b_(law.b_), range_(law.range_)
{}
/* destructor. */
Uniform::~Uniform() {}
/* Generate a pseudo Uniform random variate. */
Real Uniform::rand() const
{
  return ((range_ <= 1.) || (a_ <=1.)) ? a_ + range_ * generator.randUnif()
                                       : a_ * (1. + generator.randUnif()*range_/a_) ;
}
/* Give the value of the pdf at x.
 *  @param x a real value
 **/
Real Uniform::pdf( Real const& x) const
{
  if (!Arithmetic<Real>::isFinite(x) ) return x;
  if ((x < a_)||(x > b_)) return 0.;
  return 1./range_;
}
/* Give the value of the log-pdf at x.
 *  @param x a real value
 **/
Real Uniform::lpdf( Real const& x) const
{
  if (!Arithmetic<Real>::isFinite(x) ) return x;
  if ((x < a_)||(x > b_)) return -Arithmetic<Real>::infinity();
  return -std::log(range_);
}
/* The cumulative distribution function is
 * \f[
 *  F(t; a,b)= \frac{t - a}{b-a}
 * \f]
 *  @param t a real value
 **/
Real Uniform::cdf( Real const& t) const
{
  if (!Arithmetic<Real>::isFinite(t) ) return t;
  if (t <= a_) return 0.;
  if (t >= b_) return 1.;
  return (b_ - t)/range_;
}

/* The inverse cumulative distribution function is
 * \f[
 * F^{-1}(p; \lambda) = p (b-a) + a.
 * \f]
 *  @param p a probability
 **/
Real Uniform::icdf( Real const& p) const
{
  // check parameter
  if ((p > 1.) || (p < 0.))
    STKDOMAIN_ERROR_1ARG(Exponential::icdf,p,invalid argument);

  if (!Arithmetic<Real>::isFinite(p) ) return p;
  if (p == 1.) return b_;
  if (p == 0.) return a_;
  return a_ + p * range_;
}

/* Generate a pseudo Uniform random variate with the specified
 *  parameter.
 *  @param scale the scale of the distribution
 **/
Real Uniform::rand( Real a, Real b)
{
  Real sampleVal;
  if (b-a <= 1.)
  {
    Real sampleUni = generator.randUnif();
    sampleVal = a + (b-a) * sampleUni;
  }
  else
  {
    Real sampleUni = generator.randDiscreteUnifReal();
    sampleVal = a + (b-a)* sampleUni / 4294967296.;
  }

  return sampleVal;
}
/* Give the value of the pdf at x.
 *  @param x a real value
 *  @param scale the scale of the distribution
 **/
Real Uniform::pdf( Real const& x, Real a, Real b)
{
  if (!Arithmetic<Real>::isFinite(x) ) return x;
  if ((x < a)||(x > b)) return 0.;
  return 1./(b-a);
}
/* Give the value of the log-pdf at x.
 *  @param x a real value
 *  @param scale the scale of the distribution
 **/
Real Uniform::lpdf( Real const& x, Real a, Real b)
{
  if (!Arithmetic<Real>::isFinite(x) ) return x;
  if ((x < a)||(x > b)) return -Arithmetic<Real>::infinity();
  return -std::log(b-a);
}

} // namespace Law

} // namespace STK
