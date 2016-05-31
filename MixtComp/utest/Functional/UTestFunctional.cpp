/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

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
 *  Created on: May 31, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(Functional, Vandermonde) {
  Vector<Real> timeStep(2);
  timeStep << 2., 12.;

  Index nCoeff = 3;
  Index nObs = 2;
  Matrix<Real> vm;

  VandermondeMatrix(timeStep,
                    nCoeff,
                    vm);

  Matrix<Real> expectedVm(nObs, nCoeff);
  expectedVm << 1., 2., 4.,
                1., 12., 144.;

  ASSERT_EQ(true, vm.isApprox(expectedVm));
}

TEST(Functional, regressionNoNoise) {
  Index nCoeff = 4;
  Index nObs = 100;

  Real xMin = -50.;
  Real xMax = 50.;

  Vector<Real> x(nObs);
  Vector<Real> y(nObs, 0.);

  Matrix<Real> design(nObs, nCoeff);

  Vector<Real> beta(nCoeff + 1); // +1 for the standard deviation
  beta << 6, -4, 12, 5, 0; // 5 x**3 + 12 x**2 - 4 x + 6, standard deviation is 0
  Vector<Real> betaEstimated;

  NormalStatistic normal;
  UniformStatistic uni;

  for (Index i = 0; i < nObs; ++i) {
    x(i) = uni.sample(xMin, xMax);
    for (Index p = 0; p < nCoeff; ++p) {
      y(i) += beta(p) * pow(x(i), p);
    }
  }

  VandermondeMatrix(x,
                    nCoeff,
                    design);

  regression(design,
             y,
             betaEstimated);

  ASSERT_EQ(true, betaEstimated.isApprox(beta, epsilon));
}

TEST(Functional, regressionNoise) {
  Index nCoeff = 4;
  Index nObs = 100000;

  Real xMin = -50.;
  Real xMax = 50.;

  Vector<Real> x(nObs);
  Vector<Real> y(nObs, 0.);

  Matrix<Real> design(nObs, nCoeff);

  Vector<Real> beta(nCoeff + 1); // +1 for the standard deviation
  beta << 6, -4, 12, 5, 1; // 5 x**3 + 12 x**2 - 4 x + 6, standard deviation is 12
  Vector<Real> betaEstimated;

  NormalStatistic normal;
  UniformStatistic uni;

  for (Index i = 0; i < nObs; ++i) {
    x(i) = uni.sample(xMin, xMax);
    for (Index p = 0; p < nCoeff; ++p) {
      y(i) += beta(p) * pow(x(i), p);
    }
    y(i) += normal.sample(0, beta(nCoeff));
  }

  VandermondeMatrix(x,
                    nCoeff,
                    design);

  regression(design,
             y,
             betaEstimated);

  ASSERT_EQ(true, betaEstimated.isApprox(beta, 1e-3));
}

TEST(Functional, subRegression) {
  Index nCoeff = 3;
  Index nObs = 100000;
  Index nSub = 3;

  Real xMin = -50.;
  Real xMax = 50.;

  Vector<Real> x(nObs);
  Vector<Real> y(nObs, 0.);

  Matrix<Real> design(nObs, nCoeff);

  Matrix<Real> beta(nSub, nCoeff + 1);
  beta.row(0) << 6, -4, 12, 1.; // 12 x**2 - 4 x + 6, sd = 1
  beta.row(1) << 9, 1, -3, 2; // -3 x**2 - x + 9, sd = 2
  beta.row(2) << -25, 25, 75, 3; // -3 x**2 - x + 9, sd = 3

  Matrix<Real> betaEstimated;

  MultinomialStatistic multi;
  NormalStatistic normal;
  UniformStatistic uni;

  Vector<std::list<Index> > w(nSub);

  for (Index i = 0; i < nObs; ++i) {
    x(i) = uni.sample(xMin, xMax);
    Index currW = multi.sampleInt(0, nSub - 1);
    w(currW).push_back(i);

    for (Index p = 0; p < nCoeff; ++p) {
      y(i) += beta(currW, p) * pow(x(i), p);
    }
    y(i) += normal.sample(0, beta(currW, nCoeff));
  }

  VandermondeMatrix(x,
                    nCoeff,
                    design);

  subRegression(design,
                y,
                w,
                betaEstimated);

  ASSERT_EQ(true, betaEstimated.isApprox(beta, 1e-3));
}

TEST(Functional, smallTest) {
  Vector<Real> a(4, 0.);

  Vector<Real> dummy;
  ASSERT_NEAR(4, std::exp(dummy.logToMulti(a)), epsilon);
}

