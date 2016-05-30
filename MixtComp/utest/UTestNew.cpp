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

TEST(Functional, optimRealSimpleCase) {
  Index nTime = 1000;
  Index nSub = 2; // number of subregression in the generation / estimation phases
  Index nIter = 100000; // number of iterations in the optimization process
  Index nCoeff = 2; // order of each subregression
  Real xMax = 50.;
  Real alphaSlope = 0.5;

  Real alpha0 = xMax / 2.;
  Index nParam = nSub * 2; // regression order for

  Vector<Real> t(nTime);
  for (Index i = 0; i < nTime; ++i) {
    t(i) = i * xMax / nTime;
  }

  Vector<Real> alpha(nParam); // alpha is linearized in a single vector, for easier looping
  alpha <<  alpha0 * alphaSlope, -alphaSlope,
           -alpha0 * alphaSlope,  alphaSlope;
  std::cout << "Initial alpha" << std::endl;
  std::cout << itString(alpha) << std::endl;

  Vector<std::list<Index> > w(nSub);
  Vector<Real> y(nTime, 0.);

  Matrix<Real> beta(nSub, nCoeff + 1);
  beta.row(0) <<  0.,  1., 0.; // y =  x      + N(0, 1)
  beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)

  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;
  timeValue(t,
            alpha,
            logValue,
            logSumExpValue);
  std::cout << "logValue" << std::endl;
  std::cout << logValue << std::endl;
  std::cout << "logSumExpValue" << std::endl;
  std::cout << itString(logSumExpValue) << std::endl;

  MultinomialStatistic multi;
  NormalStatistic normal;
  UniformStatistic uni;

  Matrix<Real> kappa(nTime, nSub);
  for (Index i = 0; i < nTime; ++i) {
    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
    Index currW = multi.sample(kappa.row(i));
    w(currW).push_back(i); // sample the subregression

    for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
      y(i) += beta(currW, p) * pow(t(i), p);
    }
    y(i) += normal.sample(0, beta(currW, nCoeff));
  }

//  for (Index sub = 0; sub < nSub; ++sub) {
//    std::cout << "sub: " << sub << std::endl;
//    std::cout << itString(w(sub)) << std::endl;
//  }
//  std::cout << "y" << std::endl;
//  std::cout << itString(y) << std::endl;

  Matrix<Real> lambda;
  computeLambda(t,
                y,
                alpha,
                beta,
                lambda);

//  Vector<Real> estimatedAlpha(nParam);
//  for (Index i = 0; i < nParam; ++i) {
//    estimatedAlpha(i) = uni.sample(-20., 20.);
//  }
//  std::cout << "estimated alpha : " << itString(estimatedAlpha) << std::endl;
  Vector<Real> estimatedAlpha(nParam, 0.); // all alpha coefficient initialized at 0
//  Vector<Real> estimatedAlpha = alpha; // alpha initialized using the solution

  Real alpha_k = 1.;
  Real costCurr;
  Vector<Real> gradCurr;

  timeValue(t,
            estimatedAlpha,
            logValue,
            logSumExpValue);
  costFunction(t,
               logValue,
               logSumExpValue,
               w,
               costCurr);
  gradCostFunction(t,
                   logValue,
                   logSumExpValue,
                   w,
                   gradCurr);

  for (Index iter = 0; iter < nIter; ++iter) {
    updateAlpha(nParam,
                t,
                w,
                estimatedAlpha,
                alpha_k,
                costCurr,
                gradCurr);
    std::cout << "iter: " << iter << ", estimatedAlpha: " << itString(estimatedAlpha) << ", costCurr: " << costCurr << ", gradCurr: " << itString(gradCurr) << std::endl;
  }
}

//TEST(Functional, optimRealCase) {
//  Index nTime = 1000;
//  Index nSub = 2; // number of subregression in the generation / estimation phases
//  Index nIter = 10; // number of iterations in the optimization process
//  Index nCoeff = 2; // order of each subregression
//  Real xMax = 50.;
//
//  Index nParam = nSub * 2; // regression order for
//
//  Vector<Real> t(nTime);
//  for (Index i = 0; i < nTime; ++i) {
//    t(i) = i * xMax / nTime;
//  }
//
//  Vector<Real> alpha(nParam); // alpha is linearized in a single vector, for easier looping
//  alpha <<  25., -1.,
//           -25.,  1.;
//
//  Vector<Real> w(nTime);
//  Vector<Real> y(nTime, 0.);
//
//  Matrix<Real> beta(nSub, nCoeff + 1);
//  beta.row(0) << 0. ,  1., 1.; // y =  x      + N(0, 1)
//  beta.row(1) << 50., -1., 1.; // y = -x + 50 + N(0, 1)
//
//  Matrix<Real> logValue;
//  Vector<Real> logSumExpValue;
//  timeValue(t,
//            alpha,
//            logValue,
//            logSumExpValue);
//
//  MultinomialStatistic multi;
//  NormalStatistic normal;
//
//  Matrix<Real> kappa(nTime, nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
//    w(i) = multi.sample(kappa.row(i)); // sample the subregression
//    for (Index p = 0; p < nCoeff; ++p) {
//      y(i) += beta(w(i), p) * pow(t(i), p);
//    }
//    y(i) += normal.sample(0, beta(w(i), nCoeff));
//  }
//
//  Matrix<Real> lambda;
//  computeLambda(t,
//                y,
//                alpha,
//                beta,
//                lambda);
//
//  Vector<std::list<Index> > postW(nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    postW(multi.sample(lambda.row(i))).push_back(i); // w is resampled, now using the a posteriori probability, to take Indexo account the error term sampled at each time
//  }
//
//  Vector<Real> estimatedAlpha(nParam, 0.);
//
//  for (Index i = 0; i < nIter; ++i) {
//    updateAlpha(nParam,
//                t,
//                postW,
//                estimatedAlpha);
//    std::cout << "i: " << i << ", alpha: " << itString(alpha) << std::endl;
//  }
//}

//TEST(Functional, optim) {
//  Index nTime = 1000;
//  Index nParam = 4;
//  Index nIter = 10;
//
//  Index nSub = nParam / 2;
//
//  Vector<Real> t(nTime);
//  Vector<std::list<Index> > w(nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    t(i) = i;
//    if (i < nTime / 2) {
//      w(0).push_back(i);
//    }
//    else {
//      w(1).push_back(i);
//    }
//  }
//
//  Vector<Real> alpha; // alpha is linearized in a single vector, for easier looping
//  initAlpha(nParam,
//            t,
//            alpha);
//
//  std::cout << "alpha: " << itString(alpha) << std::endl;
//  std::cout << "cost: " << costFunctionDebug(t, alpha, w) << std::endl;
//
//  for (Index i = 0; i < nIter; ++i) {
//    updateAlpha(nParam,
//                t,
//                w,
//                alpha);
//    std::cout << "alpha: " << itString(alpha) << std::endl;
//    std::cout << "cost: " << costFunctionDebug(t, alpha, w) << std::endl;
//  }
//}
