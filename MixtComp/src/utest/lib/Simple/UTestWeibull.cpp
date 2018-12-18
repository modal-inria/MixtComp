/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

/**
 * Estimation of k using Newton-Raphson.
 */
TEST(Weibull, EstimateK) {
	Index nObs = 5000;

	Real lambdaExpected = 1.0;
	Real kExpected = 1.5;

	std::string idName = "dummy";
	Index nClass = 0;
	Vector<Real> param(0);

	WeibullStatistic weib;
	Weibull w(idName, nClass, param);

	Vector<Real> x(nObs);
	for (Index i = 0; i < nObs; ++i) {
		x(i) = weib.sample(kExpected, lambdaExpected);
	}

	Real kComputed = w.estimateK(x, 10.0);
	Real lambdaComputed = w.estimateLambda(x, kExpected);

	ASSERT_NEAR(kExpected, kComputed, 0.01);
	ASSERT_NEAR(lambdaExpected, lambdaComputed, 0.01);
}

/**
 * All data is marked as completely missing. The distribution is fixed. Missing data are completed, and the parameters are estimated.
 * With enough observations, the estimated values should be close to the parameters used for sampling.
 */
TEST(Weibull, SampleAndEstimate) {
	Index nObs = 30000;

	Real kExpected = 1.5;
	Real lambdaExpected = 1.0;
	Index nClass = 1;

	std::string idName = "dummy";
	Vector<std::set<Index>> classInd;

	AugmentedData<Vector<Real>> augData;
	augData.setAllMissing(nObs);

	Vector<Real> param(2);
	param << kExpected, lambdaExpected;

	Vector<Real> paramExpected = param;

	Vector<std::set<Index>> setInd(nClass);
	for (Index i = 0; i < nObs; ++i) {
		setInd(0).insert(i);
	}

	WeibullSampler wsampler(augData, param, 1);
	for (Index i = 0; i < nObs; ++i) {
		wsampler.samplingStepNoCheck(i, 0);
	}

	Weibull weibull(idName, nClass, param);
	weibull.setData("", augData, learning_);
	weibull.initParam();

	weibull.mStep(setInd);

	ASSERT_LT((param - paramExpected).norm()/paramExpected.norm(), 0.01);
}

/**
 * Sample truncated data, compute some empirical cdf and quantile values, and compare them with what WeibullStatistic provides.
 */
TEST(Weibull, truncated) {
	Index nObs = 10000;

	Real kExpected = 5.0;
	Real lambdaExpected = 1.0;
	Real a = 1.0; // lower bound
	Index nClass = 1;

	std::string idName = "dummy";
	Vector<std::set<Index>> classInd;

	AugmentedData<Vector<Real>>::MisVal mv;
	mv.first = missingRUIntervals_;
	mv.second.push_back(a);

	AugmentedData<Vector<Real>> augData;
	augData.resizeArrays(nObs);
	for (Index i = 0; i < nObs; ++i) {
		augData.setMissing(i, mv);
	}

	Vector<Real> param(2);
	param << kExpected, lambdaExpected;

	Vector<Real> paramExpected = param;

	Vector<std::set<Index>> setInd(nClass);
	for (Index i = 0; i < nObs; ++i) {
		setInd(0).insert(i);
	}

	WeibullSampler wsampler(augData, param, 1);
	for (Index i = 0; i < nObs; ++i) {
		wsampler.samplingStepNoCheck(i, 0);
	}

	WeibullStatistic wstatistic;
	Real expectedPdf = wstatistic.cdfIB(1.25, kExpected, lambdaExpected, a);
	Real computedPdf = cdf(augData.data_, 1.25);
	Real expectedQuantile = wstatistic.quantileIB(kExpected, lambdaExpected, a, 0.5);
	Real computedQuantile = quantile(augData.data_, 0.5);

	ASSERT_NEAR(expectedPdf, computedPdf, 0.01);
	ASSERT_NEAR(expectedQuantile, computedQuantile, 0.01);
}
