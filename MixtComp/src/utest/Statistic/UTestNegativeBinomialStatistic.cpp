/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "../UTest.h"

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

	ASSERT_NEAR(lpdf0, lpdf1, epsilon);
}
