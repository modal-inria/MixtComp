/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */
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
#include "mixt_RNG.h"

namespace mixt
{

PoissonStatistic::PoissonStatistic() :
    rng_(seed(this))
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
