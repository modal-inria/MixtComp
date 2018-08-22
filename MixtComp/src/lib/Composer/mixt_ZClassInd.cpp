/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: February 24, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <IO/IOFunctions.h>
#include "mixt_ZClassInd.h"

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
	std::cout << "zi_: " << itString(zi_.data_) << std::endl;

	for (Index k = 0; k < nbClass_; ++k) {
		std::cout << "k = " << k << ":";
		for (std::set<Index>::const_iterator it = classInd_(k).begin(), itEnd = classInd_(k).end(); it != itEnd; ++it) {
			std::cout << " " << *it;
		}
		std::cout << std::endl;
	}
}

} /* namespace mixt */
