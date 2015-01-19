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
#include "../Various/mixt_Constants.h"

namespace mixt
{

typedef PoissonStatistic::Type Type;

PoissonStatistic::PoissonStatistic() :
    rng_(time(0))
{}

PoissonStatistic::~PoissonStatistic()
{}

Real PoissonStatistic::pdf(Type x,
                                Real lambda) const
{
  if (lambda > epsilon)
  {
    boost::math::poisson pois(lambda);
    Real proba = boost::math::pdf(pois,
                                       x);
    return proba;
  }
  else if (x == 0)
  {
#ifdef MC_DEBUG
    std::cout << "Lambda very close to 0., for x = 0 -> proba is 1" << std::endl;
#endif
    return 1.;
  }
  else
  {
#ifdef MC_DEBUG
    std::cout << "Lambda very close to 0., for x = 1 -> proba is 0" << std::endl;
#endif
    return 0.;
  }
}

Type PoissonStatistic::sample(Real lambda)
{
  if (lambda > epsilon)
  {
    boost::poisson_distribution<> pois(lambda);
    boost::variate_generator<boost::mt19937&,
                             boost::poisson_distribution<> > generator(rng_,
                                                                       pois);
    Type x = generator();
    return x;
  }
  else
  {
    return 0;
  }
}

} // namespace mixt
