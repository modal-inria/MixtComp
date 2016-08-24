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

FunctionalClass::FunctionalClass(Vector<Function>& data,
                                 const std::set<Index>& setInd,
                                 Real confidenceLevel) :
    nSub_(0),
    nCoeff_(0),
    data_(data),
    setInd_(setInd),
    alphaParamStat_(alpha_, confidenceLevel),
    betaParamStat_ (beta_ , confidenceLevel),
    sdParamStat_   (sd_   , confidenceLevel) {}

void FunctionalClass::setSize(Index nSub,
                              Index nCoeff) {
  nSub_ = nSub;
  nCoeff_ = nCoeff;

  alpha_.resize(nSub, 2); // remember that storage is linear for alpha, two coefficients per subregression
  beta_.resize(nSub, nCoeff);
  sd_.resize(nSub);

  alpha_ = 0.; // initialization is mandatory as the optimization is an iterative process
  beta_ = 0.;
  sd_ = 0.;
}

void FunctionalClass::mStep() {
  mStepAlpha();
  mStepBetaSd();
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
  opt = nlopt_create(NLOPT_LD_LBFGS, nParam); // algorithm and dimensionality
  nlopt_set_max_objective(opt, optiFunctionalClass, this); // cost and grad function, data for the function
  nlopt_optimize(opt, alpha, &minf); // launch the effective optimization run
  nlopt_destroy(opt);

  for (Index s = 0; s < nSub; ++s) {
    alpha_(s, 0) = alpha[2 * s    ];
    alpha_(s, 1) = alpha[2 * s + 1];
  }
}

void FunctionalClass::mStepBetaSd() {
  Vector<Index> nTTotal(nSub_, 0);

  for (std::set<Index>::const_iterator itData  = setInd_.begin(),
                                       itDataE = setInd_.end();
       itData != itDataE;
       ++itData) { // to create the complete design matrix and y for the class, the total number of timesteps over the class must be determined
    for (Index s = 0; s < nSub_; ++s) {
      nTTotal(s) += data_(*itData).w()(s).size();
    }
  }

  Vector<Matrix<Real> > design(nSub_);
  Vector<Vector<Real> > y(nSub_);
  for (Index s = 0; s < nSub_; ++s) {
    design(s).resize(nTTotal(s), nCoeff_);
    y(s).resize(nTTotal(s));

    Index i = 0; // current row in the global design matrix
    for (std::set<Index>::const_iterator itData  = setInd_.begin(),
                                         itDataE = setInd_.end();
         itData != itDataE;
         ++itData) {
      for (std::set<Index>::const_iterator itTime  = data_(*itData).w()(s).begin(),
                                           itTimeE = data_(*itData).w()(s).end();
           itTime != itTimeE;
           ++itTime) {
        design(s).row(i) = data_(*itData).vandermonde().row(*itTime);
        y(s)(i) = data_(*itData).x()(*itTime);
        ++i;
      }
    }
  }

  subRegression(design,
                y,
                beta_,
                sd_);
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

Index FunctionalClass::checkSampleCondition(std::string* warnLog) const {
  if (warnLog == NULL) {
    if (!(checkNbDifferentValue() && checkNonNullSigma())) {
      return 0;
    }
  }
  else {
    std::stringstream sstm;
    bool value = checkNbDifferentValue();
    bool sd = checkNonNullSigma();

    if (!value) {
      sstm << "Not enough different values for t. Is your data sampled at enough different timesteps ?" << std::endl;
    }

    if (!sd) {
      sstm << "Not enough different values for t. Is your data sampled at enough different timesteps ?" << std::endl;
    }

    if (!(value && sd)) {
      *warnLog += sstm.str();
      return 0;
    }
  }

  return 1;
}

bool FunctionalClass::checkNbDifferentValue() const {
  for (Index s = 0; s < nSub_; ++s) {
    std::set<Real> values;
    for (std::set<Index>::const_iterator it = setInd_.begin(), itE = setInd_.end();
         it != itE;
         ++it) { // only loop on individuals in the current class
      for (std::set<Index>::const_iterator itW = data_(*it).w()(s).begin(), itWE = data_(*it).w()(s).end();
          itW != itWE;
           ++itW) { // only loop on timesteps in the current subregression
        values.insert(data_(*it).t()(*itW));
        if (nCoeff_ <= values.size()) { // this sub-regression is valid and has enough time steps
          goto endIt;
        }
      }
    }

    return false;

    endIt:;
  }

  return true;
}

bool FunctionalClass::checkNonNullSigma() const {
  for (Index s = 0; s < nSub_; ++s) {
    for (std::set<Index>::const_iterator it = setInd_.begin(), itE = setInd_.end();
         it != itE;
         ++it) { // only loop on individuals in the current class
      for (std::set<Index>::const_iterator itW = data_(*it).w()(s).begin(), itWE = data_(*it).w()(s).end();
          itW != itWE;
           ++itW) { // only loop on timesteps in the current subregression
        if (data_(*it).vandermonde().row(*itW).dot(beta_.row(s)) != data_(*it).x()(*itW)) { // this subregression has enough variability to be valid
          goto endIt;
        }
      }
    }

    return false;

    endIt:;
  }

  return true;
}

void FunctionalClass::samplingStepNoCheck(Index i) {
  data_(i).sampleWNoCheck(alpha_,
                          beta_,
                          sd_);
}

void FunctionalClass::samplingStepCheck(Index i) {
  Vector<Index> nTot(nSub_, 0); // the total number of times for each subregression, over all individuals. Not that this computation could be cached to be more efficient.
  for (std::set<Index>::const_iterator it = setInd_.begin(), itE = setInd_.end();
       it != itE;
       ++it) { // only loop on individuals in the same class
    for (Index s = 0; s < nSub_; ++s) {
      nTot(s) += data_(*it).w()(s).size();
    }
  }

  data_(i).sampleWCheck(alpha_,
                        beta_,
                        sd_,
                        nTot);
}
} // namespace mixt
