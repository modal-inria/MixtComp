/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

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

	ASSERT_NEAR(mu, computedMu, epsilon);
	ASSERT_NEAR(sigma, computedSigma, epsilon);
}

/** Basic test for the unbounded sampler. Will be used as a template for more complex cases. */
TEST(NormalStatistic, sample) {
	Real mu = 12.;
	Real sigma = 5.;
	Index nSample = 100000;
	Real computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.sample(mu, sigma);
	}

	meanSD(sampleVal, computedMu, computedSigma);

	ASSERT_NEAR(mu, computedMu, 0.01);
	ASSERT_NEAR(sigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lbSamplerLowerInf0) {
	Real mu = 0.;
	Real sigma = 1.;

	Real lower = -1.;
	Real upper = std::numeric_limits<Real>::max();

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.lbSampler(lower);
	}

	meanSD(sampleVal, computedMu, computedSigma);
	normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu,
			expectedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lbSamplerLowerSup0) {
	Real mu = 0.;
	Real sigma = 1.;

	Real lower = 1.;
	Real upper = std::numeric_limits<Real>::max();

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.lbSampler(lower);
	}

	meanSD(sampleVal, computedMu, computedSigma);
	normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu,
			expectedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lrbSamplerLowerInf0UpperSup0) {
	Real mu = 0.;
	Real sigma = 1.;

	Real lower = -0.5;
	Real upper = 0.25;

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.lrbSampler(lower, upper);
	}

	meanSD(sampleVal, computedMu, computedSigma);
	normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu,
			expectedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lrbSamplerUpperInf0) {
	Real mu = 0.;
	Real sigma = 1.;

	Real lower = -0.5;
	Real upper = -0.25;

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.lrbSampler(lower, upper);
	}

	meanSD(sampleVal, computedMu, computedSigma);
	normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu,
			expectedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, lrbSamplerLowerSup0) {
	Real mu = 0.;
	Real sigma = 1.;

	Real lower = 0.25;
	Real upper = 0.5;

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.lrbSampler(lower, upper);
	}

	meanSD(sampleVal, computedMu, computedSigma);
	normal.expSigmaTruncated(mu, sigma, lower, upper, expectedMu,
			expectedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, sampleIUpperInf0) {
	Real mu = 12.;
	Real sigma = 5.;

	Real a = 6.;
	Real b = 9.;

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.sampleI(mu, sigma, a, b);
	}

	normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

	meanSD(sampleVal, computedMu, computedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, sampleILowerSup0) {
	Real mu = -12.;
	Real sigma = 1.;

	Real a = -5.;
	Real b = 9.;

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.sampleI(mu, sigma, a, b);
	}

	normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

	meanSD(sampleVal, computedMu, computedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
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

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.sampleI(mu, sigma, a, b);
	}

	normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

	meanSD(sampleVal, computedMu, computedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
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

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.sampleI(mu, sigma, a, b);
	}

	normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

	meanSD(sampleVal, computedMu, computedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}

TEST(NormalStatistic, sampleIB) {
	Real mu = 25.;
	Real sigma = 3.3;

	Real a = 30.;
	Real b = std::numeric_limits<Real>::max();

	Index nSample = 50000;
	Real expectedMu, expectedSigma, computedMu, computedSigma;

	NormalStatistic normal;
	Vector<Real> sampleVal(nSample);

	for (Index i = 0; i < nSample; ++i) {
		sampleVal(i) = normal.sampleIB(mu, sigma, a);
	}

	normal.expSigmaTruncated(mu, sigma, a, b, expectedMu, expectedSigma);

	meanSD(sampleVal, computedMu, computedSigma);

	ASSERT_NEAR(expectedMu, computedMu, 0.01);
	ASSERT_NEAR(expectedSigma, computedSigma, 0.01);
}
