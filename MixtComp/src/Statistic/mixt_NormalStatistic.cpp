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
#include "mixt_NormalStatistic.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/math/distributions/normal.hpp>
#include "../Various/mixt_Constants.h"

namespace mixt
{
NormalStatistic::NormalStatistic() :
    rng_(size_t(this) + time(0))
{}

Real NormalStatistic::cdf(Real x,
                          Real mean,
                          Real sd) const
{
  boost::math::normal norm(mean,
                           sd);
  Real proba = boost::math::cdf(norm,
                                x);
  return proba;
}

/** log pdf evaluated at individual x */
Real NormalStatistic::lpdf(Real x,
                           Real mean,
                           Real sd) const
{
  return -log(sd) - l2pi - 0.5 * pow((x - mean) / sd, 2);
}

Real NormalStatistic::pdf(Real x,
                          Real mean,
                          Real sd) const
{
  boost::math::normal norm(mean,
                           sd);
  Real proba = boost::math::pdf(norm,
                                     x);
  return proba;
}

Real NormalStatistic::sample(Real mean,
                             Real sd)
{
  boost::normal_distribution<> norm(mean,
                                    sd);
  boost::variate_generator<boost::mt19937&,
                           boost::normal_distribution<> > generator(rng_,
                                                                    norm);
  Real x = generator();
  return x;
}

Real NormalStatistic::sampleI(Real mean,
                              Real sd,
                              Real infBound,
                              Real supBound)
{
#ifdef MC_DEBUG
  std::cout << "NormalStatistic::sampleI" << std::endl;
#endif
  Real z;

  Real lower = (infBound - mean) / sd;
  Real upper = (supBound - mean) / sd;

#ifdef MC_DEBUG
  std::cout << "\tmissingIntervals_" << std::endl;
  std::cout << "\tinfBound " << infBound << ", supBound: " << supBound << std::endl;
  std::cout << "\tlower: " << lower << ", upper: " << upper << std::endl;
#endif

  if (upper < 0)
  {
    z = -sideSampler(-upper, -lower);
  }
  else if (lower < 0 && 0 < upper)
  {
    if (upper - lower < sqrt(2. * pi)) // tight spread, need to use specific algorithm
    {
      z = lrbSampler(lower, upper);
    }
    else // spread between bounds not important enough, traditional rejection sampling should work quickly
    {
      do
      {
        z = sample(0., 1.);
      }
      while(z < lower || upper < z);
    }
  }
  else if (0 < lower)
  {
    z = sideSampler(lower, upper);
  }
  else {
    z = std::numeric_limits<Real>::signaling_NaN();; // the case is impossible
  }

#ifdef MC_DEBUG
  std::cout << "z * sd + mean: " << z * sd + mean << std::endl;
#endif
  return z * sd + mean;
}

Real NormalStatistic::sampleIB(Real mean,
                               Real sd,
                               Real infBound)
{
  Real z;
  Real lower = (infBound - mean) / sd;
  z = lbSampler(lower);
  return z * sd + mean;
}

Real NormalStatistic::sampleSB(Real mean,
                               Real sd,
                               Real supBound)
{
  Real z;
  Real upper = (supBound - mean) / sd;
  z = -lbSampler(-upper);
  return z * sd + mean;
}

Real NormalStatistic::lbSampler(Real lower)
{
#ifdef MC_DEBUG
  std::cout << "NormalStatistic::lbSampler" << std::endl;
#endif
  Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
  Real z, u, rho;
  if (lower < 0) // less than 0.5 proba of rejection using regular sampling
  {
    do
    {
      z = sample(0.,
                 1.);
    }
    while (z < lower);
  }
  else
  {
    do
    {
      z = exponential_.sample(alpha) + lower;
      rho = exp(-pow((z - alpha), 2) / 2.);
      u = uniform_.sample(0., 1.);
    }
    while (u > rho);
  }
  return z;
}

Real NormalStatistic::lrbSampler(Real lower, Real upper)
{
  Real z, u, rho;
#ifdef MC_DEBUG
  std::cout << "NormalStatistic::lrbSampler" << std::endl;
#endif
  do
  {
    z = uniform_.sample(lower, upper);

    if (lower < 0. && 0. < upper)
    {
      rho = exp(-pow(z, 2));
    }
    else if (upper < 0.)
    {
      rho = exp((pow(upper, 2) - pow(z, 2))/2);
    }
    else if (0. < lower)
    {
#ifdef MC_DEBUG
      std::cout << "0. < lower" << std::endl;
#endif
      rho = exp((pow(lower, 2) - pow(z, 2))/2);
    }

    u = uniform_.sample(0., 1.);
#ifdef MC_DEBUG
    std::cout << "lower: " << lower << ", upper: " << upper << std::endl;
    std::cout << "(pow(lower, 2): " << pow(lower, 2) << ", pow(z, 2): " << pow(z, 2) << "(pow(lower, 2) - pow(z, 2))/2: " << (pow(lower, 2) - pow(z, 2))/2 << std::endl;
    std::cout << "z: " << z << ", u: " << u << ", rho: " << rho << std::endl;
    std::cout << std::endl;
#endif
  }
  while(u > rho);

  return z;
}

Real NormalStatistic::sideSampler(Real lower, Real upper)
{
  Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
  Real z;

#ifdef MC_DEBUG
  std::cout << "NormalStatistic::sideSampler" << std::endl;
  std::cout << "\talpha: " << alpha << std::endl;
  std::cout << "(alpha * exp(alpha * lower / 2.) / sqrt(exp(1)): " << alpha * exp(alpha * lower / 2.) / sqrt(exp(1)) << std::endl;
  std::cout << "exp(pow(lower, 2) / 2) / (upper - lower))" << exp(pow(lower, 2) / 2) / (upper - lower) << std::endl;
  std::cout << "log(alpha) + alpha * lower / 2.: " << log(alpha) + alpha * lower / 2. << std::endl;
  std::cout << "pow(lower, 2) / 2. - log(upper - lower): " << pow(lower, 2) / 2. - log(upper - lower) << std::endl;
#endif

  if (log(alpha) + alpha * lower / 2. > pow(lower, 2) / 2. - log(upper - lower)) // test carried out in log, to avoid overflow
  {
#ifdef MC_DEBUG
    std::cout << "sup version, lbSampler" << std::endl;
#endif
    do
    {
      z = lbSampler(lower); // if upper is too far from lower, use lbSampler with rejection
    }
    while(upper < z);
  }
  else // use true bounded sampler
  {
    z = lrbSampler(lower, upper);
  }

  return z;
}

} // namespace mixt
