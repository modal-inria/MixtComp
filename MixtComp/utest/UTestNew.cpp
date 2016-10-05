/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

using namespace mixt;

/** Compute statistics on truncated distribution, using limit cases, where the truncation is numerically farthest from the mode. */
TEST(NormalStatistic, expVarTruncated) {
  Real mu = 12.;
  Real sigma = 23.;
  Real a = std::numeric_limits<Real>::lowest();
  Real b = std::numeric_limits<Real>::max();

  NormalStatistic normal;

  Real computedMu, computedSigma;

  normal.expSigmaTruncated(mu, sigma, a, b, computedMu, computedSigma);

  ASSERT_NEAR(mu   , computedMu   , epsilon);
  ASSERT_NEAR(sigma, computedSigma, epsilon);
}

/** Basic test for the unbounded sampler. Will be used as a template for more complex cases. */
TEST(NormalStatistic, sample) {
  Real mu = 12.;
  Real sigma = 5.;
  Index nSample = 1000000;
  Real computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.sample(mu, sigma);
  }

  meanSD(sampleVal, computedMu, computedSigma);

  ASSERT_NEAR(mu   , computedMu   , 0.01);
  ASSERT_NEAR(sigma, computedSigma, 0.01);
}

TEST(NormalStatistic, sampleIUpperInf0) {
  Real mu = 12.;
  Real sigma = 5.;

  Real a = 6.;
  Real b = 9.;

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.sampleI(mu, sigma, a, b);
  }

  normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

  meanSD(sampleVal, computedMu, computedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}
