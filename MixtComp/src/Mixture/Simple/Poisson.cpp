/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/Simple/Poisson.h>
#include "../../IO/mixt_IO.h"
#include "../../Various/mixt_Constants.h"
#include "../../Various/mixt_Enum.h"

namespace mixt {

Poisson::Poisson(
		const std::string& idName,
		int nbClass,
		Vector<Real>& param,
		const Vector<std::set<Index> >& classInd)
: idName_(idName),
  nClass_(nbClass),
  param_(param),
  p_data_(0),
  classInd_(classInd) {
	param_.resize(nbClass);
}

Vector<bool> Poisson::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true; // present_,
	at(1) = true;// missing_,
	at(2) = false;// missingFiniteValues_,
	at(3) = false;// missingIntervals_,
	at(4) = false;// missingLUIntervals_,
	at(5) = false;// missingRUIntervals_,
	return at;
}

int Poisson::computeNbFreeParameters() const {
	return nClass_;
}

bool Poisson::hasModalities() const {
	return false;
}

void Poisson::mStep() {
	for (int k = 0; k < nClass_; ++k) {
		Real sumClass = 0.;
		for (std::set<Index>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
				it != itE;
				++it) {
			sumClass += (*p_data_)(*it);
		}

		param_(k) = sumClass / Real(classInd_(k).size());
	}
}

std::vector<std::string> Poisson::paramNames() const {
	std::vector<std::string> names(nClass_);
	for (int k = 0; k < nClass_; ++k) {
		std::stringstream sstm;
		sstm << "k: "
				<< k + minModality
				<< ", lambda";
		names[k] = sstm.str();
	}
	return names;
}

std::string Poisson::setData(
		const std::string& paramStr,
		AugmentedData<Vector<int> >& augData,
		RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	if (augData.dataRange_.min_ < 0) {
		std::stringstream sstm;
		sstm << "Variable: " << idName_ << " requires a minimum value of : " << minModality << " in either provided values or bounds. "
				<< "The minimum value currently provided is : " << augData.dataRange_.min_ + minModality << std::endl;
		warnLog += sstm.str();
	}

	return warnLog;
}

void Poisson::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tlambda: " << param_[k] << std::endl;
	}

	std::cout << sstm.str() << std::endl;
}

std::string Poisson::checkSampleCondition() const {
	if (degeneracyAuthorizedForNonBoundedLikelihood) return "";

	for (Index k = 0; k < nClass_; ++k) {
		for (std::set<Index>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
				it != itE;
				++it) {
			if ((*p_data_)(*it) > 0) {
				goto endItK;
			}
		}

		return "Poisson variables must have at least one non-zero individual per class. Class: " + std::to_string(k) + " only contains the 0 modality. If your data has too many individuals with a value of 0, a Poisson model can not describe it." + eol;

		endItK:;
	}

	return "";
}

std::string Poisson::initParam(const Vector<Index>& initObs) {
	for (Index k = 0; k < nClass_; ++k) {
		param_(k) = Real((*p_data_)(initObs(k)));
	}

	std::stringstream sstm;
	if (param_.minCoeff() == 0.0) {
		sstm << "Too many observations equal to 0. The Poisson distribution will only model a variable that has enough values not equal to 0." << std::endl;
	}

	return sstm.str();
};

std::vector<bool> Poisson::parametersInInterior() {
	std::vector<bool> res(nClass_);
	for (Index k = 0; k < nClass_; ++k) {
		res[k] = (param_(k) == 0.0) ? false : true;
	}
	return res;
}

} // namespace mixt
