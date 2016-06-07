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
 *  Created on: June 1, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Function.h"

namespace mixt {

void Function::setVal(const Vector<Real>& t,
                      const Vector<Real>& x,
                      const Vector<std::list<Index> >& w) {
  nTime_ = t.size();
  nSub_ = w.size();
  t_ = t;
  x_ = x;
  w_ = w;
}

void Function::computeVandermonde(Index nCoeff) {
  vandermondeMatrix(t_, nCoeff, vandermonde_);
}

void Function::computeJointLogProba(const Matrix<Real>& alpha,
                                    const Matrix<Real>& beta,
                                    const Vector<Real>& sd) {
  jointLogProba_.resize(nTime_, nSub_);
  NormalStatistic normal;
  Vector<Real> currKappa(nSub_);
  for (Index i = 0; i < nTime_; ++i) {
    kappaMatrix(t_(i), alpha, currKappa);

    for (Index s = 0; s < nSub_; ++s) {
      Real currExpectation = vandermonde_.row(i).dot(beta.row(s)); // since the completed probability is computed, only the current subregression is taken into account in the computation
      Real logAPosteriori = normal.lpdf(x_(i), currExpectation, sd(s));

      Real logAPriori = std::log(currKappa(s));

      jointLogProba_(i, s) = logAPriori + logAPosteriori;
    }
  }
}

Real Function::lnCompletedProbability() {
  Real logProba = 0.;

  NormalStatistic normal;
  for (Index s = 0; s < nSub_; ++s) {
    for (std::list<Index>::const_iterator it  = w_(s).begin(),
                                          itE = w_(s).end();
         it != itE;
         ++it) {
      logProba += jointLogProba_(*it, s); // only the completed value of w is taken into account
    }
  }

  return logProba;
}

Real Function::lnObservedProbability() {
  Real logProba = 0.;

  NormalStatistic normal;
  for (Index i = 0; i < nTime_; ++i) {
    for (Index s = 0; s < nSub_; ++s) {
      logProba += jointLogProba_(i, s); // every possible value of w is taken into account
    }
  }

  return logProba;
}

} // namespace mixt
