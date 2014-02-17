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

/** @file STK_Law_Exponential.h
 *  @brief In this file we implement the exponential law.
 **/

#ifndef STK_LAW_EXPONENTIAL_H
#define STK_LAW_EXPONENTIAL_H

#include "STK_Law_IUnivLaw.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 *  @brief class for the Exponential random generator.
 * 
 *  The Exponential distribution, is a continuous probability
 *  distribution with probability pdf function
 *  \f[
 *    f(x; \lambda) = \frac{1}{\lambda} e^{- x/\lambda} 1_{x\geq 0}
 *  \f]
 *  where \f$\lambda>0\f$ is the scale parameter.
**/
class Exponential : public IUnivLaw<Real>
{
  public:
    typedef IUnivLaw<Real> Base;
    /** constructor. */
    Exponential( Real const& scale=1);
    /** destructor. */
	  virtual ~Exponential();

    /** Generate a pseudo Exponential random variate. */
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
     *  F(t; \lambda)= 1- e^{- t/\lambda}
     * \f]
     *  @param t a real value
     **/
    virtual Real cdf( Real const& t) const;
    /** The inverse cumulative distribution function is
     * \f[
     * F^{-1}(p; \lambda) = - \lambda\log(1-p).
     * \f]
     *  @param p a probability
     **/
    virtual Real icdf( Real const& p) const;

    /** Generate a pseudo Exponential random variate with the specified
     *  parameter.
     *  @param scale the scale of the distribution
     **/
    static Real rand( Real const& scale=1);
    /** Give the value of the pdf at x.
     *  @param x a real value
     *  @param scale the scale of the distribution
     **/
    static Real pdf( Real const& x, Real const& scale=1);
    /** Give the value of the log-pdf at x.
     *  @param x a real value
     *  @param scale the scale of the distribution
     **/
    static Real lpdf( Real const& x, Real const& scale=1);

  protected:
    /** The scale parameter. */
    Real scale_;
};

} // namespace Law

} // namespace STK

#endif /*STK_LAW_EXPONENTIAL_H*/
