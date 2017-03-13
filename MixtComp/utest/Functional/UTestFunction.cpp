/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 3, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(Function, lnCompletedProbability) {
  Index nTime = 5;
  Index nObs = 100000;
  Index nSub = 2;
  Index nCoeff = 4;
  Real xMin = 0.;
  Real xMax = 50.;
  Real alphaSlope = 0.5;
  Real sdVar = 1.;

  Real alpha0 = (xMax - xMin) / 2. + xMin;
  Index nParam = 2 * nSub;

  Matrix<Real> beta(nSub, nCoeff);
  beta << 6, -4, 12, 1, // x**3 + 12 x**2 - 4 x + 6
          9,  1, -3, 2; // 2 x**3 - 3 x**2 - x + 9

  Vector<Real> sd(nSub);
  sd << sdVar, sdVar;

  Vector<Real> t(nTime);
  for (Index i = 0; i < nTime; ++i) {
    t(i) = xMax * i / (nTime - 1.) + xMin;
  }

  double alphaArray[] =  { alpha0 * alphaSlope, -alphaSlope, // alpha is linearized in a single vector, for easier looping
                          -alpha0 * alphaSlope,  alphaSlope};
  Matrix<Real> alpha(nSub, 2);
  alpha <<  alpha0 * alphaSlope, -alphaSlope, // alpha is linearized in a single vector, for easier looping
           -alpha0 * alphaSlope,  alphaSlope;

  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;
  timeValue(t,
            nParam,
            alphaArray,
            logValue,
            logSumExpValue);

  UniformStatistic uni;
  NormalStatistic normal;
  MultinomialStatistic multi;

  Real mode = std::numeric_limits<Real>::lowest();

  Matrix<Real> kappa(nTime, nSub);
  Real logKappaMaxSum = 0.;
  for (Index i = 0; i < nTime; ++i) {
    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
    logKappaMaxSum += std::log(kappa.row(i).maxCoeff());
  }

  for (Index o = 0; o < nObs; ++o) { // loop over the observations
    Vector<std::set<Index> > w(nSub);
    Vector<Real> x(nTime, 0.);
    Function function;

    for (Index i = 0; i < nTime; ++i) {
      Index currSub = multi.sample(kappa.row(i));
      w(currSub).insert(i);
      for (Index p = 0; p < nCoeff; ++p) {
        x(i) += beta(currSub, p) * pow(t(i), p);
      }
      x(i) += normal.sample(0., sd(currSub));
    }

    function.setVal(t, x, w);
    function.computeVandermonde(nCoeff);

    Real logProba = function.lnCompletedProbability(alpha, beta, sd);
    if (mode < logProba) {
      mode = logProba;
    }
  }

  Real expectedMode = nTime * (normal.lpdf(0., 0., sdVar)) + logKappaMaxSum;
  ASSERT_GT(expectedMode, mode);
  ASSERT_NEAR(expectedMode, mode, 0.05);
}

TEST(Function, removeMissingQuantile) {
  Index nTime = 5;
  Index nSub = 2;

  Vector<Real> t(nTime);
  t << -5., 16., 45., 1., -3.;

  Vector<Real> x(nTime);
  x << 9., 12., -4., 50., 0.;

  Vector<std::set<Index> > w(nSub);

  Vector<Real> quantile(nSub - 1);
  quantile << 2.3;

  Function function;
  function.setVal(t, x, w);
  function.removeMissingQuantile(quantile);

  std::set<Index> w0 = {0, 3, 4};
  std::set<Index> w1 = {1, 2};

  ASSERT_EQ(w0, function.w()(0));
  ASSERT_EQ(w1, function.w()(1));
}

TEST(Function, sampleWCheck) {
  Index nTime = 1000;
  Index nSub = 2;
  Index nCoeff = 1; // constant functions will be used
  Real xMin = 0.;
  Real xMax = 50.;
  Real alphaSlope = 0.5;
  Real sdVar = 0.1;

  Real alpha0 = (xMax - xMin) / 2. + xMin;

  Matrix<Real> beta(nSub, nCoeff);
  beta << 0.,
          10.;

  Vector<Real> sd(nSub);
  sd << sdVar, sdVar;

  Vector<Real> t(nTime);
  for (Index i = 0; i < nTime; ++i) {
    t(i) = xMax * i / (nTime - 1.) + xMin;
  }

  Matrix<Real> alpha(nSub, 2);
  alpha <<  alpha0 * alphaSlope, -alphaSlope, // alpha is linearized in a single vector, for easier looping
           -alpha0 * alphaSlope,  alphaSlope;

  Vector<std::set<Index> > w(nSub);
  Vector<Real> x(nTime, 0.);
  Vector<Function> function(1); // a single function is considered

  MultinomialStatistic multi;
  NormalStatistic normal;

  Matrix<Real> vandermonde;
  vandermondeMatrix(t, nCoeff, vandermonde);

  for (Index i = 0; i < nTime; ++i) {
    w(1).insert(i); // every points are assigned to the second subregression
    x(i) = normal.sample(vandermonde.row(i).dot(beta.row(0)),
                         sd(0)); // but data is generated using the first subregression at all timesteps, so that, without check, all w should switch to first subregression
  }

  function(0).setVal(t, x, w);
  function(0).computeVandermonde(nCoeff);

  std::set<Index> setInd;
  setInd.insert(0);

  FunctionalClass fc(function,
                     setInd,
                     0.95);
  fc.setSize(nSub, nCoeff);
  fc.setParam(alpha, beta, sd);

  function(0).sampleWCheck(alpha,
                           beta,
                           sd,
                           fc);

  ASSERT_EQ(nCoeff, function(0).w()(1).size());
}
