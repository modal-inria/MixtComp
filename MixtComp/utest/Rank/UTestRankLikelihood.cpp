/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: September 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

#include "../../src/Mixture/Rank/mixt_RankLikelihood.h"

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
