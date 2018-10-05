/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "NegativeBinomial.h"
#include "../../../IO/mixt_IO.h"
#include "../../../Various/mixt_Constants.h"
#include "../../../Various/mixt_Enum.h"
#include "../../../LinAlg/mixt_Math.h"
#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/trigamma.hpp>
#include <boost/exception/diagnostic_information.hpp>

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
	at(3) = false; // missingIntervals_,
	at(4) = false; // missingLUIntervals_,
	at(5) = false; // missingRUIntervals_,
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
		for (std::set<Index>::const_iterator it = classInd(k).begin(), itEnd =
				classInd(k).end(); it != itEnd; ++it) {
			x(currObsInClass) = (*p_data_)(*it);
			++currObsInClass;
		}

		try {
			Real nParam = estimateN(x, param_(2 * k)); // starting point is the current value of the parameter
			Real pParam = estimateP(x, nParam);

			param_(2 * k) = nParam;
			param_(2 * k + 1) = pParam;

			if ((1 - pParam  < epsilon) | (pParam < epsilon)) {
				warnLog +=
						"Negative Binomial variables must have a p value different from 0 or 1 in each class. It is not the case in class: "
						+ std::to_string(k) + ". " + eol;
			}
		}catch(boost::exception &e){
			warnLog += "Negative Binomial model, parameter n divergence in class: "
					+ std::to_string(k)+ ". "+boost::diagnostic_information(e) + "."+ eol;
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
	std::function<std::pair<Real, Real>(const Vector<int>&, Real)>  f = std::bind(&NegativeBinomial::evalFuncDeriv, this, std::placeholders::_1, std::placeholders::_2);
	return positiveNewtonRaphson<int>(x, n0, maxIterationOptim, f);
}

Real NegativeBinomial::estimateP(const Vector<int>& x, Real n) const {
	Index nObs = x.size();
	Real sumx = x.sum();
	Real nNobs = n * nObs;

	return(nNobs / (nNobs + sumx));
}


std::vector<std::string> NegativeBinomial::paramNames() const {
	std::vector<std::string> names(2 * nClass_);
	for (int k = 0; k < nClass_; ++k) {
		std::stringstream sstmN, sstmP;
		sstmN << "k: " << k + minModality << ", n: ";
		sstmP << "k: " << k + minModality << ", p";
		names[2 * k] = sstmN.str();
		names[2 * k + 1] = sstmP.str();
	}
	return names;
}

std::string NegativeBinomial::setData(const std::string& paramStr,
		AugmentedData<Vector<int> >& augData, RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	if (augData.dataRange_.min_ < 0) {
		std::stringstream sstm;
		sstm << "Variable: " << idName_
				<< " requires a minimum value of 0 in either provided values or bounds. "
				<< "The minimum value currently provided is : "
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

	std::cout << sstm.str() << std::endl;
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
