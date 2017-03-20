/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
                              Real supBound) {
  Real z;

  Real lower = (infBound - mean) / sd;
  Real upper = (supBound - mean) / sd;

  if (upper < 0) {
    z = -sideSampler(-upper, -lower);
  }
  else if (lower < 0 && 0 < upper) {
    if (upper - lower < sqrt(2. * pi)) { // tight spread, need to use specific algorithm
      z = lrbSampler(lower, upper);
    }
    else { // spread between bounds not important enough, traditional rejection sampling should work quickly
      do {
        z = sample(0., 1.);
      }
      while(z < lower || upper < z);
    }
  }
  else if (0 < lower) {
    z = sideSampler(lower, upper);
  }
  else {
    z = std::numeric_limits<Real>::signaling_NaN();; // the case is impossible
  }

  return z * sd + mean;
}

Real NormalStatistic::sampleIB(Real mean,
                               Real sd,
                               Real infBound) {
  Real z;
  Real lower = (infBound - mean) / sd;
  z = lbSampler(lower);
  return z * sd + mean;
}

Real NormalStatistic::sampleSB(Real mean,
                               Real sd,
                               Real supBound) {
  Real z;
  Real upper = (supBound - mean) / sd;
  z = -lbSampler(-upper);
  return z * sd + mean;
}

Real NormalStatistic::lbSampler(Real lower) {
  Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
  Real z, u, rho;
  if (lower < 0) { // less than 0.5 proba of rejection using regular sampling
    do {
      z = sample(0., 1.);
    }
    while (z < lower);
  }
  else {
    do {
      z = exponential_.sample(alpha) + lower;
      rho = exp(-pow((z - alpha), 2) / 2.);
      u = uniform_.sample(0., 1.);
    }
    while (u > rho);
  }
  return z;
}

Real NormalStatistic::lrbSampler(Real lower, Real upper) {
  Real z, u, rho;
  do {
    z = uniform_.sample(lower, upper);

    if (lower < 0. && 0. < upper) {
      rho = exp(-pow(z, 2));
    }
    else if (upper < 0.) { // lrbSampler is called from sideSampler for that case to occur
      rho = exp((pow(upper, 2) - pow(z, 2))/2);
    }
    else if (0. < lower) {
      rho = exp((pow(lower, 2) - pow(z, 2))/2);
    }

    u = uniform_.sample(0., 1.);
  }
  while(u > rho);

  return z;
}

Real NormalStatistic::sideSampler(Real lower, Real upper) {
  Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
  Real z;

  if (log(alpha) + alpha * lower / 2. > pow(lower, 2) / 2. - log(upper - lower)) { // test carried out in log, to avoid overflow
    do {
      z = lbSampler(lower); // if upper is far from lower, use lbSampler with rejection
    }
    while(upper < z);
  }
  else { // use true bounded sampler
    z = lrbSampler(lower, upper);
  }

  return z;
}

void NormalStatistic::expSigmaTruncated(Real mu,
                                        Real sigma,
                                        Real a,
                                        Real b,
                                        Real& truncMu,
                                        Real& truncSigma) {
  Real alpha = (a - mu) / sigma;
  Real beta  = (b - mu) / sigma;

  Real phiAlpha = pdf(alpha, 0., 1.);
  Real phiBeta  = pdf(beta , 0., 1.);

  Real PhiAlpha = cdf(alpha, 0., 1.);
  Real PhiBeta  = cdf(beta , 0., 1.);

  Real z = PhiBeta - PhiAlpha;

  truncMu = mu + (phiAlpha - phiBeta) / z * sigma;
  truncSigma = sqrt(pow(sigma, 2) * (1. + (alpha * phiAlpha - beta * phiBeta) / z - pow((phiAlpha - phiBeta) / z, 2)));
}

} // namespace mixt
