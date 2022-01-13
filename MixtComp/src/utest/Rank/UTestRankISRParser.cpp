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
 *  Created on: September 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

typedef typename std::pair<MisType, std::vector<int> > MisVal;

TEST(RankParser, basicTest) {
	int nbMod = 4;

	std::vector<std::string> vecStr = { "1 , 3 , 2 , 0", "0 , 3 , 1 , 2", "0 , 1 , 2 , 3" };

	Vector<RankISRIndividual> vecIndComputed; // result of parsing
	Vector<RankISRIndividual> vecIndExpected(3); // expected result of parsing

	vecIndExpected(0).setNbPos(4);
	vecIndExpected(0).setO(std::vector<int>( { 1, 3, 2, 0 }));

	vecIndExpected(1).setNbPos(4);
	vecIndExpected(1).setO(std::vector<int>( { 0, 3, 1, 2 }));

	vecIndExpected(2).setNbPos(4);
	vecIndExpected(2).setO(std::vector<int>( { 0, 1, 2, 3 }));

	parseRankStr(vecStr, 0, nbMod, vecIndComputed);

	ASSERT_EQ(vecIndComputed(0).x(), vecIndExpected(0).x());
	ASSERT_EQ(vecIndComputed(1).x(), vecIndExpected(1).x());
	ASSERT_EQ(vecIndComputed(2).x(), vecIndExpected(2).x());
}

/** This test checks that the minimal and maximal modalities are correct, that is in the interval [minModality, nbPos - minModality + 1] */
TEST(RankParser, minMaxDetection) {
	int nbPos;
	std::vector<std::string> vecStr = { "{22 -8 1 2 3}, -1, 3, 38, 2" };

	Vector<RankISRIndividual> vecInd;

	MisVal misVal;

	std::string resStr = parseRankStr(vecStr, 1, nbPos, vecInd);

	ASSERT_GT(resStr.size(), 0);
}
