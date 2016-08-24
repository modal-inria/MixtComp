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
    Vector<std::list<Index> > w(nSub);

    for (Index currT = 0; currT < nTime; ++currT) {
      t(currT) = 12.; // all time values identical: that should be enough to trigger the error detection
      x(currT) = uni.sample(xMin, xMax);
      w(multi.sampleInt(0, nSub - 1)).push_back(currT);
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
    Vector<std::list<Index> > w(nSub);
    Matrix<Real> vandermonde;

    for (Index currT = 0; currT < nTime; ++currT) {
      t(currT) = uni.sample(tMin, tMax);
    }
    vandermondeMatrix(t, nCoeff, vandermonde);

    for (Index currT = 0; currT < nTime; ++currT) {
      Index s = multi.sampleInt(0, nSub - 1);
      w(s).push_back(currT);
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
