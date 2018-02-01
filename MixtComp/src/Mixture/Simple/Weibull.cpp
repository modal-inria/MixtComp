/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 17th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>
#include <iostream>

#include "Various/mixt_Constants.h"
#include "Various/mixt_Enum.h"

#include "Weibull.h"

namespace mixt {

Weibull::Weibull(
    const std::string& idName,
    Index nbClass,
    Vector<Real>& param,
    const Vector<std::set<Index> >& classInd)
: idName_(idName),
  nClass_(nbClass),
  param_(param),
  p_data_(0),
  classInd_(classInd) {
  param_.resize(2 * nbClass);
}

std::pair<Real, Real> Weibull::evalFuncDeriv(const Vector<Real>& x, Real k) const {
	Index nObs = x.size();
	Vector<Real> xk(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xk(i) = std::pow(x(i), k);
	}

	Vector<Real> lnx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		lnx(i) = std::log(x(i));
	}

	Vector<Real> xklnx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xklnx(i) = xk(i) * lnx(i);
	}

	Vector<Real> xklnxlnx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xklnxlnx(i) = xklnx(i) * lnx(i);
	}

	Real sumxk = xk.sum();
	Real sumlnx = lnx.sum();
	Real sumxklnx = xklnx.sum();
	Real sumxklnxlnx = xklnxlnx.sum();

	Real f = sumxklnx / sumxk - 1.0 / k - 1 / Real(nObs) * sumlnx;
	Real df = (sumxklnxlnx * sumxk - sumxklnx * sumxklnx) / (sumxk * sumxk) + 1.0 / (k * k);

	return std::pair<Real, Real>(f, df);
}

Real Weibull::positiveNewtonRaphson(const Vector<Real>& x, Real currK, Real nIt) const {
	if (nIt < 0)
		return currK;
	else {
		std::pair<Real, Real> eval = evalFuncDeriv(x, currK);
		Real candidate = currK - eval.first / eval.second;
		if (0.0 < candidate)
			return positiveNewtonRaphson(x, candidate  , nIt - 1);
		else
			return positiveNewtonRaphson(x, currK / 2.0, nIt - 1);
	}
}

Real Weibull::estimateK(const Vector<Real>& x, Real k0) const {
  return positiveNewtonRaphson(x, k0, maxIterationOptim);
}

Real Weibull::estimateLambda(const Vector<Real>& x, Real k) const {
	Index nObs = x.size();

	Vector<Real> xk(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xk(i) = std::pow(x(i), k);
	}
	Real sumxk = xk.sum();

	return std::pow(1.0 / Real(nObs) * sumxk, 1.0 / k);
}

Vector<bool> Weibull::acceptedType() const {
  Vector<bool> at(nb_enum_MisType_);
  at(present_) = true;
  at(missing_) = true;
  at(missingFiniteValues_) = false;
  at(missingIntervals_) = false;
  at(missingLUIntervals_) = false;
  at(missingRUIntervals_) = true ;
  return at;
}

Index Weibull::computeNbFreeParameters() const {
  return 2 * nClass_;
}

bool Weibull::hasModalities() const {
  return false;
}

std::string Weibull::setData(
    const std::string& paramStr,
    AugmentedData<Vector<Real> >& augData,
    RunMode mode) {
  std::string warnLog;

  p_data_ = &(augData.data_);

  if (augData.dataRange_.min_ < 0.0) {
    std::stringstream sstm;
    sstm << "Variable: " << idName_ << " requires a minimum value of 0.0 in either provided values or bounds. "
         << "The minimum value currently provided is : " << augData.dataRange_.min_ + minModality << std::endl;
    warnLog += sstm.str();
  }

  return warnLog;
}

void Weibull::mStep() {
  for (Index k = 0; k < nClass_; ++k) {
    Vector<Real> x(classInd_(k).size()); // the optimizer needs a particular format for the data
    Index currObsInClass = 0;
    for (std::set<Index>::const_iterator it = classInd_(k).begin(), itEnd = classInd_(k).end(); it != itEnd; ++it) {
      x(currObsInClass) = *it;
      ++currObsInClass;
    }

    Real kParam = estimateK(x, param_(2 * k)); // starting point is the current value of the parameter
    Real lambda = estimateLambda(x, kParam);

    param_(2 * k) = kParam;
    param_(2 * k + 1) = lambda;
  }
}

std::vector<std::string> Weibull::paramNames() const {
  std::vector<std::string> names(nClass_ * 2);

  for (Index k = 0; k < nClass_; ++k) {
    std::stringstream sstmMean, sstmSd;
    sstmMean << "k: " << k + minModality << ", k: ";
    sstmSd   << "k: " << k + minModality << ", lambda";
    names[2 * k    ] = sstmMean.str();
    names[2 * k + 1] = sstmSd  .str();
  }

  return names;
}

void Weibull::writeParameters() const {
  std::stringstream sstm;
  for (int k = 0; k < nClass_; ++k) {
    sstm << "Class: " << k << std::endl;
    sstm << "\tk (shape): "      << param_[2 * k    ] << std::endl;
    sstm << "\tlambda (scale): " << param_[2 * k + 1] << std::endl;
  }

  std::cout << sstm.str() << std::endl;
}

std::string Weibull::checkSampleCondition() const {
//  if (degeneracyAuthorizedForNonBoundedLikelihood) return ""; // Weibull pdf is unbounded, so this line should be commented out

  for (Index k = 0; k < nClass_; ++k) {
    for (std::set<Index>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
        it != itE;
        ++it) {
      if ((*p_data_)(*it) > 0.0) {
        goto endItK;
      }
    }

    return "Weibull variables must have at least one non-zero individual per class. Class: " + std::to_string(k) + " only contains the 0.0 value. If your data has too many individuals with a value of 0.0, a Weibull model can not describe it." + eol;

    endItK:;
  }

  return "";
}

std::string Weibull::initParam(const Vector<Index>& initObs) {
  std::stringstream sstm;

  for (Index k = 0; k < nClass_; ++k) {
    Real median = (*p_data_)(initObs(k));

    param_(2 * k    ) = std::log(std::log(2.0)) / std::log(median);
    param_(2 * k + 1) = 1.0;

    if (median == 0.0) {
      sstm << "Class " << k << " representative observation is 0.0. Too many observations are close to 0 to initialize properly a Weibull model." << std::endl;
    }
  }

  return sstm.str();
}

std::vector<bool> Weibull::parametersInInterior() {
  std::vector<bool> res(nClass_);
  for (Index k = 0; k < nClass_; ++k) {
    bool condOnK      = (param_(2 * k    ) == 0.0) ? false : true;
    bool condOnLambda = (param_(2 * k + 1) == 0.0) ? false : true;

    res[k] = condOnK && condOnLambda;
  }
  return res;
}

}
