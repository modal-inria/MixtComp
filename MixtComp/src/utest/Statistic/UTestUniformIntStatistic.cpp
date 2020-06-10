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
 * UTestUniformIntStatistic.cpp
 *
 *  Created on: 17 déc. 2018
 *      Author: Quentin Grimonprez
 */


#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;


TEST(UniformIntStatistic, cdf) {
	int min = -3;
	int max = 8;

	UniformIntStatistic unif;

	Real cdf1 = unif.cdf(max + 1, min, max);
	Real cdf0 = unif.cdf(min - 1, min, max);
	Real cdf = unif.cdf(min, min, max);
	Real cdfb = unif.cdf((max + min)/2, min, max);

	EXPECT_NEAR(cdf0, 0., epsilon);
	EXPECT_NEAR(cdf1, 1., epsilon);
	EXPECT_NEAR(cdf, 1./12., epsilon);
	EXPECT_NEAR(cdfb, 1./2., epsilon);
}



TEST(UniformIntStatistic, pdf) {
	int min = -3;
	int max = 8;

	UniformIntStatistic unif;

	Real pdf0 = unif.pdf(min - 1, min, max);
	Real pdf00 = unif.pdf(max + 1, min, max);
	Real pdf = unif.pdf(0, min, max);

	EXPECT_NEAR(pdf0, 0., epsilon);
	EXPECT_NEAR(pdf00, 0., epsilon);
	EXPECT_NEAR(pdf, 1./12., epsilon);
}


TEST(UniformIntStatistic, sample) {
	int min = -3;
	int max = 8;

	UniformIntStatistic unif;

	for(int i = 0; i < 10; ++i)
	{
		int x = unif.sample(min, max);
		EXPECT_GE(x, min);
		EXPECT_LE(x, max);
	}

}
