/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;


/** Verify on a simple 2 individuals, 2 classes test case that the harmonic mean estimator
 * provides the correct results */
TEST(RankLikelihood, basicTest)
{
  RankLikelihood rl;
  rl.init(2, 2, 4);

  Matrix<Real> computedMat(2, 2);
  Matrix<Real> expectedMat(2, 2);

  rl.observe(0, 0, 0.3);
  rl.observe(0, 0, 0.2);
  rl.observe(0, 0, 0.8);

  rl.observe(0, 1, 0.1);
  rl.observe(0, 1, 0.5);

  rl.observe(1, 0, 0.3);
  rl.observe(1, 0, 0.2);
  rl.observe(1, 0, 0.9);
  rl.observe(1, 0, 0.1);

  rl.observe(1, 1, 0.3);
  rl.observe(1, 1, 0.2);
  rl.observe(1, 1, 0.7);

  rl.getHMean(computedMat);

  expectedMat(0, 0) = - std::log(1. / 3. * (1. / 0.3 + 1. / 0.2 + 1. / 0.8));
  expectedMat(0, 1) = - std::log(1. / 2. * (1. / 0.1 + 1. / 0.5));
  expectedMat(1, 0) = - std::log(1. / 4. * (1. / 0.3 + 1. / 0.2 + 1. / 0.9 + 1. / 0.1));
  expectedMat(1, 1) = - std::log(1. / 3. * (1. / 0.3 + 1. / 0.2 + 1. / 0.7));

#ifdef MC_DEBUG
  std::cout << "expectedMat: " << std::endl;
  std::cout << expectedMat << std::endl;
  std::cout << "computedMat: " << std::endl;
  std::cout << computedMat << std::endl;
#endif

  ASSERT_EQ(computedMat.isApprox(expectedMat), true);
}
