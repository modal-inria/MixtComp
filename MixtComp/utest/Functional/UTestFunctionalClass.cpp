/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 20, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(FunctionalClass, optimOneclassOneInd) {
  Index nTime = 100000;
  Index nSub = 2; // number of subregression in the generation / estimation phases
  Index nCoeff = 2; // order of each subregression
  Real xMax = 50.;

  Real alpha0 = - 1.;
  Real alpha1 = - alpha0 * 2. / xMax;

  Index nParam = nSub * 2; // regression order for

  Vector<Real> alpha(nParam);
  alpha << 0., 0., // alpha is linearized in a single vector, for easier looping
           alpha0,  alpha1;

  Matrix<Real> beta(nSub, nCoeff + 1);
  beta.row(0) <<  0.,  1., 0.; // y =  x      + N(0, 1)
  beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)

  Vector<Real> sd(nSub);
  sd << 0.1, 1.;

  Vector<std::set<Index> > w(nSub);
  Vector<Real> x(nTime, 0.);

  Vector<Real> t(nTime);
  for (Index i = 0; i < nTime; ++i) {
    t(i) = i * xMax / nTime;
  }

  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;
  timeValue(t,
            nParam,
            alpha.data(),
            logValue,
            logSumExpValue);

  MultinomialStatistic multi;
  NormalStatistic normal;
  UniformStatistic uni;

  Matrix<Real> kappa(nTime, nSub);
  for (Index i = 0; i < nTime; ++i) {
    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
    Index currW = multi.sample(kappa.row(i));
    w(currW).insert(i); // sample the subregression

    for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
      x(i) += beta(currW, p) * pow(t(i), p);
    }
    x(i) += normal.sample(0, sd(currW));
  }

  Vector<Function> data(1);
  data(0).setVal(t, x, w);

  std::set<Index> setInd;
  setInd.insert(0);

  FunctionalClass funcClass(data,
                            setInd,
                            0.95);
  funcClass.setSize(nSub, nCoeff);

  funcClass.mStepAlpha();

  Vector<Real> alphaComputed(nParam);
  for (Index s = 0; s < nSub; ++s) {
    alphaComputed(2 * s    ) = funcClass.alpha()(s, 0);
    alphaComputed(2 * s + 1) = funcClass.alpha()(s, 1);
  }

  std::cout << "alpha: " << itString(alpha) << std::endl;
  std::cout << "alphaComputed: " << itString(alphaComputed) << std::endl;

  ASSERT_EQ(true, alphaComputed.isApprox(alpha, 0.1));
}

TEST(FunctionalClass, optimOneclassMultiIndAlphaBetaSd) {
  Index nTime = 10000;
  Index nInd = 10;
  Index nSub = 2; // number of subregression in the generation / estimation phases
  Index nCoeff = 2; // order of each subregression
  Real xMax = 50.;

  Real alpha0 = - 1.;
  Real alpha1 = - alpha0 * 2. / xMax;

  Index nParam = nSub * 2; // regression order for

  Vector<Real> alpha(nParam);
  alpha << 0., 0., // alpha is linearized in a single vector, for easier looping
           alpha0, alpha1;

  Matrix<Real> beta(nSub, nCoeff);
  beta.row(0) <<  0.,  1.; // y =  x
  beta.row(1) << 50., -1.; // y = -x + 50

  Vector<Real> sd(nSub);
  sd << 0.1, 1.;

  Vector<Function> data(nInd);
  std::set<Index> setInd;

  for (Index ind = 0; ind < nInd; ++ind) {
    Vector<std::set<Index> > w(nSub);
    Vector<Real> x(nTime, 0.);

    Vector<Real> t(nTime);
    for (Index i = 0; i < nTime; ++i) {
      t(i) = i * xMax / nTime;
    }

    Matrix<Real> logValue;
    Vector<Real> logSumExpValue;
    timeValue(t,
              nParam,
              alpha.data(),
              logValue,
              logSumExpValue);

    MultinomialStatistic multi;
    NormalStatistic normal;
    UniformStatistic uni;

    Matrix<Real> kappa(nTime, nSub);
    for (Index i = 0; i < nTime; ++i) {
      kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
      Index currW = multi.sample(kappa.row(i));
      w(currW).insert(i); // sample the subregression

      for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
        x(i) += beta(currW, p) * pow(t(i), p);
      }
      x(i) += normal.sample(0, sd(currW));
    }

    data(ind).setVal(t, x, w);
    data(ind).computeVandermonde(nCoeff);
    setInd.insert(ind);
  }

  FunctionalClass funcClass(data,
                            setInd,
                            0.95);
  funcClass.setSize(nSub, nCoeff);

  funcClass.mStep();

  Vector<Real> alphaComputed(nParam);
  for (Index s = 0; s < nSub; ++s) {
    alphaComputed(2 * s    ) = funcClass.alpha()(s, 0);
    alphaComputed(2 * s + 1) = funcClass.alpha()(s, 1);
  }

  std::cout << "alpha: " << itString(alpha) << std::endl;
  std::cout << "alphaComputed: " << itString(alphaComputed) << std::endl;

  ASSERT_EQ(true, alphaComputed.isApprox(alpha, 0.1));
  ASSERT_EQ(true, funcClass.beta().isApprox(beta, 0.1));
  ASSERT_EQ(true, funcClass.sd().isApprox(sd, 0.1));
}

