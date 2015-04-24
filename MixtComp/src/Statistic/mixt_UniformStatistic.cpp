/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    rng_(time(0))
{}

UniformStatistic::~UniformStatistic()
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
