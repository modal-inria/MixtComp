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
    setInd_(setInd),
    alphaParamStat_(alpha_, confidenceLevel),
    betaParamStat_ (beta_ , confidenceLevel),
    sdParamStat_   (sd_   , confidenceLevel) {}

void FunctionalClass::setSize(Index nSub,
                              Index nCoeff) {
  alpha_.resize(nSub, 2); // remember that storage is linear for alpha, two coefficients per subregression
  beta_.resize(nSub, nCoeff);
  sd_.resize(nSub);

  alpha_ = 0.; // initialization is mandatory as the optimization is an iterative process
  beta_ = 0.;
  sd_ = 0.;
}

void FunctionalClass::mStep() {
  mStepAlpha();
  mStepBetaEpsilon();
}

void FunctionalClass::mStepAlpha() {
  Index nSub   = alpha_.rows();
  Index nParam = 2 * nSub;
  double minf;

  double alpha[nParam]; // linear version of alpha, to conform to nlopt argument format
  for (Index s = 0; s < nSub; ++s) {
    alpha[2 * s    ] = alpha_(s, 0);
    alpha[2 * s + 1] = alpha_(s, 1);
  }

  nlopt_opt opt;
  opt = nlopt_create(NLOPT_LD_LBFGS, nParam); /* algorithm and dimensionality */
  nlopt_set_max_objective(opt, optiFunctionalClass, this);
  nlopt_optimize(opt, alpha, &minf);
  nlopt_destroy(opt);

  for (Index s = 0; s < nSub; ++s) {
    alpha_(s, 0) = alpha[2 * s    ];
    alpha_(s, 1) = alpha[2 * s + 1];
  }
}

void FunctionalClass::mStepBetaEpsilon() {
  // to create the complete design matrix and y for the class, the total number of timesteps over the class must be determined
  // first create the design matrix by concatenating the design for the individuals in the class, by block-copying the relevant data
  // same applies for y

//  subRegression(design,
//                y,
//                const Vector<std::list<Index> >& w,
//                Matrix<Real>& beta,
//                Vector<Real>& sd)
}

void FunctionalClass::setParamStorage() {
  alphaParamStat_.setParamStorage();
  betaParamStat_ .setParamStorage();
  sdParamStat_   .setParamStorage();
}

void FunctionalClass::setExpectationParam() {
  alphaParamStat_.setExpectationParam();
  betaParamStat_ .setExpectationParam();
  sdParamStat_   .setExpectationParam();
}

void FunctionalClass::sampleParam(Index iteration,
                                  Index iterationMax) {
  alphaParamStat_.sampleParam(iteration, iterationMax);
  betaParamStat_ .sampleParam(iteration, iterationMax);
  sdParamStat_   .sampleParam(iteration, iterationMax);
}

double FunctionalClass::costAndGrad(Index nParam,
                                    const double* alpha,
                                    double* grad) {
  Real cost = 0.;
  double gradInd[nParam];

  for (Index p = 0; p < nParam; ++p) {
    grad[p] = 0.;
  }

  for (std::set<Index>::const_iterator it  = setInd_.begin(),
                                       itE = setInd_.end();
       it != itE;
       ++it) { // each individual in current class adds a contribution to the gradient of alpha
    cost += data_(*it).costAndGrad(nParam,
                                   alpha,
                                   gradInd);
    for (Index p = 0; p < nParam; ++p) {
      grad[p] += gradInd[p];
    }
  }

  return cost;
}

} // namespace mixt
