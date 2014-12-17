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

/** @file STK_Law_Categorical.h
 *  @brief In this file we define the Categorical distribution.
 **/

#ifndef STK_LAW_CATEGORICAL_H
#define STK_LAW_CATEGORICAL_H

#include "STK_Law_IUnivLaw.h"
#include "../include/STK_Law_Util.h"
#include "Arrays/include/STK_Array2DVector.h"

namespace STK
{

namespace Law
{

/** @ingroup Laws
 *  @brief Implement the Categorical probability law.
 *
 * In probability theory and statistics, a categorical distribution (also called
 * a "generalized Bernoulli distribution" or, less precisely, a "discrete
 * distribution") is a probability distribution that describes the result of a
 * random event that can take on one of @e K possible outcomes, with the
 * probability of each outcome separately specified. There is not necessarily
 * an underlying ordering of these outcomes, but numerical labels are attached
 * for convenience in describing the distribution, often in the range 1 to K.
 * Note that the K-dimensional categorical distribution is the most general
 * distribution over a K-way event; any other discrete distribution over a
 * size-K sample space is a special case. The parameters specifying the
 * probabilities of each possible outcome are constrained only by the fact that
 * each must be in the range 0 to 1, and all must sum to 1.
 *
 * Note that, in some fields, such as machine learning and natural language
 * processing, the categorical and multinomial distributions are conflated, and
 * it is common to speak of a "multinomial distribution" when a categorical
 * distribution is actually meant.
 */
class Categorical: public IUnivLaw<int>
{
  public:
    typedef IUnivLaw<int> Base;
    /** Default constructor. Only one category */
    inline Categorical() : Base(_T("Categorical")), prob_(1,1) { computeCumProb();}
    /** constructor with given probabilities.
     *  The probabilities will be normalized in order to have an overall sum of 1
     *  @param prob probabilities of success in a Categorical trial
     **/
    Categorical(Array2DVector<Real> const& prob) : Base(_T("Categorical")), prob_(prob)
    { computeCumProb();}
    /** constructor with given probabilities.
     *  The probabilities will be normalized in order to have an overall sum of 1
     *  @param prob probabilities of success in a Categorical trial
     **/
    template <class OtherArray>
    Categorical(OtherArray const& prob) : Base(_T("Categorical"))
    { prob_ = prob; computeCumProb();}
    /** destructor */
    inline virtual ~Categorical() {}

    /** @return the probabilities of success */
    inline Array2DVector<Real> const& prob() const { return prob_;}
    /** @return the cumulative probabilities of success */
    inline Array2DVector<Real> const& cumProb() const { return cumProb_;}

    /** @param prob the probability of success to set */
    template<class OtherArray>
    inline void setProb(OtherArray const& prob )
    { prob_ = prob; computeCumProb();}

    /** @return a categorical random variate . */
    virtual int rand() const;
    /** @brief compute the cumulative distribution function
     *  Give the probability that a Categorical random variate is less or equal
     *  to t.
     *  @param t a real value
     *  @return the value of the cdf
     **/
    virtual Real cdf(Real const& t) const;
    /** @brief compute the probability distribution function (density)
     *  Give the value of the pdf at the point x.
     *  @param x an integer value
     *  @return the value of the pdf
     **/
    virtual Real pdf(int const& x) const;
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x a binary value
     *  @return the value of the log-pdf
     **/
    virtual Real lpdf(int const& x) const;
    /** @brief inverse cumulative distribution function
     *  The quantile is defined as the smallest value @e x such that
     *  <em> F(x) >= p </em>, where @e F is the cumulative distribution function.
     *  @param prob a probability number
     **/
    virtual int icdf(Real const& prob) const;

    /** @return a categorical random variate . */
    template<class OtherArray>
    static int rand(OtherArray const& prob)
    {
      Real u = Law::generator.randUnif(), cum = 0.;
      for(int k = prob.begin(); k< prob.lastIdx(); k++)
      {
        cum += prob[k];
        if (u<=cum) return k;
      }
      return prob.lastIdx();
    }
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x the value to compute the lpdf.
     *  @param prob the probability of each value
     *  @return the value of the log-pdf
     **/
    template<class OtherArray>
    static Real lpdf(int const& x, OtherArray const& prob)
    { return (prob[x] == 0) ? -Arithmetic<Real>::infinity() : std::log(prob[x]);}

  protected:
    /** probabilities in a Categorical trial */
    Array2DVector<Real> prob_;
    /** cumulative probabilities in a Categorical trial */
    Array2DVector<Real> cumProb_;

  private:
    void computeCumProb();
};

} // namespace Law

} // namespace STK

#endif /* STK_LAW_CATEGORICAL_H */
