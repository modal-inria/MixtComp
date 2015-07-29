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
