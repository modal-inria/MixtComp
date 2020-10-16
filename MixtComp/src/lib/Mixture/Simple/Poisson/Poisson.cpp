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
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Poisson.h"
#include "../../../IO/IO.h"
#include "../../../Various/Constants.h"
#include "../../../Various/Enum.h"

namespace mixt {

const std::string Poisson::name = "Poisson";

Poisson::Poisson(const std::string& idName, int nbClass, Vector<Real>& param) :
		idName_(idName), nClass_(nbClass), param_(param), p_data_(0) {
	param_.resize(nbClass);
}

Vector<bool> Poisson::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true; // present_,
	at(1) = true; // missing_,
	at(2) = false; // missingFiniteValues_,
	at(3) = true; // missingIntervals_,
	at(4) = false; // missingLUIntervals_,
	at(5) = true; // missingRUIntervals_,
	return at;
}

int Poisson::computeNbFreeParameters() const {
	return nClass_;
}

bool Poisson::hasModalities() const {
	return false;
}

std::string Poisson::mStep(const Vector<std::set<Index>>& classInd) {
	std::string warnLog;

	for (int k = 0; k < nClass_; ++k) {
		Real sumClass = 0.;
		for (std::set<Index>::const_iterator it = classInd(k).begin(), itE =
				classInd(k).end(); it != itE; ++it) {
			sumClass += (*p_data_)(*it);
		}

		param_(k) = sumClass / Real(classInd(k).size());
	}

	return warnLog;
}

std::vector<std::string> Poisson::paramNames() const {
	std::vector<std::string> names(nClass_);
	for (int k = 0; k < nClass_; ++k) {
		std::stringstream sstm;
		sstm << "k: " << k + minModality << ", lambda";
		names[k] = sstm.str();
	}
	return names;
}

std::string Poisson::setData(const std::string& paramStr,
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

void Poisson::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tlambda: " << param_[k] << std::endl;
	}

#ifdef MC_VERBOSE
	std::cout << sstm.str() << std::endl;
#endif
}

std::string Poisson::checkSampleCondition(
		const Vector<std::set<Index>>& classInd) const {
	if (degeneracyAuthorizedForNonBoundedLikelihood)
		return "";

	for (Index k = 0; k < nClass_; ++k) {
		for (std::set<Index>::const_iterator it = classInd(k).begin(), itE =
				classInd(k).end(); it != itE; ++it) {
			if ((*p_data_)(*it) > 0) {
				goto endItK;
			}
		}

		return "Poisson variables must have at least one non-zero individual per class. Class: "
				+ std::to_string(k)
				+ " only contains the 0 modality. If your data has too many individuals with a value of 0, a Poisson model can not describe it."
				+ eol;

		endItK: ;
	}

	return "";
}

void Poisson::initParam() {
}
;

} // namespace mixt
