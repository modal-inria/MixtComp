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

  Vector<std::list<Index> > w(nSub);
  Vector<Real> x(nTime, 0.);
  Function function;

  MultinomialStatistic multi;
  NormalStatistic normal;

  for (Index i = 0; i < nTime; ++i) {
    w(multi.sampleInt(0, nSub - 1)).push_back(i); // initial subregression is chosen at random
    for (Index p = 0; p < nCoeff; ++p) {
      x(i) += beta(0, p) * pow(t(i), p); // but data is generated using the first subregression at all timesteps
    }
    x(i) += normal.sample(0., sd(0));
  }

  function.setVal(t, x, w);
  function.computeVandermonde(nCoeff);

  Vector<Index> nTot(nSub, 0); // the total number of times for each subregression, over all individuals. Not that this computation could be cached to be more efficient.
  for (Index s = 0; s < nSub; ++s) {
    nTot(s) += function.w()(s).size();
  }

  function.sampleWCheck(alpha,
                        beta,
                        sd,
                        nTot);

  ASSERT_EQ(nSub, function.w()(1).size());
}
