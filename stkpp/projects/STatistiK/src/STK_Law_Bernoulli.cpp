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

/** @file STK_Law_Bernoulli.cpp
 *  @brief In this file we implement the Bernoulli class.
 **/

#include "../include/STK_Law_Bernoulli.h"
#include "../include/STK_Law_Util.h"

namespace STK
{
namespace Law
{
/* constructor
 * @param prob probability of success in a Bernoulli trial
 **/
Bernoulli::Bernoulli(Real const& prob) : Base(String(_T("Bernoulli")) ), prob_(prob)
{
  if (prob<0) STKDOMAIN_ERROR_1ARG(Bernoulli,prob,prob must be >= 0);
  if (prob>1) STKDOMAIN_ERROR_1ARG(Bernoulli,prob,prob must be <= 1);
}

/* @return a @c Type random variate . */
Binary Bernoulli::rand() const
{
return (Law::generator.randUnif()<=prob_) ? one_ : zero_;}

/* @return a @c Bernoulli random variate . */
Binary Bernoulli::rand(Real const& prob)
{
#ifdef STK_DEBUG
  if (prob<0) STKDOMAIN_ERROR_1ARG(Bernoulli::rand,prob,prob must be >= 0);
  if (prob>1) STKDOMAIN_ERROR_1ARG(Bernoulli::rand,prob,prob must be <= 1);
#endif
  return (generator.randUnif()<=prob) ? one_ : zero_;
}

/* @brief compute the probability distribution function (density)
 *  Give the value of the pdf at the point x.
 *  @param x the value to compute the pdf.
 *  @return the value of the pdf
 **/
Real Bernoulli::pdf(Binary const& x) const
{
  switch (x)
  {
    case zero_: return 1.-prob_;
    case one_:  return prob_;
    default: break;
  }
  return Arithmetic<Real>::NA();
}
/* @brief compute the probability distribution function (density)
 *  Give the value of the pdf at the point x.
 *  @param x the value to compute the pdf.
 *  @return the value of the pdf
 **/
Real Bernoulli::pdf(Binary const& x, Real const& prob)
{
#ifdef STK_DEBUG
  if (prob<0) STKDOMAIN_ERROR_1ARG(Bernoulli::pdf,prob,prob must be >= 0);
  if (prob>1) STKDOMAIN_ERROR_1ARG(Bernoulli::pdf,prob,prob must be <= 1);
#endif
  switch (x)
  {
    case zero_: return 1.-prob;
    case one_:  return prob;
    default: break;
  }
  return Arithmetic<Real>::NA();
}
/* @brief compute the log probability distribution function
 *  Give the value of the log-pdf at the point x.
 *  @param x the value to compute the lpdf.
 *  @return the value of the log-pdf
 **/
Real Bernoulli::lpdf(Binary const& x) const
{
  switch (x)
  {
    case zero_: return (prob_ == 1) ? -Arithmetic<Real>::infinity() : std::log(1.-prob_);
    case one_: return (prob_ == 0) ? -Arithmetic<Real>::infinity() : std::log(prob_);
    default: break;
  }
  return Arithmetic<Real>::NA();
}

/* @brief compute the log probability distribution function
 *  Give the value of the log-pdf at the point x.
 *  @param x the value to compute the lpdf.
 *  @return the value of the log-pdf
 **/
Real Bernoulli::lpdf(Binary const& x, Real const& prob)
{
#ifdef STK_DEBUG
  if (prob<0) STKDOMAIN_ERROR_1ARG(Bernoulli::lpdf,prob,prob must be >= 0);
  if (prob>1) STKDOMAIN_ERROR_1ARG(Bernoulli::lpdf,prob,prob must be <= 1);
#endif
  switch (x)
  {
    case zero_: return (prob == 1) ? -Arithmetic<Real>::infinity() : std::log(1.-prob);
    case one_: return (prob == 0) ? -Arithmetic<Real>::infinity() : std::log(prob);
    default: break;
  }
  return Arithmetic<Real>::NA();
}

/* @brief compute the cumulative distribution function
 *  Give the probability that a Bernoulli random variate is less or equal
 *  to t.
 *  @param t the value to compute the cdf.
 *  @return the value of the cdf
 **/
Real Bernoulli::cdf(Real const& t) const
{ return (t<0.) ? 0. : (t<1.) ? 1.-prob_ : 1.;}

/* @brief inverse cumulative distribution function
 *  Compute the Real quantile t such that the probability of a random
 *  variate less to t is less or equal to p.
 *  @param p value of the probability giving the quantile
 **/
Binary Bernoulli::icdf(Real const& prob) const
{
  if (prob<0) STKDOMAIN_ERROR_1ARG(Bernoulli::icdf,prob,prob must be >= 0);
  if (prob>1) STKDOMAIN_ERROR_1ARG(Bernoulli::icdf,prob,prob must be <= 1);
  return (prob==0) ? zero_ : (prob==1) ? one_ : (prob <= 1.-prob_) ? zero_ :  one_;
}

} /* namespace Law */

} /* namespace STK */
