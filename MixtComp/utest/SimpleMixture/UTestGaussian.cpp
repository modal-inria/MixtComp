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
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(Gaussian_sjk, OKSample) {
  int nbClass = 2;
  int nbInd = 4;

  Vector<Real> param(2 * nbClass);
  param << 3. , 1.,
           10., 1.;

  Vector<std::set<Index> > classInd(nbClass);
  classInd(0) = {0, 2};
  classInd(1) = {1, 3};

  Vector<Real> data(nbInd);
  data << 23., 12., 5., 7.;

  std::string warnLog;

  Gaussian_sjk mixture("Gaussian",
                       nbClass,
                       param,
                       classInd);

  mixture.setData(data);
  int proba = mixture.checkSampleCondition(&warnLog);

#ifdef MC_DEBUG
  std::cout << "warnLog: " << warnLog << std::endl;
#endif

  ASSERT_EQ(proba, 1);
}

TEST(Gaussian_sjk, nullVarianceSample) {
  int nbClass = 2;
  int nbInd = 4;

  Vector<Real> param(2 * nbClass);
  param << 3. , 1.,
           10., 1.;

  Vector<std::set<Index> > classInd(nbClass);
  classInd(0) = {0, 2};
  classInd(1) = {1, 3};

  Vector<Real> data(nbInd);
  data << 23., 12., 23., 7.;

  std::string warnLog;

  Gaussian_sjk mixture("Gaussian",
                       nbClass,
                       param,
                       classInd);

  mixture.setData(data);
  int proba = mixture.checkSampleCondition(&warnLog);

#ifdef MC_DEBUG
  std::cout << "warnLog: " << warnLog << std::endl;
#endif

  ASSERT_EQ(proba, 0);
}

TEST(Gaussian_sjk, smallClass) {
  int nbClass = 2;
  int nbInd = 4;

  Vector<Real> param(2 * nbClass);
  param << 3. , 1.,
           10., 1.;

  Vector<std::set<Index> > classInd(nbClass);
  classInd(0) = {0, 2, 3};
  classInd(1) = {1};

  Vector<Real> data(nbInd);
  data << 23., 12., 15., 7.;

  std::string warnLog;

  Gaussian_sjk mixture("Gaussian",
                       nbClass,
                       param,
                       classInd);

  mixture.setData(data);
  int proba = mixture.checkSampleCondition(&warnLog);

#ifdef MC_DEBUG
  std::cout << "warnLog: " << warnLog << std::endl;
#endif

  ASSERT_EQ(proba, 0);
}

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

TEST(NormalStatistic, lbSamplerLowerInf0) {
  Real mu = 0.;
  Real sigma = 1.;

  Real lower = -1.;
  Real upper = std::numeric_limits<Real>::max();

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.lbSampler(lower);
  }

  meanSD(sampleVal, computedMu, computedSigma);
  normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu, expectedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lbSamplerLowerSup0) {
  Real mu = 0.;
  Real sigma = 1.;

  Real lower = 1.;
  Real upper = std::numeric_limits<Real>::max();

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.lbSampler(lower);
  }

  meanSD(sampleVal, computedMu, computedSigma);
  normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu, expectedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lrbSamplerLowerInf0UpperSup0) {
  Real mu = 0.;
  Real sigma = 1.;

  Real lower = -0.5;
  Real upper = 0.25;

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.lrbSampler(lower, upper);
  }

  meanSD(sampleVal, computedMu, computedSigma);
  normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu, expectedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lrbSamplerUpperInf0) {
  Real mu = 0.;
  Real sigma = 1.;

  Real lower = -0.5;
  Real upper = -0.25;

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.lrbSampler(lower, upper);
  }

  meanSD(sampleVal, computedMu, computedSigma);
  normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu, expectedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lrbSamplerLowerSup0) {
  Real mu = 0.;
  Real sigma = 1.;

  Real lower = 0.25;
  Real upper = 0.5;

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.lrbSampler(lower, upper);
  }

  meanSD(sampleVal, computedMu, computedSigma);
  normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu, expectedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
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

TEST(NormalStatistic, sampleILowerSup0) {
  Real mu = -12.;
  Real sigma = 1.;

  Real a = -5.;
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

TEST(NormalStatistic, sampleITight) {
  Real mu = -12.;
  Real sigma = 1.;

  Real range = sqrt(2. * pi);

  Real lower = -0.25 * range;
  Real upper = 0.2 * range;

  Real a = lower * sigma + mu;
  Real b = upper * sigma + mu;

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

TEST(NormalStatistic, sampleIWide) {
  Real mu = 25.;
  Real sigma = 0.5;

  Real range = sqrt(2. * pi);

  Real lower = -2. * range;
  Real upper = 4. * range;

  Real a = lower * sigma + mu;
  Real b = upper * sigma + mu;

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

TEST(NormalStatistic, sampleIB) {
  Real mu = 25.;
  Real sigma = 4.3;

  Real a = 30.;
  Real b = std::numeric_limits<Real>::max();

  Index nSample = 1000000;
  Real expectedMu, expectedSigma, computedMu, computedSigma;

  NormalStatistic normal;
  Vector<Real> sampleVal(nSample);

  for (Index i = 0; i < nSample; ++i) {
    sampleVal(i) = normal.sampleIB(mu, sigma, a);
  }

  normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

  meanSD(sampleVal, computedMu, computedSigma);

  ASSERT_NEAR(expectedMu   , computedMu   , 0.01);
  ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}
