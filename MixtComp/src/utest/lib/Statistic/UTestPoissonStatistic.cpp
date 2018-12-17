/*
 * UTestPoissonStatistic.cpp
 *
 *  Created on: 17 déc. 2018
 *      Author: Quentin Grimonprez
 */

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;


TEST(PoissonStatistic, sample) {
	Real lambda = 4.;
	int infBound = 3;
	int supBound = 6;

	PoissonStatistic pois;


	for(int i = 0; i < 10; ++i)
	{
		int xI = pois.sampleI(lambda, infBound, supBound);
		int xIB = pois.sampleIB(lambda, infBound);
		int xSB = pois.sampleSB(lambda, supBound);

		EXPECT_GE(xI, infBound);
		EXPECT_LE(xI, supBound);
		EXPECT_GE(xIB, infBound);
		EXPECT_LE(xSB, supBound);
	}

}
