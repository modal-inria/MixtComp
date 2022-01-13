/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: June 3, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(Function, lnCompletedProbability) {
	Index nTime = 5;
	Index nObs = 10000;
	Index nSub = 2;
	Index nCoeff = 4;
	Real xMin = 0.;
	Real xMax = 50.;
	Real alphaSlope = 0.5;
	Real sdVar = 1.;

	Real alpha0 = (xMax - xMin) / 2. + xMin;
	Index nParam = 2 * nSub;

	Matrix<Real> beta(nSub, nCoeff);
	beta << 6, -4, 12, 1, // x**3 + 12 x**2 - 4 x + 6
	9, 1, -3, 2; // 2 x**3 - 3 x**2 - x + 9

	Vector<Real> sd(nSub);
	sd << sdVar, sdVar;

	Vector<Real> t(nTime);
	for (Index i = 0; i < nTime; ++i) {
		t(i) = xMax * i / (nTime - 1.) + xMin;
	}

	Vector<Real> alphaArray(4);
	alphaArray << alpha0 * alphaSlope, -alphaSlope, -alpha0 * alphaSlope, alphaSlope; // alpha is linearized in a single vector, for easier looping
	Matrix<Real> alpha(nSub, 2);
	alpha << alpha0 * alphaSlope, -alphaSlope, -alpha0 * alphaSlope, alphaSlope; // alpha is linearized in a single vector, for easier looping

	Matrix<Real> logValue;
	Vector<Real> logSumExpValue;
	timeValue(t, nParam, alphaArray, logValue, logSumExpValue);

	UniformStatistic uni;
	GaussianStatistic normal;
	MultinomialStatistic multi;

	Real mode = std::numeric_limits<Real>::lowest();

	Matrix<Real> kappa(nTime, nSub);
	Real logKappaMaxSum = 0.;
	for (Index i = 0; i < nTime; ++i) {
		kappa.row(i) = logValue.row(i).expE() / std::exp(logSumExpValue(i));
		logKappaMaxSum += std::log(kappa.row(i).maxCoeff());
	}

	for (Index o = 0; o < nObs; ++o) { // loop over the observations
		Vector<std::set<Index> > w(nSub);
		Vector<Real> x(nTime, 0.);
		FunctionCS function;

		for (Index i = 0; i < nTime; ++i) {
			Index currSub = multi.sample(kappa.row(i));
			w(currSub).insert(i);
			for (Index p = 0; p < nCoeff; ++p) {
				x(i) += beta(currSub, p) * pow(t(i), p);
			}
			x(i) += normal.sample(0., sd(currSub));
		}

		function.setVal(t, x, w);
		function.computeVandermonde(nCoeff);

		Real logProba = function.lnCompletedProbability(alpha, beta, sd);
		if (mode < logProba) {
			mode = logProba;
		}
	}

	Real expectedMode = nTime * (normal.lpdf(0., 0., sdVar)) + logKappaMaxSum;
	ASSERT_GT(expectedMode, mode);
	ASSERT_NEAR(expectedMode, mode, 0.05);
}

TEST(Function, removeMissingQuantile) {
	Index nTime = 5;
	Index nSub = 2;

	Index nQuantile = nSub + 1;

	Vector<Real> t(nTime);
	t << -5., 16., 45., 1., -3.;

	Vector<Real> x(nTime);
	x << 9., 12., -4., 50., 0.;

	Vector<std::set<Index> > w(nSub);

	Vector<Real> quantile(nQuantile);
	quantile << -5., 2.3, 45.;

	FunctionCS function;
	function.setVal(t, x, w);
	function.removeMissingQuantile(quantile);

	std::set<Index> w0 = { 0, 3, 4 };
	std::set<Index> w1 = { 1, 2 };

	ASSERT_EQ(w0, function.w()(0));
	ASSERT_EQ(w1, function.w()(1));
}
