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
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/**
 * Numerically check that lpdf is the log of pdf
 * */
TEST(NegativeBinomialStatistic, DistributionFunctions) {
	Real n = 10.5;
	Real p = 0.6;
	size_t obs = 3;

	NegativeBinomialStatistic nBinom;

	Real lpdf0 = std::log(nBinom.pdf(obs, n, p));
	Real lpdf1 = nBinom.lpdf(obs, n, p);

	EXPECT_NEAR(lpdf0, lpdf1, epsilon);


	// case p == 1
	Real pdf0 = nBinom.pdf(0, n, 1.);
	lpdf0 = nBinom.lpdf(0, n, 1.);

	EXPECT_EQ(pdf0, 1.);
	EXPECT_EQ(lpdf0, 0.);

	Real pdf1 = nBinom.pdf(obs, n, 1.);
	lpdf1 = nBinom.lpdf(obs, n, 1.);

	EXPECT_EQ(pdf1, 0.);
	EXPECT_EQ(lpdf1, minInf);

	// case p == 0
	pdf0 = nBinom.pdf(0, n, 0.);
	lpdf0 = nBinom.lpdf(0, n, 0.);

	EXPECT_EQ(pdf0, 0.);
	EXPECT_EQ(lpdf0, minInf);

	pdf1 = nBinom.pdf(obs, n, 0.);
	lpdf1 = nBinom.lpdf(obs, n, 0.);

	EXPECT_EQ(pdf1, 0.);
	EXPECT_EQ(lpdf1, minInf);

}

TEST(NegativeBinomialStatistic, sample) {
	Real n = 10.5;
	Real p = 0.6;
	int infBound = 3;
	int supBound = 6;

	NegativeBinomialStatistic nBinom;

	for(int i = 0; i < 10; ++i)
	{
		int xI = nBinom.sampleI(n, p, infBound, supBound);
		int xIB = nBinom.sampleIB(n, p, infBound);

		EXPECT_GE(xI, infBound);
		EXPECT_LE(xI, supBound);
		EXPECT_GE(xIB, infBound);
	}


}
