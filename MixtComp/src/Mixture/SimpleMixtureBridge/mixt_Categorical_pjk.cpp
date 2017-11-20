/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 17, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "boost/regex.hpp"

#include "IO/mixt_IO.h"
#include "IO/mixt_SpecialStr.h"
#include "Various/mixt_Constants.h"
#include "LinAlg/mixt_LinAlg.h"
#include "Various/mixt_Enum.h"

#include "mixt_Categorical_pjk.h"

namespace mixt {

Categorical_pjk::Categorical_pjk(
		const std::string& idName,
		int nbClass,
		Vector<Real>& param,
		const Vector<std::set<Index> >& classInd)
: idName_(idName),
  nClass_(nbClass),
  nModality_(0),
  p_data_(0),
  param_(param),
  classInd_(classInd) {} // modalities are not known at the creation of the object, hence a call to setModality is needed later

Vector<bool> Categorical_pjk::acceptedType() const {
	Vector<bool> at(nb_enum_MisType_);
	at(0) = true ; // present_,
	at(1) = true ; // missing_,
	at(2) = true ; // missingFiniteValues_,
	at(3) = false; // missingIntervals_,
	at(4) = false; // missingLUIntervals_,
	at(5) = false; // missingRUIntervals_,
	return at;
}

int Categorical_pjk::computeNbFreeParameters() const {
	return nClass_ * (nModality_ - 1);
}

void Categorical_pjk::mStep() {
	for (Index k = 0; k < nClass_; ++k) {
		Vector<Real> modalities(nModality_, 0.);

		for (
				std::set<Index>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
				it != itE;
				++it) {
			modalities((*p_data_)(*it)) += 1.;
		}

		modalities = modalities / Real(classInd_(k).size());

		for (Index p = 0; p < nModality_; ++p) {
			param_(k * nModality_ + p) = modalities(p);
		}
	}
}

std::vector<std::string> Categorical_pjk::paramNames() const {
	std::vector<std::string> names(nClass_ * nModality_);
	for (Index k = 0; k < nClass_; ++k) {
		for (Index p = 0; p < nModality_; ++p) {
			std::stringstream sstm;
			sstm << "k: "
					<< k + minModality
					<< ", modality: "
					<< p + minModality;
			names[k * nModality_ + p] = sstm.str();
		}
	}
	return names;
}

std::string Categorical_pjk::setData(
		std::string& paramStr,
		AugmentedData<Vector<int> >& augData,
		RunMode mode) {
	std::string warnLog;

	p_data_ = &(augData.data_);

	if (paramStr.size() == 0) { // During learning without parameter space descriptor. Parameter space is deduced.
		nModality_ = augData.dataRange_.max_ + 1;
		param_.resize(nClass_ * nModality_);

		paramStr = "nModality: " + std::to_string(nModality_); // paramStr must be generated from the data, for future use and export for prediction
	}
	else { // During learning with parameter space descriptor, or in prediction
		std::string nModStr = std::string("nModality: *") + strPositiveInteger;
		boost::regex nModRe(nModStr);
		boost::smatch matchesVal;

		if (boost::regex_match(paramStr, matchesVal, nModRe)) { // value is present
			nModality_ = str2type<Index>(matchesVal[1].str());
		}
		else {
			std::stringstream sstm;
			sstm << "Variable: " << idName_ << " parameter string is not in the correct format, which should be \"nModality: x\" "
					<< "with x the number of modalities in the variable." << std::endl;
			warnLog += sstm.str();
		}

		if (nModality_ <= augData.dataRange_.max_) { // check if the range of data is within the parameter space. As > 0 is checked systematically, here only the higher bound is checked
			std::stringstream sstm;
			sstm << "Variable: " << idName_ << " requires a maximum value of : " << nModality_ - 1 + minModality << " in either provided values or bounds. "
					<< "The maximum currently provided value is : " << augData.dataRange_.max_ + minModality << std::endl;
			warnLog += sstm.str();
		}
	}

	if (augData.dataRange_.min_ < 0) { // Neither in learning nor in prediction are sub zero values valid
		std::stringstream sstm;
		sstm << "Variable: " << idName_ << " requires a minimum value of : " << minModality << " in either provided values or bounds. "
				<< "The minimum value currently provided is : " << augData.dataRange_.min_ + minModality << std::endl;
		warnLog += sstm.str();
	}

	//  Code to check that all modalities have been observed at least once, which put a constraint on the data set, but allows for prediction in every cases
	//	if (mode == learning_) {
	//		Vector<bool> modalityPresent(nModality_, false);
	//		for (Index i = 0; i < p_data_->size(); ++i) {
	//			modalityPresent((*p_data_)(i)) = true;
	//			if (modalityPresent == true) { // each modality is present i
	//				break; // avoid further testing
	//			}
	//		}
	//
	//		if (modalityPresent != true) { // if at least one modality has been observed
	//			for (Index p = 0; p < nModality_; ++p) {
	//				if (modalityPresent(p) == false) {
	//					warnLog += "Categorical variables must have one individual with each modality present in the sample. Modality: " + std::to_string(p + minModality) + " is not observed in your data set. You should check that all of your observed modalities are encoded using contiguous integers starting at "
	//							+ std::to_string(minModality) + "." + eol;
	//				}
	//			}
	//		}
	//	}

	// Once everything has been set, adjust the range of data to align with the parameter space
	augData.dataRange_.min_ = 0;
	augData.dataRange_.max_ = nModality_ - 1;
	augData.dataRange_.range_ = nModality_;

	return warnLog;
}

void Categorical_pjk::writeParameters() const {
	std::stringstream sstm;
	for (Index k = 0; k < nClass_; ++k) {
		sstm << "Class: " << k << std::endl;
		for (Index p = 0; p < nModality_; ++p) {
			sstm << "\talpha_ "  << p << ": " << param_(k * nModality_ + p) << std::endl;
		}
	}

	std::cout << sstm.str() << std::endl;
}

std::string Categorical_pjk::checkSampleCondition() const {
	if (degeneracyAuthorizedForNonBoundedLikelihood) return "";

	for (Index k = 0; k < nClass_; ++k) {
		std::string warnLog;
		Vector<bool> modalityPresent(nModality_, false);
		for (std::set<Index>::const_iterator it  = classInd_(k).begin(),
				itE = classInd_(k).end();
				it != itE;
				++it) {
			modalityPresent((*p_data_)(*it)) = true;
			if (modalityPresent == true) { // each modality is present i
				goto endItK; // avoid further testing
			}
		}

		for (Index p = 0; p < nModality_; ++p) {
			if (modalityPresent(p) == false) {
				warnLog += "Categorical variables must have one individual with each modality present in each class. Modality: " + std::to_string(p + minModality) + " is absent from class: " + std::to_string(k)
				+ " You can check whether you have enough individuals regarding the number of classes and whether all of your modalities are encoded using contiguous integers starting at " + std::to_string(minModality) + "." + eol;
			}
		}
		return warnLog;

		endItK:; // jump to avoid testing and output of log
	}

	return "";
}


bool Categorical_pjk::hasModalities() const {
	return true;
}

std::string Categorical_pjk::initParam(const Vector<Index>& initObs) {
	std::cout << "Categorical_pjk::initParam" << std::endl;

	Real constantTerm = 1. / nClass_; // this could also be based on the number of observations, to get a Bayesian justification
	param_ = 0.;

	for (Index k = 0; k < nClass_; ++k) {
		Vector<Real> currProp(nModality_, constantTerm); // proportions for current class
		currProp((*p_data_)(initObs(k))) += 1.;
		currProp = currProp / currProp.sum();

		Index baseClassIndex = k * nModality_;

		for (Index m = 0; m < nModality_; ++m) {
			param_(m + baseClassIndex) = currProp(m);
		}
	}

	return "";
}

std::vector<bool> Categorical_pjk::parametersInInterior() {
	std::vector<bool> res(nClass_);

	for (Index k = 0; k < nClass_; ++k) {
		res[k] = (param_.block(
				k * nModality_,
				0,
				nModality_,
				1).minCoeff() == 0.0) ? false : true;
	}

	return res;
}

} // namespace mixt
