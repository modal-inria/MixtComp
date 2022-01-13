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
 *  Created on: July 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

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

TEST(Math, NewtonRaphsonRelTolNoConstraint) {
	Real xMin = 5.0;

	std::function<std::pair<Real, Real>(Real)> evalFunc([xMin](Real x){return std::pair<Real, Real>((x - xMin) * (x - xMin), 2.0 * x - 2.0 * xMin);}); // quadratic with minimum at 5.;
	Real x = 30.;

	Real sol = minimizePositiveNewtonRaphson(evalFunc, x);

	ASSERT_NEAR(sol, 5.0, 0.1);
}

TEST(Math, NewtonRaphsonRelTolConstraint) {
	Real xMin = -10.0;

	std::function<std::pair<Real, Real>(Real)> evalFunc([xMin](Real x){return std::pair<Real, Real>((x - xMin) * (x - xMin), 2.0 * x - 2.0 * xMin);}); // quadratic with minimum at 5.;
	Real x = 30.;

	Real sol = minimizePositiveNewtonRaphson(evalFunc, x);

	ASSERT_NEAR(sol, 0.0, 0.1);
}
