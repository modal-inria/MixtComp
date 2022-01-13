/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: December 16, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <ctime>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/math/distributions/uniform.hpp>
#include <Statistic/RNG.h>
#include <Statistic/UniformStatistic.h>

namespace mixt
{

UniformStatistic::UniformStatistic() :
    rng_(seed(this))
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
