/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January the 23rd, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/** Compute statistics on truncated distribution, using limit cases, where the truncation is numerically farthest from the mode. */
TEST(WeibullStatistic, sampleIB) {
  Real k = 2.3;
  Real lambda = 0.9;

  WeibullStatistic weibull;



  normal.expSigmaTruncated(mu, sigma, a, b, computedMu, computedSigma);

  ASSERT_NEAR(mu   , computedMu   , epsilon);
  ASSERT_NEAR(sigma, computedSigma, epsilon);
}
