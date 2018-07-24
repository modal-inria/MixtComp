/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_AUGMENTEDDATA_H
#define MIXT_AUGMENTEDDATA_H

#include <limits>
#include <utility>
#include <vector>
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Constants.h"
#include "../Statistic/mixt_UniformStatistic.h"
#include "../Mixture/Simple/Categorical/MultinomialStatistic.h"
#include "../Various/mixt_Enum.h"

namespace mixt {

template<typename Type>
class Range {
public:
	Range(Type min = std::numeric_limits<Type>::quiet_NaN(), Type max = std::numeric_limits<Type>::quiet_NaN(), bool hasRange = false);

	~Range() {
	}
	;

	Type min_;
	Type max_;
	Type range_;
	bool hasRange_;
};

template<typename DataType>
class AugmentedData {
public:
	/** Base type of the data table, for example, Real */
	typedef typename DataType::Type Type;

	/** Missing value descriptor: type of missing, and list of parameters */
	typedef typename std::pair<MisType, std::vector<Type> > MisVal;

	AugmentedData() :
			nbSample_(0), misCount_(nb_enum_MisType_), dataRange_(), fixedInitialization_(false) {
		for (int i = 0; i < nb_enum_MisType_; ++i) { // initialize counter for each type of missing value to 0
			misCount_(i) = 0;
		}
	}
	;

	/**
	 * Return the correct expression according to the number of individuals
	 * @param  Number of individuals
	 * @return "individual(s) has/have"
	 */
	std::string indExpression(int nbInd) {
		std::string num;
		if (misCount_(missing_) == 1) {
			num = " individual has a value ";
		} else {
			num = " individuals have values ";
		}
		return num;
	}

	/**
	 * Check the types of missing value.
	 * @param  A vector containing true or false according to the mixture support of a a given missing value type
	 * @return Description of the eventual error, otherwise empty string
	 */
	std::string checkMissingType(Vector<bool> listType) {
		std::string warnLog;
		if (listType(missing_) == false && misCount_(missing_) > 0) {
			std::stringstream sstm;
			sstm << "Non observed values are not supported for this model, yet " << misCount_(missing_) << indExpression(misCount_(missing_)) << "completely missing." << std::endl;
			warnLog += sstm.str();
		}

		if (listType(missingFiniteValues_) == false && misCount_(missingFiniteValues_) > 0) {
			std::stringstream sstm;
			sstm << "Partially observed values defined by list of possible values, {a, b, c, ... }, are not supported for this model, yet " << misCount_(missingFiniteValues_)
					<< indExpression(misCount_(missingFiniteValues_)) << "defined by list of possible values." << std::endl;
			warnLog += sstm.str();
		}

		if (listType(missingIntervals_) == false && misCount_(missingIntervals_) > 0) {
			std::stringstream sstm;
			sstm << "Partially observed values defined by interval, [a:b], are not supported for this model, yet " << misCount_(missingIntervals_) << indExpression(misCount_(missingIntervals_))
					<< "defined by interval." << std::endl;
			warnLog += sstm.str();
		}

		if (listType(missingLUIntervals_) == false && misCount_(missingLUIntervals_) > 0) {
			std::stringstream sstm;
			sstm << "Partially observed values defined by upper-bounded semi-interval, [-inf:a], are not supported for this model, yet " << misCount_(missingLUIntervals_)
					<< indExpression(misCount_(missingLUIntervals_)) << "defined by upper-bounded semi-interval." << std::endl;
			warnLog += sstm.str();
		}

		if (listType(missingRUIntervals_) == false && misCount_(missingRUIntervals_) > 0) {
			std::stringstream sstm;
			sstm << "Partially observed values defined by lower-bounded semi-interval, [a:+inf], are not supported for this model, yet " << misCount_(missingRUIntervals_)
					<< indExpression(misCount_(missingRUIntervals_)) << "defined by lower-bounded semi-interval." << std::endl;
			warnLog += sstm.str();
		}
		return warnLog;
	}

