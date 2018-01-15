/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Data/mixt_AugmentedData.h"
#include "LinAlg/mixt_Math.h"
#include "LinAlg/mixt_LinAlg.h"
#include "Various/mixt_Constants.h"
#include "IO/mixt_IO.h"
#include "Various/mixt_Enum.h"

#include "mixt_Gaussian_sjk.h"

namespace mixt {

Gaussian_sjk::Gaussian_sjk(
		const std::string& idName,
		int nbClass,
		Vector<Real>& param,
		const Vector<std::set<Index> >& classInd)
: idName_(idName),
  nClass_(nbClass),
  param_(param),
  p_data_(0),
  classInd_(classInd) {
	param_.resize(2 * nbClass);
}

Vector<bool> Gaussian_sjk::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true ; // present_,
	at(1) = true ; // missing_,
	at(2) = false; // missingFiniteValues_,
	at(3) = true ; // missingIntervals_,
	at(4) = true ; // missingLUIntervals_,
	at(5) = true ; // missingRUIntervals_,
	return at;
}

int Gaussian_sjk::computeNbFreeParameters() const {
	return 2 * nClass_;
}

bool Gaussian_sjk::hasModalities() const {
	return false;
}

void Gaussian_sjk::mStep() {
	for (int k = 0; k < nClass_; ++k) {
		Real mean;
		Real sd;

		meanSD(classInd_(k),
				*p_data_,
				mean,
				sd);

		param_(2 * k    ) = mean;
		param_(2 * k + 1) = sd;
	}
}

std::vector<std::string> Gaussian_sjk::paramNames() const {
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

std::string Gaussian_sjk::setData(
		const std::string& paramStr,
		AugmentedData<Vector<Real> >& augData,
		RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	return warnLog;
}

void Gaussian_sjk::writeParameters() const {
	std::stringstream sstm;
	for (int k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		sstm << "\tmean: " << param_[2 * k    ] << std::endl;
		sstm << "\tsd: "   << param_[2 * k + 1] << std::endl;
	}

	std::cout << sstm.str() << std::endl;
}

std::string Gaussian_sjk::checkSampleCondition() const {
	for (Index k = 0; k < nClass_; ++k) {
		if (classInd_(k).size() < 2) {
			return "Gaussian variables must have at least two individuals per class. This is not the case for at least one class. You can check whether you have enough individuals regarding the number of classes." + eol;
		}

		std::set<Index>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
		Real previousElemClass = (*p_data_)(*it);
		++it;
		for (; it != itE; ++it) {
			if ((*p_data_)(*it) != previousElemClass) { // stop checking soon as there are two different values in the current class
				goto endItK; // goto used as a kind of super break statement, see http://stackoverflow.com/questions/1257744/can-i-use-break-to-exit-multiple-nested-for-loops
			}
		}

		return "Gaussian variables must have a minimum amount of variability in each class. It seems that at least one class only contains the value: "
				+  std::to_string(previousElemClass) + ". If some values are repeated often in this variable, maybe a Categorical or a Poisson variable might describe it better." + eol;

		endItK:;
	}

	return "";
}

std::string Gaussian_sjk::initParam(const Vector<Index>& initObs) {
	Real sampleMean, sampleSd;
	meanSD((*p_data_), sampleMean, sampleSd); // computation of sample standard deviation
	Real classSd = sampleSd / nClass_; // variance per class

	bool hasVariance = false;
	for (Index k = 0; k < nClass_; ++k) {
		param_(2 * k    ) = (*p_data_)(initObs(k)); // mean is obtained from a sampled value
		param_(2 * k + 1) = classSd;
		if (0.0 < classSd) hasVariance = true;
	}

	std::stringstream sstm;
	if (hasVariance == false) {
		sstm << "Not enough variance. The data is accumulated around some values for at least one class, yet the Gaussian distribution needs some variability to adequately model a sample." << std::endl;
	}

	return sstm.str();
}

std::vector<bool> Gaussian_sjk::parametersInInterior() {
	std::vector<bool> res(nClass_);
	for (Index k = 0; k < nClass_; ++k) {
		res[k] = (param_(2 * k + 1) == 0.0) ? false : true;
	}
	return res;
}

} // namespace mixt
