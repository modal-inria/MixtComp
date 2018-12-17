/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_AugmentedData.h"

namespace mixt {

template<>
Range<Real>::Range(Real min, Real max, bool hasRange) :
		min_(min), max_(max), range_(max - min), hasRange_(hasRange) {
}

template<>
Range<int>::Range(int min, int max, bool hasRange) :
		min_(min), max_(max), range_(max - min + 1), // used to store the number of modalities, for example,
		hasRange_(hasRange) {
}

template<>
Range<std::size_t>::Range(std::size_t min, std::size_t max, bool hasRange) :
		min_(min), max_(max), range_(max - min + 1), // used to store the number of modalities, for example,
		hasRange_(hasRange) {
}

template<>
Range<std::ptrdiff_t>::Range(std::ptrdiff_t min, std::ptrdiff_t max, bool hasRange) :
		min_(min), max_(max), range_(max - min + 1), // used to store the number of modalities, for example,
		hasRange_(hasRange) {
}

template<>
void AugmentedData<Vector<Real> >::removeMissingSample(Index i) {
	if (misData_(i).first != present_) {
		Real sampleVal = 0.;
		switch (misData_(i).first) // (iterator on map)->(mapped element).(MisType)
		{
		case present_: {
		}
			break;

		case missing_: {
			Real min = dataRange_.min_;
			Real max = dataRange_.max_;
			sampleVal = uniform_.sample(min, max);
		}
			break;

		case missingFiniteValues_: // no missing finite values for continuous data
		{
		}
			break;

		case missingIntervals_: {
			Real infBound = misData_(i).second[0]; // (iterator on map)->(mapped element).(vector of parameters)[element]
			Real supBound = misData_(i).second[1];
			sampleVal = uniform_.sample(infBound, supBound);
		}
			break;

		case missingLUIntervals_: {
			Real min = dataRange_.min_;
			Real supBound = misData_(i).second[0];
			if (min < supBound) {
				sampleVal = uniform_.sample(min, supBound);
			} else {
				sampleVal = supBound;
			}
		}
			break;

		case missingRUIntervals_: {
			Real infBound = misData_(i).second[0];
			Real max = dataRange_.max_;
			if (infBound < max) {
				sampleVal = uniform_.sample(infBound, max);
			} else {
				sampleVal = infBound;
			}
		}
			break;

		default: {
			throw("AugmentedData<Vector<Real> >::removeMissingSample, error in missing data handling, please report to the maintainer.");
		}
			break;

		}
		data_(i) = sampleVal;
	}
}

template<>
void AugmentedData<Vector<int> >::removeMissingSample(Index i) {
	if (misData_(i).first != present_) {
		int sampleVal = 0;
		int nbModalities = dataRange_.range_;

		switch (misData_(i).first) // (iterator on map)->(mapped element).(MisType)
		{
		case present_: {
		}
			break;

		case missing_: {
			Vector<Real> modalities(nbModalities);
			modalities = 1. / nbModalities;
			sampleVal = multi_.sample(modalities);
		}
			break;

		case missingFiniteValues_: {
			Real proba = 1. / misData_(i).second.size(); // (iterator on map)->(mapped element).(vector of parameters)
			Vector<Real> modalities(nbModalities);
			modalities = 0.;
			for (std::vector<int>::const_iterator itParam = misData_(i).second.begin(); itParam != misData_(i).second.end(); ++itParam) {
				modalities[*itParam] = proba;
			}
			sampleVal = multi_.sample(modalities);
		}
			break;

		case missingIntervals_: {
			Real infBound = misData_(i).second[0]; // (iterator on map)->(mapped element).(vector of parameters)[element]
			Real supBound = misData_(i).second[1];
			sampleVal = uniformInt_.sample(infBound, supBound);
		}
			break;

		case missingLUIntervals_: {
			Real min = dataRange_.min_;
			Real supBound = misData_(i).second[0];
			if (min < supBound) {
				sampleVal = uniformInt_.sample(min, supBound);
			} else {
				sampleVal = supBound;
			}
		}
			break;

		case missingRUIntervals_: {
			Real infBound = misData_(i).second[0];
			Real max = dataRange_.max_;
			if (infBound < max) {
				sampleVal = uniformInt_.sample(infBound, max);
			} else {
				sampleVal = infBound;
			}
		}
			break;

		default: {
			throw("AugmentedData<Vector<int> >::removeMissingSample, error in missing data handling, please report to the maintainer.");
		}
			break;
		}
		data_(i) = sampleVal;
	}
}

template<>
void AugmentedData<Vector<std::size_t> >::removeMissingSample(Index i) {
	if (misData_(i).first != present_) {
		int sampleVal = 0;
		int nbModalities = dataRange_.range_;

		switch (misData_(i).first) // (iterator on map)->(mapped element).(MisType)
		{
		case present_: {
		}
			break;

		case missing_: {
			Vector<Real> modalities(nbModalities);
			modalities = 1. / nbModalities;
			sampleVal = multi_.sample(modalities);
		}
			break;

		case missingFiniteValues_: {
			Real proba = 1. / misData_(i).second.size(); // (iterator on map)->(mapped element).(vector of parameters)
			Vector<Real> modalities(nbModalities);
			modalities = 0.;
			for (std::vector<std::size_t>::const_iterator itParam = misData_(i).second.begin(); itParam != misData_(i).second.end(); ++itParam) {
				modalities[*itParam] = proba;
			}
			sampleVal = multi_.sample(modalities);
		}
			break;

		default: // other types of intervals not present in integer data
		{
			throw("AugmentedData<Vector<std::size_t> >::removeMissingSample, error in missing data handling, please report to the maintainer.");
		}
			break;
		}
		data_(i) = sampleVal;
	}
}

template<>
void AugmentedData<Vector<std::ptrdiff_t> >::removeMissingSample(Index i) {
	if (misData_(i).first != present_) {
		int sampleVal = 0;
		int nbModalities = dataRange_.range_;

		switch (misData_(i).first) // (iterator on map)->(mapped element).(MisType)
		{
		case present_: {
		}
			break;

		case missing_: {
			Vector<Real> modalities(nbModalities);
			modalities = 1. / nbModalities;
			sampleVal = multi_.sample(modalities);
		}
			break;

		case missingFiniteValues_: {
			Real proba = 1. / misData_(i).second.size(); // (iterator on map)->(mapped element).(vector of parameters)
			Vector<Real> modalities(nbModalities);
			modalities = 0.;
			for (std::vector<std::ptrdiff_t>::const_iterator itParam = misData_(i).second.begin(); itParam != misData_(i).second.end(); ++itParam) {
				modalities[*itParam] = proba;
			}
			sampleVal = multi_.sample(modalities);
		}
			break;

		default: // other types of intervals not present in integer data
		{
			throw("AugmentedData<Vector<std::ptrdiff_t> >::removeMissingSample, error in missing data handling, please report to the maintainer.");
		}
			break;
		}
		data_(i) = sampleVal;
	}
}

} // namespace mixt
