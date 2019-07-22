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
 *  Created on: June 5, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef CONFINTPARAMSTAT_H
#define CONFINTPARAMSTAT_H

#include <IO/IO.h>
#include <IO/SpecialStr.h>
#include <LinAlg/LinAlg.h>
#include <regex>


namespace mixt {

/**
 * Computation of confidence interval on parameters. Templated for int or Real cases.
 * Note that the storage uses a linearized version of the initial storage, therefore ConfIntParamStat can be templated with Vector and Matrix
 * transparently.
 * */
template<typename ContainerType>
class ConfIntParamStat {
public:
	typedef typename ContainerType::Type Type;

	ConfIntParamStat(ContainerType& param, Real confidenceLevel) :
			initialNIter_(0), nRows_(0), nCols_(0), nCoeff_(0), param_(param), confidenceLevel_(
					confidenceLevel) {
	}

	void sampleParam(Index iteration, Index iterationMax) {
		if (iteration == 0) {
			nRows_ = param_.rows(); // size of nbParam is know at the first sample and from there on will stay fixed
			nCols_ = param_.cols();
			nCoeff_ = nRows_ * nCols_;

			initialNIter_ = iterationMax + 1;

			logStorage_.resize(nCoeff_, initialNIter_); // resize internal storage
			statStorage_.resize(nCoeff_, 3); // resize export storage

			sample(0); // first sampling, on each parameter
		} else if (iteration == iterationMax) {
			sample(iterationMax); // last sampling

			if (iterationMax + 1 != initialNIter_) {
				logStorage_ = logStorage_.block(0, 0, nCoeff_, iterationMax + 1).eval(); // if partition is stable, iterationMax has been reduced in comparison to initialNIter_
			}

			for (Index p = 0; p < nCoeff_; ++p) { // each row corresponds to a parameters, hence treatment is applied on each row
				RowVector<Type> currRow = logStorage_.row(p);
				currRow.sort();

				Real alpha = (1. - confidenceLevel_) / 2.;
				Index realIndLow = alpha * iterationMax;
				Index realIndHigh = (1. - alpha) * iterationMax;

				statStorage_(p, 0) = currRow(iterationMax / 2);
				statStorage_(p, 1) = currRow(realIndLow);
				statStorage_(p, 2) = currRow(realIndHigh + 1);
			}
		} else {
			sample(iteration); // standard sampling
		}
	}

	/** Set the storage of parameters, for example in the prediction case where there is no M-Step, and no
	 * statistics on the estimated parameters. */
	void setParamStorage() {
		nRows_ = param_.rows(); // size of nbParam is know at the first sample and from there on will stay fixed
		nCols_ = param_.cols();
		nCoeff_ = nRows_ * nCols_;

		statStorage_.resize(nCoeff_, 1); // no quantiles have to be computed for imported parameters, hence the single column
		logStorage_.resize(nCoeff_, 0); // in prediction, there will be no other resize than that one
		for (Index j = 0; j < nCols_; ++j) {
			for (Index i = 0; i < nRows_; ++i) {
				statStorage_(i * nCols_ + j, 0) = param_(i, j);
			}
		}
	}

	/** fill the parameters with estimators of the expectation, to be used in the Gibbs sampling */
	void setExpectationParam() {
		for (Index j = 0; j < nCols_; ++j) {
			for (Index i = 0; i < nRows_; ++i) {
				param_(i, j) = statStorage_(i * nCols_ + j, 0);
			}
		}
	}

	/**
	 * Perform renormalization on statStorage. Useful for categorical modes where imputed parameters must
	 * sum to 1
	 */
	void normalizeParam(const std::string& paramStr) {
		Index nModality = 0;

		std::string nModStr = std::string("nModality: *") + strPositiveInteger;
		std::regex nModRe(nModStr);
		std::smatch matchesVal;

		if (std::regex_match(paramStr, matchesVal, nModRe)) { // value is present
			nModality = str2type<Index>(matchesVal[1].str());
		}

		if (nModality > 0) {
			Index nClass = param_.size() / nModality;

			for (Index j = 0; j < statStorage_.cols(); ++j) { // note that the normalization is done column by column...
				for (Index k = 0; k < nClass; ++k) {
					Type sumClass = 0; // ... and class by class
					for (Index p = 0; p < nModality; ++p) {
						sumClass += statStorage_(k * nModality + p, j);
					}
					for (Index p = 0; p < nModality; ++p) {
						statStorage_(k * nModality + p, j) /= sumClass;
					}
				}
			}
		}
	}

	const Matrix<Type>& getStatStorage() const {
		return statStorage_;
	}
	const Matrix<Type>& getLogStorage() const {
		return logStorage_;
	}

private:
	void sample(Index iteration) {
		for (Index j = 0; j < nCols_; ++j) {
			for (Index i = 0; i < nRows_; ++i) {
				logStorage_(i * nCols_ + j, iteration) = param_(i, j);
			}
		}
	}

	// number of iterations used to compute the statistics
	Index initialNIter_;

	// number of parameters
	Index nRows_;
	Index nCols_;
	Index nCoeff_;

	// Reference to param array
	ContainerType& param_;

	/** Array to export the statistics at the last iteration
	 * first dimension: index of the parameter
	 * second dimension: median, left quantile, right quantile */
	Matrix<Type> statStorage_;

	/** Storage for iterations results,
	 * first dimension: index of the parameter
	 * second dimension: iteration of the stored value */
	Matrix<Type> logStorage_;

	/** Confidence level */
	Real confidenceLevel_;
};

} // namespace mixt

#endif // INTCONFPARAMSTAT_H
