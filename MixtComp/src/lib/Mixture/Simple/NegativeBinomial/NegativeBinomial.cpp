/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include <functional>

#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/trigamma.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <IO/IO.h>
#include <LinAlg/Maths.h>

#include <Various/Enum.h>
#include <Various/Constants.h>

#include "NegativeBinomial.h"

namespace mixt {

const std::string NegativeBinomial::name = "NegativeBinomial";

NegativeBinomial::NegativeBinomial(const std::string& idName, int nbClass, Vector<Real>& param) :
		idName_(idName), nClass_(nbClass), param_(param), p_data_(0) {
	param_.resize(2 * nbClass);
}

Vector<bool> NegativeBinomial::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true; // present_,
	at(1) = true; // missing_,
	at(2) = false; // missingFiniteValues_,
	at(3) = true; // missingIntervals_,
	at(4) = false; // missingLUIntervals_,
	at(5) = true; // missingRUIntervals_,
	return at;
}

int NegativeBinomial::computeNbFreeParameters() const {
	return 2 * nClass_;
}

bool NegativeBinomial::hasModalities() const {
	return false;
}

std::string NegativeBinomial::mStep(const Vector<std::set<Index>>& classInd) {

	std::string warnLog;
	for (Index k = 0; k < nClass_; ++k) {
		Vector<int> x(classInd(k).size()); // the optimizer needs a particular format for the data
		Index currObsInClass = 0;
		for (std::set<Index>::const_iterator it = classInd(k).begin(), itEnd = classInd(k).end(); it != itEnd; ++it) {
			x(currObsInClass) = (*p_data_)(*it);
			++currObsInClass;
		}

		try {
			Real nParam = estimateN(x, param_(2 * k)); // starting point is the current value of the parameter
			Real pParam = estimateP(x, nParam);

			param_(2 * k) = nParam;
			param_(2 * k + 1) = pParam;

			if ((1 - pParam < epsilon) | (pParam < epsilon)) {
				warnLog += "NegativeBinomial variables must have a p value different from 0 or 1 in each class. It is not the case in class: " + std::to_string(k) + ". " + eol;
			}
		} catch (boost::exception &e) {
			warnLog += "NegativeBinomial model, parameter n divergence in class: " + std::to_string(k) + ". " + boost::diagnostic_information(e) + "." + eol;
		}

	}

	return warnLog;
}

std::pair<Real, Real> NegativeBinomial::evalFuncDeriv(const Vector<int>& x, Real n) const {
	Index nObs = x.size();

	Real nNobs = n * nObs;

	Vector<Real> dignx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		dignx(i) = boost::math::digamma(n + x(i));
	}

	Vector<Real> trignx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		trignx(i) = boost::math::trigamma(n + x(i));
	}

	Real dign = boost::math::digamma(n);
	Real trign = boost::math::trigamma(n);

	Real sumx = x.sum();
	Real sumdignx = dignx.sum();
	Real sumtrignx = trignx.sum();

	Real f = sumdignx - nObs * dign + nObs * std::log(nNobs) - nObs * std::log(nNobs + sumx);
	Real df = sumtrignx - nObs * trign + nObs / n - nObs * nObs / (nNobs + sumx);

	return std::pair<Real, Real>(f, df);
}

Real NegativeBinomial::estimateN(const Vector<int>& x, Real n0) const {
	std::function<std::pair<Real, Real>(Real)> f = std::bind(&NegativeBinomial::evalFuncDeriv, this, x, std::placeholders::_1);
	return minimizePositiveNewtonRaphson(maxIterationOptim, relTolOptim, f, n0);
}

Real NegativeBinomial::estimateP(const Vector<int>& x, Real n) const {
	Index nObs = x.size();
	Real sumx = x.sum();
	Real nNobs = n * nObs;

	return (nNobs / (nNobs + sumx));
}

std::vector<std::string> NegativeBinomial::paramNames() const {
	std::vector<std::string> names(2 * nClass_);
	for (int k = 0; k < nClass_; ++k) {
		std::stringstream sstmN, sstmP;
		sstmN << "k: " << k + minModality << ", n";
		sstmP << "k: " << k + minModality << ", p";
		names[2 * k] = sstmN.str();
		names[2 * k + 1] = sstmP.str();
	}
	return names;
}

std::string NegativeBinomial::setData(const std::string& paramStr, AugmentedData<Vector<int> >& augData, RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	if (augData.dataRange_.min_ < 0) {
		std::stringstream sstm;
		sstm << "Variable: " << idName_ << " requires a minimum value of 0 in either provided values or bounds. " << "The minimum value currently provided is : "
				<< augData.dataRange_.min_ + minModality << std::endl;
		warnLog += sstm.str();
	}

	return warnLog;
}

void NegativeBinomial::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tn: " << param_[2 * k] << std::endl;
		sstm << "\tp: " << param_[2 * k + 1] << std::endl;
	}

#ifdef MC_VERBOSE
	std::cout << sstm.str() << std::endl;
#endif
}

std::string NegativeBinomial::checkSampleCondition(const Vector<std::set<Index>>& classInd) const {

	return "";
}

void NegativeBinomial::initParam() {
	for (Index k = 0; k < nClass_; ++k) { // initialize with an arbitrary, valid starting point
		param_(2 * k) = 5.2;
		param_(2 * k + 1) = 0.7;
	}
}

} // namespace mixt
