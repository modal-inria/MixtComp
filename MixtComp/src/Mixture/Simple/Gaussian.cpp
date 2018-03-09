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

Gaussian::Gaussian(
		const std::string& idName,
		int nbClass,
		Vector<Real>& param)
: idName_(idName),
  nClass_(nbClass),
  param_(param),
  p_data_(0) {
	param_.resize(2 * nbClass);
}

Vector<bool> Gaussian::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true ; // present_,
	at(1) = true ; // missing_,
	at(2) = false; // missingFiniteValues_,
	at(3) = true ; // missingIntervals_,
	at(4) = true ; // missingLUIntervals_,
	at(5) = true ; // missingRUIntervals_,
	return at;
}

int Gaussian::computeNbFreeParameters() const {
	return 2 * nClass_;
}

bool Gaussian::hasModalities() const {
	return false;
}

void Gaussian::mStep(const Vector<std::set<Index>>& classInd) {
	for (int k = 0; k < nClass_; ++k) {
		Real mean;
		Real sd;

		meanSD(classInd(k),
				*p_data_,
				mean,
				sd);

		param_(2 * k    ) = mean;
		param_(2 * k + 1) = sd;
	}
}

std::vector<std::string> Gaussian::paramNames() const {
	std::vector<std::string> names(nClass_ * 2);
	for (int k = 0; k < nClass_; ++k) {
		std::stringstream sstmMean, sstmSd;
		sstmMean << "k: "
				<< k + minModality
				<< ", mean: ";
		sstmSd << "k: "
				<< k + minModality
				<< ", sd";
		names[2 * k    ] = sstmMean.str();
		names[2 * k + 1] = sstmSd  .str();
	}
	return names;
}

std::string Gaussian::setData(
		const std::string& paramStr,
		AugmentedData<Vector<Real> >& augData,
		RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	return warnLog;
}

void Gaussian::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tmean: " << param_[2 * k    ] << std::endl;
		sstm << "\tsd: "   << param_[2 * k + 1] << std::endl;
	}

	std::cout << sstm.str() << std::endl;
}

std::string Gaussian::checkSampleCondition(const Vector<std::set<Index>>& classInd) const {
	for (Index k = 0; k < nClass_; ++k) {
		if (classInd(k).size() < 2) {
			return "Gaussian variables must have at least two individuals per class. This is not the case for at least one class. You can check whether you have enough individuals regarding the number of classes." + eol;
		}

		std::set<Index>::const_iterator it = classInd(k).begin(), itE = classInd(k).end();
		Real previousElemClass = (*p_data_)(*it);
		Real eps = (*p_data_)(*it) * epsilon;
		++it;
		for (; it != itE; ++it) {
			if (eps < (std::fabs((*p_data_)(*it) - previousElemClass))) { // stop checking soon as there are two different values in the current class
				goto endItK; // goto used as a kind of super break statement, see http://stackoverflow.com/questions/1257744/can-i-use-break-to-exit-multiple-nested-for-loops
			}
		}

		return "Gaussian variables must have a minimum relative variability of 1e-8 in each class. It seems that at least one class only contains values close to: "
				+ std::to_string(previousElemClass) + ". If some values are repeated often in this variable, maybe a Categorical or a Poisson variable will describe it better." + eol;

		endItK:;
	}

	return "";
}

void Gaussian::initParam() {}

} // namespace mixt
