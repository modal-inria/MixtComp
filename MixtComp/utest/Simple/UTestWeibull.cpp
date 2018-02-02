/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/**
 * Estimation of k using Newton-Raphson.
 */
TEST(Weibull, EstimateK) {
	Index nObs = 100000;

	Real lambdaExpected = 1.0;
	Real kExpected = 1.5;

	std::string idName = "dummy";
	Index nClass = 0;
	Vector<Real> param(0);
	Vector<std::set<Index>> classInd;

	WeibullStatistic weib;
	Weibull w(idName, nClass, param, classInd);

	Vector<Real> x(nObs);
	for (Index i = 0; i < nObs; ++i) {
		x(i) = weib.sample(kExpected, lambdaExpected);
	}

	Real kComputed = w.positiveNewtonRaphson(x, 10.0, 10);
	Real lambdaComputed = w.estimateLambda(x, kExpected);

	ASSERT_NEAR(kExpected, kComputed, 0.01);
	ASSERT_NEAR(lambdaExpected, lambdaComputed, 0.01);
}

/**
 * All data is marked as completely missing. The distribution is fixed. Missing data are completed, and the parameters are estimated.
 * With enough observations, the estimated values should be close to the parameters used for sampling.
 */
TEST(Weibull, SampleAndEstimate) {
	Index nObs = 100000;

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

	Weibull weibull(idName, nClass, param, setInd);
	weibull.setData("", augData, learning_);

	weibull.mStep();

	ASSERT_LT((param - paramExpected).norm(), 0.01);
}

/**
 * initParam is supposed to fix the value of the parameters so that the median of the distribution equals the representative observation.
 * A Weibull variable is thus initialised, parameters are initialised via initParam, data is sampled, and median value is checked.
 */
TEST(Weibull, initParam) {
	Index nObs = 100000;

	Real medianExpected = 5.0;
	Index nClass = 1;

	std::string idName = "dummy";
	Vector<std::set<Index>> classInd;

	AugmentedData<Vector<Real>> augData;
	augData.setAllMissing(nObs);
	augData.setPresent(0, medianExpected); // the first observation will be used as representative

	Vector<Real> param(2);
	param << 0.0, 0.0;

	Vector<std::set<Index>> setInd(nClass);
	for (Index i = 0; i < nObs; ++i) {
		setInd(0).insert(i);
	}

	Weibull weibull(idName, nClass, param, setInd);
	weibull.setData("", augData, learning_);

	Vector<Index> initObs(1);
	initObs << 0;

	weibull.initParam(initObs);

	WeibullSampler wsampler(augData, param, 1);
	for (Index i = 0; i < nObs; ++i) {
		wsampler.samplingStepNoCheck(i, 0);
	}

	Real medianComputed = quantile(augData.data_, 0.5);

	ASSERT_NEAR(medianExpected, medianComputed, 1.0);
}
