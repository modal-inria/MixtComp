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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

using namespace mixt;

TEST(Functional, Vandermonde) {
  Vector<Real> timeStep(2);
  timeStep << 2., 12.;

  int degree = 2;
  Matrix<Real> vm;

  VandermondeMatrix(timeStep,
                    degree,
                    vm);

  Matrix<Real> expectedVm(2, 3);
  expectedVm << 1., 2., 4.,
                1., 12., 144.;

  ASSERT_EQ(true, vm.isApprox(expectedVm));
}

TEST(Functional, regression) {
  int degree = 3;
  int nObs = 100;

  Real xMin = -50.;
  Real xMax = 50.;

  Vector<Real> x(nObs);
  Vector<Real> y(nObs, 0.);

  Matrix<Real> design(nObs, degree + 1);

  Vector<Real> beta(degree + 1);
  beta << 6, -4, 12, 5; // 5 x**3 + 12 x**3 - 4 x + 6
  Vector<Real> betaEstimated;

  NormalStatistic normal;
  UniformStatistic uni;

  for (int i = 0; i < nObs; ++i) {
    x(i) = uni.sample(xMin, xMax);
    for (int p = 0; p < degree + 1; ++p) {
      y(i) += beta(p) * pow(x(i), p);
    }
  }

  VandermondeMatrix(x,
                    degree,
                    design);

  regression(design,
             y,
             betaEstimated);

  ASSERT_EQ(true, betaEstimated.isApprox(beta, epsilon));
}
