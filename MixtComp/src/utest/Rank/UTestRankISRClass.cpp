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
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

typedef typename std::pair<MisType, std::vector<int> > MisVal;

/**
 * Test if Rank::probaYgX (direct computation of distribution of Y / X) has produced a correct list of Y candidate using < comparator on consecutive values of result.
 * Actual value of the conditional probability is not tested, but the lnCompletedProbability used in its computation
 * already is tested elsewhere.
 * */
TEST(RankISRIndividual, probaYgX) {
	int nbPos = 4;
	int nbE = fac(nbPos);

	RankISRIndividual rank(nbPos);
	Vector<MisVal> obsData(nbPos, MisVal(missing_, { }));
	rank.setObsData(obsData);
	rank.removeMissing();

	Vector<int> muVec(nbPos);
	muVec << 0, 3, 1, 2;
	RankVal mu(nbPos);
	mu.setO(muVec);

	Real pi = 0.3;

	Vector < Vector<int> > resVec(nbE);
	Vector<Real> resProba(nbE);

	rank.probaYgX(mu, pi, resVec, resProba);

	bool sorted = true;
	for (int i = 0; i < nbE - 1; ++i) {
		if (!(resVec(i) < resVec(i + 1))) {
			sorted = false;
			break;
		}
	}

	EXPECT_TRUE(sorted);
}

/**
 * Test if the conditional distribution p(y / x) obtained using Gibbs sampling is significantly different from the distribution
 * obtained through direct computation. Kullback–Leibler_divergence quantifies this difference.
 * */
TEST(RankISRClass, gibbsY) {
	int nbPos = 4;
	int nbIterBurnIn = 500;
	int nbIterRun = 100000;
	Real tolerance = 1.e-3;
	int nbE = fac(nbPos); // number of random events

	Vector<int> dummyVec(nbPos);

	RankISRIndividual rank(nbPos);
	dummyVec << 2, 3, 1, 0;
	rank.setO(dummyVec); // set observed value x
	rank.removeMissing(); // initialize y_ randomly

	RankVal mu(nbPos);
	dummyVec << 0, 3, 1, 2;
	mu.setO(dummyVec);

	Real pi = 0.3;

	std::map<Vector<int>, Real> empDist; // empirical distribution
	std::map<Vector<int>, Real> calcDist; // calculated distribution

	for (int i = 0; i < nbIterBurnIn; ++i) {
		rank.sampleY(mu, pi);
	}

	for (int i = 0; i < nbIterRun; ++i) {
		rank.sampleY(mu, pi);
		dummyVec = rank.y();

		if (empDist.find(dummyVec) == empDist.end()) {
			empDist[dummyVec] = 1.; // first occurrence
		} else {
			empDist[dummyVec] += 1.;
		}
	}

	for (std::map<Vector<int>, Real>::iterator it = empDist.begin(); it != empDist.end(); ++it) // renormalization of the empirical distribution
			{
		it->second /= nbIterRun;
	}

	Vector < Vector<int> > resVec(nbE);
	Vector<Real> resProba(nbE);
	rank.probaYgX(mu, pi, resVec, resProba);
	for (int e = 0; e < nbE; ++e) // filling the map object used to compute the KL-divergence
			{
		calcDist[resVec(e)] = resProba(e);
	}

	Real KLDivergence = 0.; // computation of the Kullback–Leibler_divergence
	for (std::map<Vector<int>, Real>::iterator it = empDist.begin(); it != empDist.end(); ++it) {
		KLDivergence += it->second * (std::log(it->second) - std::log(calcDist[it->first]));
	}

	ASSERT_LT(KLDivergence, tolerance);
}

