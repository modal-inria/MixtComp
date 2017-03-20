/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 16, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ExponentialStatistic.h"

#include <ctime>
#include <boost/random/variate_generator.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/math/distributions/exponential.hpp>

namespace mixt
{

ExponentialStatistic::ExponentialStatistic() :
    rng_(size_t(this) + time(0))
{}

Real ExponentialStatistic::cdf(Real x,
                               Real lambda) const
{
  boost::math::exponential expo(lambda);
  Real proba = boost::math::cdf(expo,
                                     x);
  return proba;
}

Real ExponentialStatistic::pdf(Real x,
                               Real lambda) const
{
  boost::math::exponential expo(lambda);
  Real proba = boost::math::pdf(expo,
                                     x);
  return proba;
}

Real ExponentialStatistic::sample(Real lambda)
{
  boost::random::exponential_distribution<> expo(lambda);
  boost::variate_generator<boost::random::mt19937&,
                           boost::random::exponential_distribution<> > generator(rng_,
                                                                                 expo);
  Real x = generator();
  return x;
}

} // namespace mixt
