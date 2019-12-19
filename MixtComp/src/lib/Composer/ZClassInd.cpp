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
 *  Created on: February 24, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <IO/IOFunctions.h>
#include "ZClassInd.h"

namespace mixt {

void ZClassInd::setIndClass(Index nbInd, Index nbClass) {
	nbInd_ = nbInd;
	nbClass_ = nbClass;
	zi_.resizeArrays(nbInd);
	classInd_.resize(nbClass);
}

void ZClassInd::computeRange() {
	zi_.computeRange();
}

std::string ZClassInd::checkRange() {
	std::string warnLog;
	if (zi_.dataRange_.min_ < 0) { // Since z is currently described using unsigned integer, there is no need for this check HOWEVER it might come in handy shall this condition changes
		std::stringstream sstm;
		sstm << "The z_class latent class variable has a lowest provided value of: " << minModality + zi().dataRange_.min_ << " while the minimal value has to be: " << minModality
				<< ". Please check the encoding of this variable to ensure proper bounds." << eol;
		warnLog += sstm.str();
	}
	if (zi_.dataRange_.hasRange_ == true || zi_.dataRange_.max_ > nbClass_ - 1) {
		std::stringstream sstm;
		sstm << "The z_class latent class variable has a highest provided value of: " << minModality + zi_.dataRange_.max_
				<< " while the maximal value can not exceed the number of class: " << minModality + nbClass_ - 1 << ". Please check the encoding of this variable to ensure proper bounds."
				<< eol;
		warnLog += sstm.str();
	}
	return warnLog;
}

std::string ZClassInd::setZi(std::vector<std::string>& data) {
	std::string warnLog;
	warnLog += StringToAugmentedData("z_class", data, zi_, -minModality);

	for (int k = 0; k < nbClass_; ++k) {
		classInd_(k).clear();
	}
	zi_.computeRange();

	warnLog += checkRange();

	if(warnLog.size() == 0)
	{
		for (int i = 0; i < nbInd_; ++i) {
			classInd_(zi_.data_(i)).insert(i);
		}
	}
	return warnLog;
}


void ZClassInd::setRange(int min, int max, int range) {
	zi_.dataRange_.min_ = min; // real range provided by the parameters is enforced
	zi_.dataRange_.max_ = max;
	zi_.dataRange_.range_ = range;
}

void ZClassInd::setAllMissing() {
	zi_.setAllMissing(nbInd_);
}

std::string ZClassInd::checkMissingType() {
	Vector<bool> at(nb_enum_MisType_); // authorized missing values, should mimic what is found in categorical mixtures
	at(0) = true; // present_,
	at(1) = true; // missing_,
	at(2) = true; // missingFiniteValues_,
	at(3) = false; // missingIntervals_,
	at(4) = false; // missingLUIntervals_,
	at(5) = false; // missingRUIntervals_,

	return zi_.checkMissingType(at);
}

void ZClassInd::setZAndClassInd(Index i, Index k) {
#pragma omp critical(setZAndClassInd)
	{
		classInd_(zi_.data_(i)).erase(i);
		classInd_(k).insert(i);
	}
	zi_.data_(i) = k;
}

void ZClassInd::printState() const {
#ifdef MC_VERBOSE
	std::cout << "zi_: " << itString(zi_.data_) << std::endl;

	for (Index k = 0; k < nbClass_; ++k) {
		std::cout << "k = " << k << ":";
		for (std::set<Index>::const_iterator it = classInd_(k).begin(), itEnd = classInd_(k).end(); it != itEnd; ++it) {
			std::cout << " " << *it;
		}
		std::cout << std::endl;
	}
#endif
}

} /* namespace mixt */
