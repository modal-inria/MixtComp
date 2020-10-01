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
 *  Created on: January the 23rd, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/**
 * Numerically check that lpdf is correctly computed.
 * */
TEST(WeibullStatistic, DistributionFunctions) {
	Real k = 2.3;
	Real lambda = 0.9;
	Real obs = 3.0;

	WeibullStatistic weibull;

	Real directPDF = weibull.pdf(obs, k, lambda);
	Real directLPDF = weibull.lpdf(obs, k, lambda);

	ASSERT_NEAR(std::log(directPDF), directLPDF, epsilon);
}
