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
  at(0) = true ; // present_,
  at(1) = true ; // missing_,
  at(2) = false; // missingFiniteValues_,
  at(3) = false; // missingIntervals_,
  at(4) = false; // missingLUIntervals_,
  at(5) = true ; // missingRUIntervals_,
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

void mStep() {

}

}
