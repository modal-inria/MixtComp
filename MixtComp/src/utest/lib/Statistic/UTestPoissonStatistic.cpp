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
 * UTestPoissonStatistic.cpp
 *
 *  Created on: 17 déc. 2018
 *      Author: Quentin Grimonprez
 */

#include "gtest/gtest.h"
#include "MixtComp.h"

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

		EXPECT_GE(xI, infBound);
		EXPECT_LE(xI, supBound);
		EXPECT_GE(xIB, infBound);
	}

}
