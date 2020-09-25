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
 *  Created on: June 20, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(FuncCSClass, optimOneclassOneInd) {
	Index nTime = 10000;
	Index nSub = 2; // number of subregression in the generation / estimation phases
	Index nCoeff = 2; // order of each subregression
	Real xMax = 50.;

	Real alpha1 = 0.5;
	Real alpha0 = -alpha1 * xMax / 2.;

	Index nParam = nSub * 2; // regression order for

	Vector<Real> alpha(nParam);
	alpha << 0., 0., alpha0, alpha1; // alpha is linearized in a single vector, for easier looping

	Matrix<Real> beta(nSub, nCoeff + 1);
	beta.row(0) << 0., 1., 0.; // y =  x      + N(0, 1)
	beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)

	Vector<Real> sd(nSub);
	sd << 0.1, 1.;

	Vector<std::set<Index> > w(nSub);
	Vector<Real> x(nTime, 0.);

	Vector<Real> t(nTime);
	for (Index i = 0; i < nTime; ++i) {
		t(i) = i * xMax / nTime;
	}

	Matrix<Real> logValue;
	Vector<Real> logSumExpValue;
	timeValue(t, nParam, alpha, logValue, logSumExpValue);

	MultinomialStatistic multi;
	NormalStatistic normal;
	UniformStatistic uni;

	Matrix<Real> kappa(nTime, nSub);
	for (Index i = 0; i < nTime; ++i) {
		kappa.row(i) = logValue.row(i).expE() / std::exp(logSumExpValue(i));
		Index currW = multi.sample(kappa.row(i));
		w(currW).insert(i); // sample the subregression

		for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
			x(i) += beta(currW, p) * pow(t(i), p);
		}
		x(i) += normal.sample(0, sd(currW));
	}

	Vector<FunctionCS> data(1);
	data(0).setVal(t, x, w);

	std::set<Index> setInd;
	setInd.insert(0);

	FuncCSClass funcClass(data, 0.95);
	funcClass.setSize(nSub, nCoeff);
	funcClass.mStepAlpha(setInd);

	Vector<Real> alphaComputed(nParam);
	for (Index s = 0; s < nSub; ++s) {
		alphaComputed(2 * s) = funcClass.alpha()(s, 0);
		alphaComputed(2 * s + 1) = funcClass.alpha()(s, 1);
	}

	ASSERT_EQ(true, alphaComputed.isApprox(alpha, 0.1));
}

TEST(FuncCSClass, optimOneclassMultiIndAlphaBetaSd) {
	Index nTime = 500;
	Index nInd = 10;
	Index nSub = 2; // number of subregression in the generation / estimation phases
	Index nCoeff = 2; // order of each subregression
	Real xMax = 50.;

	Real alpha1 = 0.5;
	Real alpha0 = -alpha1 * xMax / 2.;

	Index nParam = nSub * 2; // regression order for

	Vector<Real> alpha(nParam);
	alpha << 0., 0., // alpha is linearized in a single vector, for easier looping
	alpha0, alpha1;

	Matrix<Real> beta(nSub, nCoeff);
	beta.row(0) << 0., 1.; // y =  x
	beta.row(1) << 50., -1.; // y = -x + 50

	Vector<Real> sd(nSub);
	sd << 0.1, 1.;

	Vector<FunctionCS> data(nInd);
	std::set<Index> setInd;

	for (Index ind = 0; ind < nInd; ++ind) {
		Vector<std::set<Index> > w(nSub);
		Vector<Real> x(nTime, 0.);

		Vector<Real> t(nTime);
		for (Index i = 0; i < nTime; ++i) {
			t(i) = i * xMax / nTime;
		}

		Matrix<Real> logValue;
		Vector<Real> logSumExpValue;
		timeValue(t, nParam, alpha, logValue, logSumExpValue);

		MultinomialStatistic multi;
		NormalStatistic normal;
		UniformStatistic uni;

		Matrix<Real> kappa(nTime, nSub);
		for (Index i = 0; i < nTime; ++i) {
			kappa.row(i) = logValue.row(i).expE() / std::exp(logSumExpValue(i));
			Index currW = multi.sample(kappa.row(i));
			w(currW).insert(i); // sample the subregression

			for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
				x(i) += beta(currW, p) * pow(t(i), p);
			}
			x(i) += normal.sample(0, sd(currW));
		}

		data(ind).setVal(t, x, w);
		data(ind).computeVandermonde(nCoeff);
		setInd.insert(ind);
	}

	FuncCSClass funcClass(data, 0.95);
	funcClass.setSize(nSub, nCoeff);
	funcClass.mStep(setInd);

	Vector<Real> alphaComputed(nParam);
	for (Index s = 0; s < nSub; ++s) {
		alphaComputed(2 * s) = funcClass.alpha()(s, 0);
		alphaComputed(2 * s + 1) = funcClass.alpha()(s, 1);
	}

	ASSERT_EQ(true, alphaComputed.isApprox(alpha, 0.1));
	ASSERT_EQ(true, funcClass.beta().isApprox(beta, 0.1));
	ASSERT_EQ(true, funcClass.sd().isApprox(sd, 0.1));
}

TEST(FuncCSClass, checkNbDifferentValue) {
	Index nInd = 100;
	Index nClass = 2;
	Index nSub = 2;
	Index nCoeff = 2;
	Index nTime = 100;
	Real xMin = -30.;
	Real xMax = 35.;

	Vector<FunctionCS> data(nInd);
	std::set<Index> setInd;
	Real confidenceLevel = 0.95;

	MultinomialStatistic multi;
	UniformStatistic uni;

	for (Index i = 0; i < nInd; ++i) {
		Vector<Real> t(nTime);
		Vector<Real> x(nTime);
		Vector<std::set<Index> > w(nSub);

		for (Index currT = 0; currT < nTime; ++currT) {
			t(currT) = 12.; // all time values identical: that should be enough to trigger the error detection
			x(currT) = uni.sample(xMin, xMax);
			w(multi.sampleInt(0, nSub - 1)).insert(currT);
		}

		data(i).setVal(t, x, w);
		if (multi.sampleInt(0, nClass - 1) == 0) {
			setInd.insert(i);
		}
	}

	FuncCSClass fc(data, confidenceLevel);
	fc.setSize(nSub, nCoeff);
	bool diffVal = fc.checkNbDifferentValue(setInd);

	ASSERT_EQ(false, diffVal);
}
