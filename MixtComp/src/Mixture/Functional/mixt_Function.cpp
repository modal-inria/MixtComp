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
#include "mixt_FunctionalClass.h"

namespace mixt {

void Function::setSize(Index nT,
                       Index nSub) {
  nTime_ = nT;
  nSub_ = nSub;
  t_.resize(nT);
  x_.resize(nT);
  w_.resize(nSub);
}

void Function::setVal(const Vector<Real>& t,
                      const Vector<Real>& x,
                      const Vector<std::set<Index> >& w) {
  nTime_ = t.size();
  nSub_ = w.size();
  t_ = t;
  x_ = x;
  w_ = w;
}

void Function::setValTime(Index i,
                          Real t,
                          Real x) {
  t_(i) = t;
  x_(i) = x;
}

void Function::computeVandermonde(Index nCoeff) {
  vandermondeMatrix(t_, nCoeff, vandermonde_);
}

void Function::computeJointLogProba(const Matrix<Real>& alpha,
                                    const Matrix<Real>& beta,
                                    const Vector<Real>& sd,
                                    Matrix<Real>& jointLogProba) const {
  jointLogProba.resize(nTime_, nSub_);
  NormalStatistic normal;
  Vector<Real> currKappa(nSub_);
  for (Index i = 0; i < nTime_; ++i) {
    kappaMatrix(t_(i), alpha, currKappa);

    for (Index s = 0; s < nSub_; ++s) {
      Real currExpectation = vandermonde_.row(i).dot(beta.row(s)); // since the completed probability is computed, only the current subregression is taken into account in the computation
      Real logAPosteriori = normal.lpdf(x_(i), currExpectation, sd(s));

      Real logAPriori = std::log(currKappa(s));

      jointLogProba(i, s) = logAPriori + logAPosteriori;
    }
  }
}

Real Function::lnCompletedProbability(const Matrix<Real>& alpha,
                                      const Matrix<Real>& beta,
                                      const Vector<Real>& sd) const {
  Real logProba = 0.;

  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);
  for (Index s = 0; s < nSub_; ++s) {
    for (std::set<Index>::const_iterator it  = w_(s).begin(),
                                         itE = w_(s).end();
         it != itE;
         ++it) {
      logProba += jointLogProba(*it, s); // only the completed value of w is taken into account
    }
  }

  return logProba;
}

Real Function::lnObservedProbability(const Matrix<Real>& alpha,
                                     const Matrix<Real>& beta,
                                     const Vector<Real>& sd) const {
  Real logProba = 0.;

  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);
  for (Index i = 0; i < nTime_; ++i) {
    Vector<Real> dummy;
    logProba += dummy.logToMulti(jointLogProba.row(i)); // every possible value of w is taken into account, to marginalize over w
  }

  return logProba;
}

void Function::sampleWNoCheck(const Matrix<Real>& alpha,
                              const Matrix<Real>& beta,
                              const Vector<Real>& sd) {
  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);

  for (Index s = 0; s < nSub_; ++s) {
    w_(s).clear();
  }

  Vector<Real> currProba;
  for (Index i = 0; i < nTime_; ++i) {
    currProba.logToMulti(jointLogProba.row(i));
    w_(multi_.sample(currProba)).insert(i);
  }
}

void Function::sampleWCheck(const Matrix<Real>& alpha,
                            const Matrix<Real>& beta,
                            const Vector<Real>& sd,
                            Vector<Index>& wTot) {
  Matrix<Real> jointLogProba;
  computeJointLogProba(alpha, beta, sd, jointLogProba);

  Vector<Index> w0(nTime_, 0); // initial subregression of each timestep for current observation
  for (Index s = 0; s < nSub_; ++s) {
    for (std::list<Index>::const_iterator it = w_(s).begin(), itE = w_(s).end();
         it != itE;
         ++it) {
      w0(*it) = s;
    }
  }

  for (Index s = 0; s < nSub_; ++s) {
    w_(s).clear();
  }

  Vector<Real> currProba;
  Index currW;
  for (Index i = 0; i < nTime_; ++i) {
    wTot(w0(i)) -= 1;

    if (wTot(w0(i)) < nSub_) { // conditions for regression will not be met. Do nothing and keep current timestep in the same subregression
      w_(w0(i)).push_back(i);
      wTot(w0(i)) += 1;
    }
    else {
      currProba.logToMulti(jointLogProba.row(i));
      currW = multi_.sample(currProba);
      w_(currW).push_back(i);
      wTot(currW) += 1;
    }
  }
}

void Function::removeMissingUniformSampling() {
  for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
    w_(s).clear();
  }

  for (Index i = 0; i < nTime_; ++i) {
    w_(multi_.sampleInt(0, nSub_ - 1)).insert(i); // w follows a uniform discrete law
  }
}

void Function::removeMissingQuantile(const Vector<Real>& quantiles) {
  Index s;
  for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
    w_(s).clear();
  }

  for (Index i = 0; i < nTime_; ++i) {
    for (s = 0; s < nSub_ - 1; ++s) {
      if (t_(i) < quantiles(s)) {
        break;
      }
    }
    w_(s).insert(i);
  }
}

double Function::costAndGrad(unsigned nParam,
                             const double* alpha,
                             double* grad) const {
  double cost;
  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;

  timeValue(t_,
            nParam,
            alpha,
            logValue,
            logSumExpValue);

  costFunction(t_,
               logValue,
               logSumExpValue,
               w_,
               cost);

  if (grad != NULL) {
    gradCostFunction(t_,
                     logValue,
                     logSumExpValue,
                     w_,
                     grad);
  }

  return cost;
}

void Function::printProp() const {
  Vector<Real> prop(nSub_, 0.);

  for (Index s = 0; s < nSub_; ++s) {
    prop(s) = w_(s).size();
  }

  prop = prop / prop.sum();

  std::cout << "Prop of w: " << itString(prop) << std::endl;
}

} // namespace mixt
