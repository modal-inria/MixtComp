/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>

#include "Data/mixt_AugmentedData.h"
#include "LinAlg/mixt_Math.h"
#include "LinAlg/mixt_LinAlg.h"
#include "Various/mixt_Constants.h"
#include "IO/mixt_IO.h"
#include "Various/mixt_Enum.h"

#include "Gaussian.h"

namespace mixt {

const std::string Gaussian::name = "Gaussian";

Gaussian::Gaussian(const std::string& idName, int nbClass, Vector<Real>& param) :
		idName_(idName), nClass_(nbClass), param_(param), p_data_(0) {
	param_.resize(2 * nbClass);
}

Vector<bool> Gaussian::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true; // present_,
	at(1) = true; // missing_,
	at(2) = false; // missingFiniteValues_,
	at(3) = true; // missingIntervals_,
	at(4) = true; // missingLUIntervals_,
	at(5) = true; // missingRUIntervals_,
	return at;
}

int Gaussian::computeNbFreeParameters() const {
	return 2 * nClass_;
}

bool Gaussian::hasModalities() const {
	return false;
}

std::string Gaussian::mStep(const Vector<std::set<Index>>& classInd) {
	std::string warnLog;

	for (Index k = 0; k < nClass_; ++k) {
		Real mean;
		Real sd;

		meanSD(classInd(k), *p_data_, mean, sd);

		param_(2 * k) = mean;
		param_(2 * k + 1) = sd;

		if (sd < epsilon) {
			warnLog +=
					"Gaussian variables must have a minimum standard deviation of "
							+ epsilonStr
							+ " in each class. It is not the case in class: "
							+ std::to_string(k)
							+ ". If some values are repeated often in this variable, maybe a Categorical or a Poisson variable will describe it better."
							+ eol;
		}
	}

	return warnLog;
}

std::vector<std::string> Gaussian::paramNames() const {
	std::vector<std::string> names(nClass_ * 2);
	for (int k = 0; k < nClass_; ++k) {
		std::stringstream sstmMean, sstmSd;
		sstmMean << "k: " << k + minModality << ", mean: ";
		sstmSd << "k: " << k + minModality << ", sd";
		names[2 * k] = sstmMean.str();
		names[2 * k + 1] = sstmSd.str();
	}
	return names;
}

std::string Gaussian::setData(const std::string& paramStr,
		AugmentedData<Vector<Real> >& augData, RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	return warnLog;
}

void Gaussian::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tmean: " << param_[2 * k] << std::endl;
		sstm << "\tsd: " << param_[2 * k + 1] << std::endl;
	}

#ifdef MC_VERBOSE
	std::cout << sstm.str() << std::endl;
#endif
}

std::string Gaussian::checkSampleCondition(
		const Vector<std::set<Index>>& classInd) const {
	for (Index k = 0; k < nClass_; ++k) {
		if (classInd(k).size() < 2) {
			return "Gaussian variables must have at least two individuals per class. This is not the case for at least one class. You can check whether you have enough individuals regarding the number of classes."
					+ eol;
		}
	}

	return "";
}

void Gaussian::initParam() {
}

} // namespace mixt
