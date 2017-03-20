/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 16, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_UniformStatistic.h"

#include <ctime>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/math/distributions/uniform.hpp>

namespace mixt
{

UniformStatistic::UniformStatistic() :
    rng_(size_t(this) + time(0))
{}

Real UniformStatistic::cdf(Real x,
                           Real min,
                           Real max) const
{
  boost::math::uniform uni(min,
                           max);
  Real proba = boost::math::cdf(uni,
                                x);
  return proba;
}

Real UniformStatistic::pdf(Real x,
                           Real min,
                           Real max) const
{
  boost::math::uniform uni(min,
                           max);
  Real proba = boost::math::pdf(uni,
                                x);
  return proba;
}

Real UniformStatistic::sample(Real min,
                              Real max)
{
  boost::random::uniform_real_distribution<> uni(min,
                                                 max);
  boost::variate_generator<boost::random::mt19937&,
                           boost::random::uniform_real_distribution<> > generator(rng_,
                                                                                  uni);
  Real x = generator();
  return x;
}

} // namespace mixt
