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

/** @file STK_Law_Uniform.h
 *  @brief In this file we implement the uniform law.
 **/

#ifndef STK_LAW_UNIFORM_H
#define STK_LAW_UNIFORM_H

#include "STK_Law_IUnivLaw.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 *  @brief class for the Uniform law distribution.
 * 
 *  The Uniform distribution, is a continuous probability
 *  distribution with probability pdf function
 *  \f[
 *    f(x; a, b) = \frac{1}{b-a} 1_{ a \leq x \leq b}.
 *  \f]
**/
class Uniform : public IUnivLaw<Real>
{
  public:
    typedef IUnivLaw<Real> Base;
    /** constructor.
     *  @param a the lower bound
     *  @param b the upper bound
     **/
    Uniform( Real a =0., Real b =1.);
    /** copy constructor.
     *  @param law the law to copy
     **/
    Uniform( Uniform const& law);
    /** destructor. */
	  virtual ~Uniform();

    /** Generate a pseudo Uniform random variate. */
    virtual Real rand() const;
    /** Give the value of the pdf at x.
     *  @param x a real value
     **/
    virtual Real pdf( Real const& x) const;
    /** Give the value of the log-pdf at x.
     *  @param x a real value
     **/
    virtual Real lpdf( Real const& x) const;
    /** The cumulative distribution function is
     * \f[
     *  F(t; a,b)= \frac{t - a}{b-a}
     * \f]
     *  @param t a real value
     **/
    virtual Real cdf( Real const& t) const;
    /** The inverse cumulative distribution function is
     * \f[
     * F^{-1}(p; \lambda) = p (b-a) + a.
     * \f]
     *  @param p a probability
     **/
    virtual Real icdf( Real const& p) const;

    /** Generate a pseudo Uniform random variate with the specified
     *  parameter.
     *  @param a the lower bound
     *  @param b the upper bound
     **/
    static Real rand( Real a, Real b);
    /** Give the value of the pdf at x.
     *  @param x a real value
     *  @param a the lower bound
     *  @param b the upper bound
     **/
    static Real pdf( Real const& x, Real a=0., Real b=1.);
    /** Give the value of the log-pdf at x.
     *  @param p a probablility
     *  @param a the lower bound
     *  @param b the upper bound
     **/
    static Real lpdf( Real const& p, Real a=0., Real b=1.);

  protected:
    /** The lower bound. */
    Real a_;
    /** The upper bound. */
    Real b_;
  private:
    Real range_;
};

} // namespace Law

} // namespace STK

#endif /*STK_LAW_UNIFORM_H*/
