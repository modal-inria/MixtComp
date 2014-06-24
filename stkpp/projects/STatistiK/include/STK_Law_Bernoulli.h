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

/** @file STK_Law_Bernoulli.h
 *  @brief In this file we define the Bernoulli distribution.
 **/

#ifndef STK_LAW_BERNOULLI_H
#define STK_LAW_BERNOULLI_H

#include "STK_Law_IUnivLaw.h"
#include "STKernel/include/STK_Binary.h"

namespace STK
{

namespace Law
{

/** @ingroup Laws
 * @brief Implement the Bernoulli probability law.
 * In probability theory and statistics, the <em>Bernoulli distribution</em>,
 * named after Swiss scientist Jacob Bernoulli, is a discrete probability
 * distribution, which takes value 1 with success probability <em>p</em> and
 * value 0 with failure probability <em>q=1-p</em>.  So if <em>X</em> is a
 * random variable with this distribution, we have:
 * @f[
 *    \mathbb{P}(X=1) = 1 - \mathbb{P}(X=0) = 1 - q = p.
 * \f]
 * A classical example of a Bernoulli experiment is a single toss of a coin.
 * The coin might come up heads with probability p and tails with probability
 * 1-p. The experiment is called fair if p=0.5, indicating the origin of the
 * terminology in betting (the bet is fair if both possible outcomes have the
 * same probability).
 */
class Bernoulli: public IUnivLaw<Binary>
{
  public:
    typedef IUnivLaw<Binary> Base;
    /** constructor
     * @param prob probability of success in a Bernoulli trial
     **/
    Bernoulli(Real const& prob =0.5);
    /** destructor */
    inline virtual ~Bernoulli() {}
    /** @return the probability of success */
    inline Real const& prob() const { return prob_;}
    /** @param prob the probability of success to set */
    inline void setProb(Real const& prob )
    {
      if (prob<0) STKDOMAIN_ERROR_1ARG(Bernoulli::setProb,prob,prob must be >= 0);
      if (prob>1) STKDOMAIN_ERROR_1ARG(Bernoulli::setProb,prob,prob must be <= 1);
      prob_ = prob;
    }

    /** @return a Binary random variate . */
    virtual Binary rand() const;
    /** @brief compute the cumulative distribution function
     *  Give the probability that a Bernoulli random variate is less or equal
     *  to t.
     *  @param t a real value
     *  @return the value of the cdf
     **/
    virtual Real cdf(Real const& t) const;
    /** @brief compute the probability distribution function (density)
     *  Give the value of the pdf at the point x.
     *  @param x a binary value
     *  @return the value of the pdf
     **/
    virtual Real pdf(Binary const& x) const;
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x a binary value
     *  @return the value of the log-pdf
     **/
    virtual Real lpdf(Binary const& x) const;
    /** @brief inverse cumulative distribution function
     *  The quantile is defined as the smallest value @e x such that
     *  <em> F(x) >= p </em>, where @e F is the cumulative distribution function.
     *  @param prob a probability number
     **/
    virtual Binary icdf(Real const& prob) const;

    /** @param prob a probability number
     *  @return a Binary random variate.
     **/
    static Binary rand(Real const& prob);
    /** @brief compute the probability distribution function (density)
     *  Give the value of the pdf at the point x.
     *  @param x a binary value
     *  @param prob a probability number
     *  @return the value of the pdf
     **/
    static Real pdf(Binary const& x, Real const& prob);
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x a binary value
     *  @param prob a probability number
     *  @return the value of the log-pdf
     **/
    static Real lpdf(Binary const& x, Real const& prob);

  protected:
    /** probability of success in a Bernoulli trial */
    Real prob_;
};

} // namespace Law

} // namespace STK

#endif /* STK_LAW_BERNOULLI_H */
