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

#include <Data/AugmentedData.h>
#include <IO/IO.h>
#include <LinAlg/LinAlg.h>
#include <LinAlg/Math.h>
#include <Various/Constants.h>
#include <cmath>

#include "Various/Enum.h"

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
							+ ". If some values are repeated often in this variable, maybe a Multinomial or a Poisson variable will describe it better."
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
