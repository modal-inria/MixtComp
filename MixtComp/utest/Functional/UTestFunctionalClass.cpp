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
  Real alphaSlope = 0.5;

  Real alpha0 = xMax / 2.;
  Index nParam = nSub * 2; // regression order for

  Vector<Real> alpha(nParam);
  alpha << alpha0 * alphaSlope, -alphaSlope, // alpha is linearized in a single vector, for easier looping
          -alpha0 * alphaSlope,  alphaSlope;

  Matrix<Real> beta(nSub, nCoeff + 1);
  beta.row(0) <<  0.,  1., 0.; // y =  x      + N(0, 1)
  beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)

  Vector<std::list<Index> > w(nSub);
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
    w(currW).push_back(i); // sample the subregression

    for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
      x(i) += beta(currW, p) * pow(t(i), p);
    }
    x(i) += normal.sample(0, beta(currW, nCoeff));
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

  ASSERT_EQ(true, alphaComputed.isApprox(alpha, 0.1));
}

TEST(FunctionalClass, optimOneclassMultipleInd) {
  Index nTime = 10000;
  Index nInd = 10;
  Index nSub = 2; // number of subregression in the generation / estimation phases
  Index nCoeff = 2; // order of each subregression
  Real xMax = 50.;
  Real alphaSlope = 0.5;

  Real alpha0 = xMax / 2.;
  Index nParam = nSub * 2; // regression order for

  Vector<Real> alpha(nParam);
  alpha << alpha0 * alphaSlope, -alphaSlope, // alpha is linearized in a single vector, for easier looping
          -alpha0 * alphaSlope,  alphaSlope;

  Matrix<Real> beta(nSub, nCoeff + 1);
  beta.row(0) <<  0.,  1., 0.; // y =  x      + N(0, 1)
  beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)

  Vector<Function> data(nInd);
  std::set<Index> setInd;

  for (Index ind = 0; ind < nInd; ++ind) {
    Vector<std::list<Index> > w(nSub);
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
      w(currW).push_back(i); // sample the subregression

      for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
        x(i) += beta(currW, p) * pow(t(i), p);
      }
      x(i) += normal.sample(0, beta(currW, nCoeff));
    }

    data(ind).setVal(t, x, w);
    setInd.insert(ind);
  }

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

  ASSERT_EQ(true, alphaComputed.isApprox(alpha, 0.1));
}
