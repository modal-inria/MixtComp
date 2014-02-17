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
 * Project:  stkpp::STatistiK::Law
 * Purpose:  Beta probability distribution.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_Beta.h
 *  @brief In this file we define the Beta probability distribution.
 **/

#ifndef STK_LAW_BETA_H
#define STK_LAW_BETA_H

#include "STK_Law_IUnivLaw.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 *  @brief Beta distribution law.
 * 
 *  The Beta distribution, is a continuous probability
 *  distribution with pdf
 *  \f[
 *  f(x) = \frac{x^{\alpha-1} (1-x)^{\beta-1} }
 *              {{\boldsymbol \beta}(\alpha,\beta)},
 *  \quad x \in [0,1]
 *  \f]
 *  where \f$ \alpha\f$ and \f$ \beta\f$ are parameters that must be
 *  greater than zero and \f${\boldsymbol \beta}(a,b) \f$ represents
 *  the beta function.
 *
 *  @sa STK::Funct::betaRatio
**/
class Beta : public IUnivLaw<Real>
{
  public:
    /** default constructor. */
    Beta( const Real& alpha = .5, const Real& beta = .5);
    /** Dtor. */
    virtual ~Beta();

    /** @return a pseudo Beta random variate.
     *  This function use the Gamma::rand() random generator.
     *  TODO : implement the order statistics when a and b
     *  are small integers (Devroye p. 431). 
     *  TODO : implement the rejection method from the normal
     *  pdf when a=b (Devroye p. 434).
     *  TODO : Implement Cheng's algorithm BA for beta pdf 
     *  (Devroye p. 438)
     **/
    virtual Real rand() const;
    
    /** @return a pseudo Beta random variate with the specified
     *  parameters.
     *  This function use the Gamma::rand() random generator.
     *  TODO : implement the order statistics when a and b
     *  are small integers (Devroye p. 431). 
     *  TODO : implement the rejection method from the normal
     *  pdf when a=b (Devroye p. 434).
     *  TODO : Implement Cheng's algorithm BA for beta pdf 
     *  (Devroye p. 438)
     **/
    static Real rand( const Real& alpha, const Real& beta);
    
    /** @return the value of the pdf at x.
     **/
    virtual Real pdf( const Real& x) const;
    
    /** @return the value of the log-pdf at x.
     **/
    virtual Real lpdf( const Real& x) const;

    /** @return The cumulative distribution function
     **/
    virtual Real cdf( const Real& t) const;
    
    /** @return The inverse cumulative distribution */
    virtual Real icdf( const Real& p) const;

  protected:
    /** First parameter. */
    Real alpha_;
    /** Second parameter. */
    Real beta_;
};

} // namespace Law

} // namespace STK

#endif /*STK_LAWBETA_H*/
