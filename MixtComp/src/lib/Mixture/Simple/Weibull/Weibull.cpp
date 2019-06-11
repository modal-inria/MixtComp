/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 17th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <LinAlg/Math.h>
#include <Various/Constants.h>
#include <cmath>
#include <iostream>

#include "Various/Enum.h"
#include "Weibull.h"

namespace mixt {

const std::string Weibull::name = "Weibull";

Weibull::Weibull(const std::string& idName, Index nbClass, Vector<Real>& param) :
		idName_(idName), nClass_(nbClass), param_(param), p_data_(NULL) {
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

Real Weibull::estimateK(const Vector<Real>& x, Real k0) const {
	std::function<std::pair<Real, Real>(Real)> f = std::bind(&Weibull::evalFuncDeriv, this, x, std::placeholders::_1);
	return minimizePositiveNewtonRaphson(maxIterationOptim, relTolOptim, f, k0);
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
	at(missingIntervals_) = true;
	at(missingLUIntervals_) = false;
	at(missingRUIntervals_) = true;
	return at;
}

Index Weibull::computeNbFreeParameters() const {
	return 2 * nClass_;
}

bool Weibull::hasModalities() const {
	return false;
}

std::string Weibull::setData(const std::string& paramStr, AugmentedData<Vector<Real> >& augData, RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	if (augData.dataRange_.min_ < 0.0) {
		std::stringstream sstm;
		sstm << "Variable: " << idName_ << " requires a minimum value of 0.0 in either provided values or bounds. " << "The minimum value currently provided is : "
				<< augData.dataRange_.min_ + minModality << std::endl;
		warnLog += sstm.str();
	}

	return warnLog;
}

std::string Weibull::mStep(const Vector<std::set<Index>>& classInd) {
	for (Index k = 0; k < nClass_; ++k) {
		Vector<Real> x(classInd(k).size()); // the optimizer needs a particular format for the data
		Index currObsInClass = 0;
		for (std::set<Index>::const_iterator it = classInd(k).begin(), itEnd = classInd(k).end(); it != itEnd; ++it) {
			x(currObsInClass) = (*p_data_)(*it);
			++currObsInClass;
		}

		Real kParam = estimateK(x, param_(2 * k)); // starting point is the current value of the parameter
		Real lambda = estimateLambda(x, kParam);

		param_(2 * k) = kParam;
		param_(2 * k + 1) = lambda;
	}

	return "";
}

std::vector<std::string> Weibull::paramNames() const {
	std::vector<std::string> names(nClass_ * 2);

	for (Index k = 0; k < nClass_; ++k) {
		std::stringstream sstmMean, sstmSd;
		sstmMean << "k: " << k + minModality << ", k: ";
		sstmSd << "k: " << k + minModality << ", lambda";
		names[2 * k] = sstmMean.str();
		names[2 * k + 1] = sstmSd.str();
	}

	return names;
}

void Weibull::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tk (shape): " << param_[2 * k] << std::endl;
		sstm << "\tlambda (scale): " << param_[2 * k + 1] << std::endl;
	}

#ifdef MC_VERBOSE
	std::cout << sstm.str() << std::endl;
#endif
}

std::string Weibull::checkSampleCondition(const Vector<std::set<Index>>& classInd) const {
//  if (degeneracyAuthorizedForNonBoundedLikelihood) return ""; // Weibull pdf is unbounded, so this line should be commented out

	for (Index k = 0; k < nClass_; ++k) {
		for (std::set<Index>::const_iterator it = classInd(k).begin(), itE = classInd(k).end(); it != itE; ++it) {
			if (epsilon < (*p_data_)(*it)) {
				goto endItK;
			}
		}

		return "Weibull variables must have at least one non-zero individual per class. Class: " + std::to_string(k)
				+ " only contains values inferior to 1e-8. If your data has too many individuals with a value of 0.0, a Weibull model can not describe it." + eol;

		endItK: ;
	}

	return "";
}

void Weibull::initParam() {
	for (Index k = 0; k < nClass_; ++k) { // initialize with an arbitrary, valid starting point
		param_(2 * k) = 5.;
		param_(2 * k + 1) = 1.;
	}
}

}
