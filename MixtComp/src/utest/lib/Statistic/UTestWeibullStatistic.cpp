/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January the 23rd, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

/**
 * Numerically check that pdf is the derivative of cdf, as per the definition. And that lpdf is correctly computed.
 * */
TEST(WeibullStatistic, DistributionFunctions) {
	Real k = 2.3;
	Real lambda = 0.9;
	Real obs = 3.0;

	WeibullStatistic weibull;

	Real cdf1 = weibull.cdf(k, lambda, obs + epsilon);
	Real cdf0 = weibull.cdf(k, lambda, obs);

	Real derivPDF = (cdf1 - cdf0) / epsilon;
	Real directPDF = weibull.pdf(k, lambda, obs);
	Real directLPDF = weibull.lpdf(k, lambda, obs);

	ASSERT_NEAR(derivPDF, directPDF, epsilon);
	ASSERT_NEAR(std::log(directPDF), directLPDF, epsilon);
}
