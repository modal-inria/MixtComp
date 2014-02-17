/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2008 Serge Iovleff

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
 * Project: stkpp::STatistiK::Law
 * Purpose: Normal probability distribution.
 * Author:  Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_Normal.h
 *  @brief In this file we define the Normal probability law class.
 **/

#ifndef STK_LAW_NORMAL_H
#define STK_LAW_NORMAL_H

#include "STK_Law_IUnivLaw.h"

namespace STK
{

namespace Law
{
class Normal;
/** A synonymous for the normal law */
typedef Normal Gaussian;

/**
 *  @ingroup Laws
 *  @brief Normal distribution law.
 * 
 *  The Normal distribution is a continuous probability distribution
 *  with pdf
 *  \f[
 * f(x;\mu,\sigma) = \frac{1}{\sigma\sqrt{2\pi}}
 *            \exp\left(-\frac{\left(x-\mu\right)^2}{2\sigma^2} \right)
 *  \f]
 * where \f$ \mu \f$ and \f$\sigma\f$ are the mean and the standard
 * deviation.
**/
class Normal : public IUnivLaw<Real>
{
  public:
    /** Constructor.
     *  @param mu mean of the Normal distribution
     *  @param sigma standard deviation of the Normal distribution
     **/
    Normal( Real const& mu=0., Real const& sigma=1.);
    /** Destructor. **/
    virtual ~Normal();
    /** @return mu */
    inline Real const& mu() const { return mu_;}
    /** @return mu */
    inline Real const& sigma() const { return sigma_;}
    /** @param mu the value to set to mu */
    inline void setMu( Real const& mu) { mu_ = mu;}
    /** @param sigma the value to set to sigma */
    inline void setSigma( Real const& sigma)
    {
      if (sigma<0) STKDOMAIN_ERROR_1ARG(Normal::setSigma,sigma,sigma must be >= 0);
       sigma_ = sigma;
    }
    /** @brief Generate a pseudo normalized Normal random variate.
     *
     *  Generate a pseudo normalized Normal random variate
     *  with location parameter @c mu_ and standard deviation @c sigma_.
     *  @return a pseudo normal random variate
     **/
    Real rand() const;
    /** @param x a real value
     *  @return the value of the normal pdf at @c x
     **/
    virtual Real pdf( Real const& x) const;
    /** @return Give the value of the log-pdf at x.
     *  @param x a real value
     **/
    virtual Real lpdf( Real const& x) const;
    /** @brief Compute the cumulative distribution function at t of
     *  the standard normal distribution.
     *
     *  @author  W. J. Cody
     *  @see     http://www.netlib.org/specfun/erf
     *
     *  This is the erfc() routine only, adapted by the
     *  transform cdf(u)=erfc(-u/sqrt(2))/2
     * 
     *  @param t a real value
     *  @return the cumulative distribution function value at t
     **/
    virtual Real cdf( Real const& t) const;
    
    /** @brief Compute the inverse cumulative distribution function at p
     *  of the standard normal distribution.
     *
     *  @author Peter J. Acklam <pjacklam@online.no>
     *  @see    http://home.online.no/~pjacklam/notes/invnorm/index.html
     *
     *  This function is based on the MATLAB code from the address above.
     * 
     *  @param p a probability number.
     *  @return the inverse cumulative distribution function value at p.
     **/
    virtual Real icdf( Real const& p) const;

    /** @brief Generate a pseudo Normal random variate.
     *
     *  Generate a pseudo Normal random variate with location @c mu and
     *  standard deviation @c sigma parameters.
     *  @param mu mean of the Normal distribution
     *  @param sigma standard deviation of the Normal distribution
     *  @return a pseudo normal random variate, centered in @c mu and with
     *  standard deviation @c sigma
     **/
    static Real rand( Real const& mu, Real const& sigma);
    /** @param x a real value
     *  @param mu mean of the normal law
     *  @param sigma standard deviation of the normal law
     *  @return the value of the normal pdf at @c x
     **/
    static Real pdf( Real const& x, Real const& mu, Real const& sigma);
    /** @return Give the value of the log-pdf at x.
     *  @param x a real value
     *  @param mu mean of the normal law
     *  @param sigma standard deviation of the normal law
     **/
    static Real lpdf( Real const& x, Real const& mu, Real const& sigma);

  protected:
    /** The mu parameter. **/
    Real mu_;
    /** The sigma parameter. **/
    Real sigma_;
};

} // namespace Law

} // namespace STK

#endif /*STK_LAW_NORMAL_H*/
