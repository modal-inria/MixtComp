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
 *  Created on: June 7, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <nlopt.h>

#include "mixt_FunctionalClass.h"

namespace mixt {

FunctionalClass::FunctionalClass(const Vector<Function>& data,
                                 const std::set<Index>& setInd,
                                 Real confidenceLevel) :
    data_(data),
    setInd_(setInd) {}

void FunctionalClass::setSize(Index nSub,
                              Index nCoeff) {
  alpha_.resize(nSub * 2); // remember that storage is linear for alpha, two coefficients per subregression
  beta_.resize(nSub, nCoeff);
  sd_.resize(nSub);

  alpha_ = 0.; // initialization is mandatory as the optimization is an iterative process
  beta_ = 0.;
  sd_ = 0.;
}

void FunctionalClass::setParam(const Vector<Real>& alpha,
                               const Matrix<Real>& beta,
                               const Vector<Real>& sd) {
  alpha_ = alpha;
  beta_ = beta;
  sd_ = sd;
}

void FunctionalClass::mStep() {
  mStepAlpha();
  // mStepBetaEpsilon();
}

void FunctionalClass::mStepAlpha() {
  Index nParam = alpha_.size();
  double minf;

  nlopt_opt opt;
  opt = nlopt_create(NLOPT_LD_LBFGS, nParam); /* algorithm and dimensionality */
  nlopt_set_max_objective(opt, optiFunctionalClass, this);
  nlopt_optimize(opt, alpha_.data(), &minf);
  nlopt_destroy(opt);
}

double FunctionalClass::gradCost(Index nParam,
                                 const double* alpha,
                                 Vector<Real>& grad) {
  Real cost = 0.;
  Vector<Real> gradInd(nParam, 0.);

  for (Index i = 0; i < nParam; ++i) {
    alpha_(i) = alpha[i];
  }

  for (std::set<Index>::const_iterator it  = setInd_.begin(),
                                       itE = setInd_.end();
       it != itE;
       ++it) {
    cost += data_(*it).costAndGrad(nParam,
                                   alpha_.data(),
                                   gradInd.data());
    grad += gradInd;
  }

  return cost;
}

} // namespace mixt
