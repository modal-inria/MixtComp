/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

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