TEST(FunctionalClass, checkNbDifferentValue) {
  Index nInd = 100;
  Index nClass = 2;
  Index nSub = 2;
  Index nCoeff = 2;
  Index nTime = 100;
  Real xMin = -30.;
  Real xMax = 35.;

  Vector<Function> data(nInd);
  std::set<Index> setInd;
  Real confidenceLevel = 0.95;

  MultinomialStatistic multi;
  UniformStatistic uni;

  for (Index i = 0; i < nInd; ++i) {
    Vector<Real> t(nTime);
    Vector<Real> x(nTime);
    Vector<std::set<Index> > w(nSub);

    for (Index currT = 0; currT < nTime; ++currT) {
      t(currT) = 12.; // all time values identical: that should be enough to trigger the error detection
      x(currT) = uni.sample(xMin, xMax);
      w(multi.sampleInt(0, nSub - 1)).insert(currT);
    }

    data(i).setVal(t, x, w);
    if (multi.sampleInt(0, nClass - 1) == 0) {
      setInd.insert(i);
    }
  }

  FunctionalClass fc(data, setInd, confidenceLevel);
  fc.setSize(nSub, nCoeff);
  bool diffVal = fc.checkNbDifferentValue();

  ASSERT_EQ(false, diffVal);
}

TEST(FunctionalClass, checkNonNullSigma) {
  Index nInd = 100;
  Index nClass = 2;
  Index nSub = 2;
  Index nCoeff = 2;
  Index nTime = 100;
  Real tMin = -4.;
  Real tMax = 12.;

  Vector<Function> data(nInd);
  std::set<Index> setInd;
  Real confidenceLevel = 0.95;

  MultinomialStatistic multi;
  UniformStatistic uni;

  Matrix<Real> alpha(nSub, 2);
  Matrix<Real> beta(nSub, nCoeff);
  beta << -9., 4.,
          5., 6.7;
  Vector<Real> sd(nSub);

  for (Index i = 0; i < nInd; ++i) {
    Vector<Real> t(nTime);
    Vector<Real> x(nTime);
    Vector<std::set<Index> > w(nSub);
    Matrix<Real> vandermonde;

    for (Index currT = 0; currT < nTime; ++currT) {
      t(currT) = uni.sample(tMin, tMax);
    }
    vandermondeMatrix(t, nCoeff, vandermonde);

    for (Index currT = 0; currT < nTime; ++currT) {
      Index s = multi.sampleInt(0, nSub - 1);
      w(s).insert(currT);
      x(currT) = vandermonde.row(currT).dot(beta.row(s)); // no noise is added and would result in a null estimated standard deviation. This will trigger the error detection.
    }

    data(i).setVal(t, x, w);
    data(i).computeVandermonde(nCoeff);

    if (multi.sampleInt(0, nClass - 1) == 0) {
      setInd.insert(i);
    }
  }

  FunctionalClass fc(data, setInd, confidenceLevel);
  fc.setSize(nSub, nCoeff);
  fc.setParam(alpha, beta, sd);
  bool diffVal = fc.checkNonNullSigma();

  ASSERT_EQ(false, diffVal);
}