	void resizeArrays(int nbSample) {
		data_.resize(nbSample);
		misData_.resize(nbSample);
	}

	void computeRange() {
		Type min = 0;
		Type max = 0;
		bool dataRangeUpdate = false;

		for (int i = 0; i < misData_.rows(); ++i) {
			switch (misData_(i).first) {
			case present_: // data is present, range is updated directly
			{
				rangeUpdate(min, max, data_(i), dataRangeUpdate);
			}
				break;

				/* Data is missing, range is updated using information on the missing value.
				 * If data is completely missing, misData_(i, j).second.size() == 0, and no update on the ranges is
				 * carried on.
				 */
			default: {
				for (typename std::vector<Type>::const_iterator it = misData_(i).second.begin(); it != misData_(i).second.end(); ++it) {
					rangeUpdate(min, max, *it, dataRangeUpdate);
				}
			}
				break;
			}
		}
		if (dataRangeUpdate == true) {
			dataRange_ = Range<Type>(min, max);
		}
	}

	void setPresent(int i, Type val) {
		data_(i) = val;
		misData_(i) = MisVal(present_, std::vector<Type>());
		++misCount_(present_);
		++nbSample_;
	}

	void setMissing(int i, const MisVal& val) {
		data_(i) = std::numeric_limits<int>::quiet_NaN(); // set to quiet nan, for types that supports it. For int, the returned value would be 0 ...
		misData_(i) = val;
		++misCount_(val.first);
		++nbSample_;
	}

	/** set all individuals to be completely missing, for example for the latent class in unsupervised learning */
	void setAllMissing(int nbInd) {
		resizeArrays(nbInd);
		MisVal misVal;
		misVal.first = missing_; // description of completely missing individual
		for (int i = 0; i < nbInd; ++i) {
			setMissing(i, misVal);
		}
	}

	void removeMissing(Index i) {
		if (fixedInitialization_) { // restore initialization values
			data_(i) = initialData_(i);
		} else { // uniform sampling of the missing values, subject to the data bound constraints
			removeMissingSample(i);
		}
	}

	/** Remove the missing values by uniform samplings */
	void removeMissingSample(Index i);

	void setFixedInitialization() {
		fixedInitialization_ = true;
		initialData_ = data_;

		MisVal misVal;
		misVal.first = missing_; // initialization is fixed, but algorithm is used in a normal fashion, all class labels still being considered missing
		for (int i = 0; i < nbSample_; ++i) {
			setMissing(i, misVal);
		}
	}

	/** Completed data, usually a Vector, for example Vector<Index> or Vector<Real> */
	DataType data_;

	/** data structure for partially observed values */
	Vector<MisVal> misData_;

	/** total number of values */
	int nbSample_;

	/** Number of each type of missing data
	 * Order of indices */
	Vector<int> misCount_;

	/** available data ranges, one pair per data column */
	Range<Type> dataRange_;

private:
	/** Is the initialization fixed ? If yes, the initial partition will be stored in initialData_. It will be restored to data_
	 * upon every call to removeMissing (instead of the uniform sampling). This configuration is usually used during debug to
	 * help diagnosing errors. */
	bool fixedInitialization_;

	/** Initial partition provided by the user. Only used when fixedInitialization_ == true. */
	DataType initialData_;

	void rangeUpdate(Type& min, Type& max, const Type& val, bool& dataRangeUpdate) {
		if (!dataRangeUpdate) {
			min = val;
			max = val;
			dataRangeUpdate = true;
		} else {
			if (val < min)
				min = val;
			if (val > max)
				max = val;
		}
	}

	/** Uniform law*/
	UniformStatistic uniform_;

	/** multinomial law */
	MultinomialStatistic multi_;
};

} // namespace mixt

#endif // MIXT_AUGMENTEDDATA_H
