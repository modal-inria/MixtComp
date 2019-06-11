/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/**
 * Estimation of k using Newton-Raphson.
 */
TEST(NegativeBinomial, EstimateK) {
	Index nObs = 100000;

	Real nExpected = 15.0;
	Real pExpected = 0.75;

	std::string idName = "dummy";
	Index nClass = 0;
	Vector<Real> param(0);

	NegativeBinomialStatistic nbinom;
	NegativeBinomial nb(idName, nClass, param);

	Vector<int> x(nObs);
	for (Index i = 0; i < nObs; ++i) {
		x(i) = nbinom.sample(nExpected, pExpected);
	}

	Real nComputed = nb.estimateN(x, 10.0);
	Real pComputed = nb.estimateP(x, nExpected);

	ASSERT_NEAR(nExpected, nComputed, 0.1);
	ASSERT_NEAR(pExpected, pComputed, 0.01);
}

/**
 * All data is marked as completely missing. The distribution is fixed. Missing data are completed, and the parameters are estimated.
 * With enough observations, the estimated values should be close to the parameters used for sampling.
 */
TEST(NegativeBinomial, SampleAndEstimate) {
	Index nObs = 150000;

	Real nExpected = 10.0;
	Real pExpected = 0.6;
	Index nClass = 1;

	std::string idName = "dummy";
	Vector<std::set<Index>> classInd;

	AugmentedData<Vector<int>> augData;
	augData.setAllMissing(nObs);

	Vector<Real> param(2);
	param << nExpected, pExpected;

	Vector<Real> paramExpected = param;

	Vector<std::set<Index>> setInd(nClass);
	for (Index i = 0; i < nObs; ++i) {
		setInd(0).insert(i);
	}

	NegativeBinomialSampler nbsampler(augData, param, 1);
	for (Index i = 0; i < nObs; ++i) {
		nbsampler.samplingStepNoCheck(i, 0);
	}

	NegativeBinomial nbinom(idName, nClass, param);
	nbinom.setData("", augData, learning_);
	nbinom.initParam();
	nbinom.mStep(setInd);

	ASSERT_NEAR(nExpected, param(0), 0.1);
	ASSERT_NEAR(pExpected, param(1), 0.01);
}
