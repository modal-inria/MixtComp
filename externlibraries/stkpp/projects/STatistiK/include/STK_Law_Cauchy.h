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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  Laws
 * Purpose:  Cauchy probability distribution.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Law_Cauchy.h
 *  @brief In this file we define the Cauchy probability distribution.
 **/

#ifndef STK_LAW_CAUCHY_H
#define STK_LAW_CAUCHY_H

#include "STK_Law_IUnivLaw.h"

namespace STK
{

namespace Law
{
/**
 *  @ingroup Laws
 *  @brief Cauchy distribution law.
 * 
 *  The Cauchy distribution, is a continuous probability
 *  distribution with pdf function
 *  \f[
 *    f(x; \mu, \gamma) =
 *    \frac{1}{\pi\gamma\left[1+\left(\frac{x-\mu}{\gamma}\right)^2\right]}
 *  \f]
 *  where \f$ \mu  \f$ is the location parameter, specifying the location
 *  of the peak of the distribution, and \f$ \gamma>0 \f$ is the scale
 *  parameter.
 **/
class Cauchy : public IUnivLaw<Real>
{
  protected:
    /** The location parameter. */
    Real location_;
    
    /** The scale parameter. */
    Real scale_;
    
  public:
    /** Constructor.
     *  @param location location of the Cauchy distribution
     *  @param scale scale of the Cauchy distribution
     **/
    Cauchy( Real const& location=0, Real const& scale=1);

    /** Destructor. **/
    virtual ~Cauchy();

    /** Generate a pseudo Cauchy random variate.
     * @return a cauchy random variable **/
    virtual Real rand() const;
    
    /** Generate a pseudo Cauchy random variate with the specified
     *  parameters.
     *  @param location location of the Cauchy distribution
     *  @param scale scale of the Cauchy distribution
     *  @return a cauchy random variable
     **/
    static Real rand( Real const& location, Real const& scale);
    
    /** Give the value of the pdf at x.
     *  @param x the value to evaluate the probability density function
     *  @return the pdf of the cauchy distribution at x
     **/
    virtual Real pdf( Real const& x) const;
    
    /**  Give the value of the log-pdf at x.
     *  @param x the value to evaluate the log-probability density function
     *  @return the log-pdf of the cauchy distribution at x
     **/
    virtual Real lpdf( Real const& x) const;

    /** The cumulative distribution function at t is
     * \f[
     *  F(t; \mu,\gamma)=
     * \frac{1}{\pi} \arctan\left(\frac{t-\mu}{\gamma}\right)+\frac{1}{2}
     * \f]
     * @see http://www.faqs.org/faqs/fr/maths/maths-faq-3/
     * for the computation of arctan.
     *
     *  @param t the value to evaluate the cumulative density function
     *  @return the cdf of the cauchy distribution at t
     **/
    virtual Real cdf( Real const& t) const;
    
    /** The inverse cumulative distribution function at p is
     * \f[
     * F^{-1}(p; \mu,\gamma) = \mu + \gamma \tan(\pi (p-1/2)).
     * \f]
     * 
     *  @param p the probability to evaluate the inverse cumulative
     *  density function
     *  @return the inverse cdf of the cauchy distribution at p
     **/
    virtual Real icdf( Real const& p) const;
};

} // namespace Law

} // namespace STK

#endif /*STK_LAWCAUCHY_H*/
