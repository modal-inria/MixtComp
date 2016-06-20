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
                                 Vector<Real>& alpha,
                                 Matrix<Real>& beta) :
    data_(data),
    setInd_(setInd),
    alpha_(alpha),
    beta_(beta) {}

void FunctionalClass::mStep() {
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
