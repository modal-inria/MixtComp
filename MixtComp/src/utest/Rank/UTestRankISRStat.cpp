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
 *  Created on: September 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

typedef typename std::pair<MisType, std::vector<int> > MisVal;

/**
 * Use RankParamStat to compute statistics on mu and pi. Use the setExpectationParam to make a comparison
 * with the real parameters
 * */
TEST(RankStat, computeStat) {
	int nbPos = 4;
	int nbInd = 100;
	int nbIterburnIn = 50;
	int nbIterRun = 100;
	Real confidenceLevel = 0.95;

	MultinomialStatistic multi;
	UniformStatistic uni;

	RankISRIndividual rankIndividual(nbPos); // rank which will be completed multiple time
	Vector<MisVal> obsData(nbPos, MisVal(missing_, { }));
	rankIndividual.setObsData(obsData);

	Vector<RankISRIndividual> data(nbInd); // will store the result of xGen
	std::set<Index> setInd;

	RankVal mu = { 0, 3, 1, 2 }; // ordering (position -> modality) representation
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

	RankISRStat paramStat(muEst, confidenceLevel); // stat computer on muEst and piEst

	RankISRClass rank(data, muEst, piEst);

	for (int i = 0; i < nbIterburnIn; ++i) {
		for (int ind = 0; ind < nbInd; ++ind) {
			data(ind).sampleY(muEst, piEst);
		}
		rank.sampleMu(setInd);
	}

	for (int i = 0; i < nbIterRun; ++i) {
		for (int ind = 0; ind < nbInd; ++ind) {
			data(ind).sampleY(muEst, piEst);
		}
		rank.mStep(setInd);
		paramStat.sampleValue(i, nbIterRun - 1);
	}

	paramStat.setExpectationParam();

	ASSERT_EQ(mu, muEst);
}
