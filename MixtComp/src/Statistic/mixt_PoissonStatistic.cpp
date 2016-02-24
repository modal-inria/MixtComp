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
 *  Created on: December 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <ctime>
#include "mixt_PoissonStatistic.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/math/distributions/poisson.hpp>
#include "../LinAlg/mixt_Math.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

PoissonStatistic::PoissonStatistic() :
    rng_(size_t(this) + time(0))
{}

Real PoissonStatistic::pdf(int x,
                           Real lambda) const {
    boost::math::poisson pois(lambda);
    Real proba = boost::math::pdf(pois,
                                  x);
    return proba;
}

Real PoissonStatistic::lpdf(int x,
                           Real lambda) const {
    return Real(x) * std::log(lambda) - lambda - logFac(x);
}

int PoissonStatistic::sample(Real lambda)
{
  if (lambda > epsilon)
  {
    boost::poisson_distribution<> pois(lambda);
    boost::variate_generator<boost::mt19937&,
                             boost::poisson_distribution<> > generator(rng_,
                                                                       pois);
    int x = generator();
    return x;
  }
  else
  {
    return 0;
  }
}

int PoissonStatistic::nonZeroSample(Real lambda)
{
  Real u = uniform_.sample(0., 1.);
  Real cdf = 0.;
  int currMod = 0; // modality 0 is ignored
  Real coeff = 1. / (1. - pdf(0, lambda)); // coeff to take the condition that x != 0 into account
  while(cdf < u)
  {
    ++currMod;
    cdf += pdf(currMod, lambda) * coeff;
  }
  return currMod;
}

} // namespace mixt
