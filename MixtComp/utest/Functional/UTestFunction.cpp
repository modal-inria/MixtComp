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
 *  Created on: June 3, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(Functional, lnCompletedProbability) {
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
    Vector<std::list<Index> > w(nSub);
    Vector<Real> x(nTime, 0.);
    Function function;

    for (Index i = 0; i < nTime; ++i) {
      Index currSub = multi.sample(kappa.row(i));
      w(currSub).push_back(i);
      for (Index p = 0; p < nCoeff; ++p) {
        x(i) += beta(currSub, p) * pow(t(i), p);
      }
      x(i) += normal.sample(0., sd(currSub));
    }

    function.setVal(t, x, w);
    function.computeVandermonde(nCoeff);
    function.computeJointLogProba(alpha, beta, sd);

    Real logProba = function.lnCompletedProbability();
    if (mode < logProba) {
      mode = logProba;
    }
  }

  Real expectedMode = nTime * (normal.lpdf(0., 0., sdVar)) + logKappaMaxSum;
  ASSERT_GT(expectedMode, mode);
  ASSERT_NEAR(expectedMode, mode, 0.05);
}