TEST(Functional, costAndGrad1SubReg) {
  Index nTime = 4;
  Index nParam = 2;
  Real delta = epsilon;

  Index nSub = nParam / 2;

  Vector<Real> t(nTime);
  t << 0., 1., 2., 3.;

  Vector<std::list<Index> > w(nSub);
  w(0).push_back(0);
  w(0).push_back(1);
  w(0).push_back(2);
  w(0).push_back(3);

  Vector<Real> alpha0(nParam);
  alpha0 << -1., 0.5;

  Matrix<Real> value;
  Vector<Real> sumExpValue;
  Vector<Real> fdGrad(nParam); // finite differences gradient
  Vector<Real> computedGrad; // analytical gradient
  Real c0; // base cost

  timeValue(t,
            alpha0,
            value,
            sumExpValue);

  costFunction(t,
               value,
               sumExpValue,
               w,
               c0);

  gradCostFunction(t,
                   value,
                   sumExpValue,
                   w,
                   computedGrad);

  for (Index s = 0; s < nParam; ++s) {
    Real c1;
    Vector<Real> alpha1 = alpha0;
    alpha1(s) += delta;

    timeValue(t,
              alpha1,
              value,
              sumExpValue);

    costFunction(t,
                 value,
                 sumExpValue,
                 w,
                 c1);

    fdGrad(s) = (c1 - c0) / delta;
  }

  ASSERT_EQ(true, computedGrad.isApprox(fdGrad, epsilon));
}

TEST(Functional, costAndGrad) {
  Index nTime = 4;
  Index nParam = 4;
  Real delta = epsilon;

  Index nSub = nParam / 2;

  Vector<Real> t(nTime);
  t << 0., 1., 2., 3.;

  Vector<std::list<Index> > w(nSub);
  w(0).push_back(0);
  w(0).push_back(1);
  w(1).push_back(2);
  w(1).push_back(3);

  Vector<Real> alpha0(nParam);
  alpha0 << -1., 0.5, 1., -0.5;

  Matrix<Real> value;
  Vector<Real> sumExpValue;
  Vector<Real> fdGrad(nParam); // finite differences gradient
  Vector<Real> computedGrad; // analytical gradient
  Real c0; // base cost

  timeValue(t,
            alpha0,
            value,
            sumExpValue);

  costFunction(t,
               value,
               sumExpValue,
               w,
               c0);

  gradCostFunction(t,
                   value,
                   sumExpValue,
                   w,
                   computedGrad);

  for (Index s = 0; s < nParam; ++s) {
    Real c1;
    Vector<Real> alpha1 = alpha0;
    alpha1(s) += delta;

    timeValue(t,
              alpha1,
              value,
              sumExpValue);

    costFunction(t,
                 value,
                 sumExpValue,
                 w,
                 c1);

    fdGrad(s) = (c1 - c0) / delta;
  }

  ASSERT_EQ(true, computedGrad.isApprox(fdGrad, 1e-4));
}

TEST(Functional, hessian) {
  Index nTime = 4;
  Index nParam = 4;
  Real delta = 1e-5;

  Index nSub = nParam / 2;

  Vector<Real> t(nTime);
  Vector<std::list<Index> > w(nSub);
  for (Index i = 0; i < nTime; ++i) {
    t(i) = i;
    if (i < nTime / 2) {
      w(0).push_back(i);
    }
    else {
      w(1).push_back(i);
    }
  }

  Vector<Real> alpha0(nParam);
  alpha0 << -1., 0.5, 1., -0.5;

  Matrix<Real> value;
  Vector<Real> sumExpValue;
  Matrix<Real> fdHessian(nParam, nParam); // finite differences gradient
  Matrix<Real> computedHessian; // analytical gradient
  Real c00; // base cost

  timeValue(t,
            alpha0,
            value,
            sumExpValue);

  costFunction(t,
               value,
               sumExpValue,
               w,
               c00);

  hessianCostFunction(t,
                      value,
                      sumExpValue,
                      w,
                      computedHessian);

  for (Index row = 0; row < nParam; ++row) {
    for (Index col = 0; col < nParam; ++col) {
      Real c01;
      Real c10;
      Real c11;

      Vector<Real> alpha1;

      alpha1 = alpha0;
      alpha1(row) += delta;
      timeValue(t,
                alpha1,
                value,
                sumExpValue);
      costFunction(t,
                   value,
                   sumExpValue,
                   w,
                   c10);

      alpha1 = alpha0;
      alpha1(col) += delta;
      timeValue(t,
                alpha1,
                value,
                sumExpValue);
      costFunction(t,
                   value,
                   sumExpValue,
                   w,
                   c01);

      alpha1 = alpha0;
      alpha1(row) += delta;
      alpha1(col) += delta;
      timeValue(t,
                alpha1,
                value,
                sumExpValue);
      costFunction(t,
                   value,
                   sumExpValue,
                   w,
                   c11);

      fdHessian(row, col) = (c11 - c01 - c10 + c00) / (delta * delta);
    }
  }

  ASSERT_EQ(true, computedHessian.isApprox(fdHessian, 1e-4));
}
