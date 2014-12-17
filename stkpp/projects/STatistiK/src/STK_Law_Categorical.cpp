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

/** @file STK_Law_Categorical.cpp
 *  @brief In this file we implement the Categorical class.
 **/

#include <cmath>
#include "../include/STK_Law_Categorical.h"

namespace STK
{
namespace Law
{

/* @return a @c Type random variate . */
int Categorical::rand() const
{
  Real u = Law::generator.randUnif();
  int k;
  for(k = cumProb_.begin(); k< cumProb_.end(); k++)
  { if (u<=cumProb_[k]) return k;}
  return k;
}

/* @brief compute the probability distribution function (density)
 *  Give the value of the pdf at the point x.
 *  @param x the value to compute the pdf.
 *  @return the value of the pdf
 **/
Real Categorical::pdf(int const& x) const
{ return prob_[x];}
/* @brief compute the log probability distribution function
 *  Give the value of the log-pdf at the point x.
 *  @param x the value to compute the lpdf.
 *  @return the value of the log-pdf
 **/
Real Categorical::lpdf(int const& x) const
{ return (prob_[x] == 0) ? -Arithmetic<Real>::infinity() : std::log(prob_[x]);}
/* @brief compute the cumulative distribution function
 *  Give the probability that a Categorical random variate is less or equal
 *  to t.
 *  @param t the value to compute the cdf.
 *  @return the value of the cdf
 **/
Real Categorical::cdf(Real const& t) const
{ return (t<prob_.begin()) ? 0. : (t>=prob_.lastIdx()) ? 1. : cumProb_[std::floor(t)];}

/* @brief inverse cumulative distribution function
 *  Compute the Real quantile t such that the probability of a random
 *  variate less to t is less or equal to p.
 *  @param p value of the probability giving the quantile
 **/
int Categorical::icdf(Real const& prob) const
{
  if (prob<0) STKDOMAIN_ERROR_1ARG(Categorical::icdf,prob,prob must be >= 0);
  if (prob>1) STKDOMAIN_ERROR_1ARG(Categorical::icdf,prob,prob must be <= 1);
  int k;
  for (k = cumProb_.begin(); k< cumProb_.lastIdx(); ++k)
  { if (cumProb_[k] >= prob) return k;}
  return k;
}

void Categorical::computeCumProb()
{
  cumProb_.resize(prob_.range());
  Real sum=0.;
  for (int k=prob_.begin(); k< prob_.end(); ++k)
  { cumProb_[k] = (sum+=prob_[k]);}
  // normalize
  if (sum) {cumProb_/=sum; prob_ /=sum;}
  else {STKINVALIDARGUMENT_ERROR_NO_ARG(Categorical::computeCumProb,sum of the probabilities is zero);}
}

} /* namespace Law */

} /* namespace STK */