/** Test sampleMu by first generating individuals, then performing sampleMu. The test checks if the real mu has been sampled at least once. */
TEST(RankISRClass, sampleMu) {
	int nbPos = 5;
	int nbInd = 100;
	int nbIterburnIn = 100;
	int nbIterRun = 100;

	std::set<RankVal> sampledResult; // store the sampled ranks

	MultinomialStatistic multi;

	RankISRIndividual rankIndividual(nbPos); // rank which will be completed multiple time
	Vector<MisVal> obsData(nbPos, MisVal(missing_, { }));
	rankIndividual.setObsData(obsData);

	Vector<RankISRIndividual> data(nbInd); // will store the result of xGen
	std::set<Index> classInd;

	RankVal mu = { 0, 3, 1, 2, 4 }; // position -> modality representation
	Real pi = 0.7; // pi high enough to get mu, no matter the y obtained in removeMissing

	for (int i = 0; i < nbInd; ++i) {
		rankIndividual.removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
		rankIndividual.xGen(mu, pi);

		data(i) = rankIndividual;

		classInd.insert(i);
	}

	Vector<int> muVec(nbPos);
	std::iota(muVec.begin(), muVec.end(), 0); // estimated mu is randomly initialized
	multi.shuffle(muVec);
	RankVal muEst(nbPos);
	muEst.setO(muVec);

	RankISRClass rank(data, muEst, pi);

	for (int i = 0; i < nbIterburnIn; ++i) {
		rank.sampleMu(classInd);
	}

	for (int i = 0; i < nbIterRun; ++i) {
		rank.sampleMu(classInd);
		sampledResult.insert(muEst);
	}

	ASSERT_TRUE(sampledResult.find(mu) != sampledResult.end());
}

/** Test sampleMu by first generating individuals, then performing sampleMu.
 * The test checks if the real mu has been sampled at least once. */
TEST(RankISRClass, mStep) {
	int nbPos = 5;
	int nbInd = 200;
	int nbIterburnIn = 100;
	Real tolerance = 0.05;

	std::set<RankVal> sampledResult; // store the sampled ranks

	MultinomialStatistic multi;
	UniformStatistic uni;

	RankISRIndividual rankIndividual(nbPos); // rank which will be completed multiple time
	Vector<MisVal> obsData(nbPos, MisVal(missing_, { }));
	rankIndividual.setObsData(obsData);

	Vector<RankISRIndividual> data(nbInd); // will store the result of xGen
	std::set<Index> setInd;

	RankVal mu = { 0, 3, 1, 2, 4 }; // position -> modality representation
	Real pi = 0.75;

	for (int i = 0; i < nbInd; ++i) {
		data(i) = rankIndividual;

		data(i).removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
		data(i).xGen(mu, pi);
		setInd.insert(i);
	}

	Vector<int> muVec(nbPos);
	std::iota(muVec.begin(), muVec.end(), 0);
	multi.shuffle(muVec); // randomly initialized
	RankVal muEst(nbPos); // estimated mu
	muEst.setO(muVec);
	Real piEst = uni.sample(0.5, 1.); // estimated pi randomly initialized too

	RankISRClass rank(data, muEst, piEst);

	for (int i = 0; i < nbIterburnIn; ++i) {
		for (int ind = 0; ind < nbInd; ++ind) {
			data(ind).sampleY(muEst, piEst); // no sampleX is performed as the completion obtained through xGen is used
		}
		rank.sampleMu(setInd);
	}

	rank.mStep(setInd);

	ASSERT_EQ(mu, muEst);
	ASSERT_LT(std::abs(pi - piEst), tolerance);
}

TEST(RankISRClass, lnObservedProbability) {
	int nbPos = 4;

	Vector<int> x(nbPos);
	x << 0, 1, 2, 3;
	Vector<MisVal> obsData(nbPos, MisVal(missing_, { }));

	Vector<RankISRIndividual> data(1);
	data(0).setNbPos(nbPos);
	data(0).setO(x);
	data(0).setObsData(obsData);

	Vector<int> muVec(nbPos);
	muVec << 2, 3, 0, 1;
	Real pi = 0.8;

	RankVal mu(nbPos);
	mu.setO(muVec);

	RankISRClass rc(data, mu, pi);

	rc.computeObservedProba();

	ASSERT_EQ(rc.lnObservedProbability(0), 0.);
}
