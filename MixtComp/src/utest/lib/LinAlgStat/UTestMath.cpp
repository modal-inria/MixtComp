/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

TEST(Math, Factorial) {
  int nbValue = 10;
  Vector<int> factoValue(nbValue);

  for (int i = 0; i < nbValue; ++i) {
    factoValue(i) = fac(i);
  }

  Vector<int> expectedFactoValue(10);
  expectedFactoValue << 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880;

  ASSERT_EQ(expectedFactoValue, factoValue);
}

TEST(Math, sumLog) {
  Vector<Real> vec(4);
  vec << 15., 1., 3., -7.;
  Real resComputed = sumLog(vec);
  Real resExpected = std::log(vec.sum());

  ASSERT_NEAR(resExpected, resComputed, epsilon);
}

TEST(Math, differentValue) {
	Real epsilon = 1.;
	Index nVal = 5;
	Index nDiffVal = 3;

	Vector<Real> falseVec(nVal);
	falseVec << 0.1, 0.2, 0.3, -10., -10.5;

	Vector<Real> trueVec(nVal);
	trueVec << 0.1, 0.2, 0.3, -10., -50.;

	ASSERT_EQ(differentValue(falseVec, nDiffVal, epsilon), false);
	ASSERT_EQ(differentValue(trueVec, nDiffVal, epsilon), true);
}
