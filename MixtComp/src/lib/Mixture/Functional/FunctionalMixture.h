/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_MIXTURE_FUNCTIONAL_FUNCTIONALMIXTURE
#define LIB_MIXTURE_FUNCTIONAL_FUNCTIONALMIXTURE

#include <vector>
#include <IO/IOFunctions.h>
#include <IO/NamedAlgebra.h>
#include <Mixture/Functional/Function.h>
#include <Mixture/IMixture.h>
#include "FunctionalClass.h"
#include "FunctionalParser.h"

namespace mixt {

template<typename Graph>
class FunctionalMixture: public IMixture {
public:
	FunctionalMixture(const Graph& data, const Graph& param, Graph& out, std::string const& idName, Index nClass, Index nObs, Real confidenceLevel, const std::string& paramStr) :
			IMixture(idName, "Func_CS", nClass, nObs), nSub_(0), nCoeff_(0), confidenceLevel_(confidenceLevel), dataG_(data), paramG_(param), outG_(out), paramStr_(paramStr) {
		class_.reserve(nClass_);
		for (Index k = 0; k < nClass_; ++k) {
			class_.emplace_back(vecInd_, confidenceLevel_);
		}

		acceptedType_.resize(nb_enum_MisType_);
		acceptedType_ << true, // present_,
		false, // missing_,
		false, // missingFiniteValues_,
		false, // missingIntervals_,
		false, // missingLUIntervals_,
		false; // missingRUIntervals
	}

	void sampleUnobservedAndLatent(Index i, Index k) {
		class_[k].samplingStepNoCheck(i);
	}
	;

	std::string checkSampleCondition(const Vector<std::set<Index> >& classInd) const {
		std::string classLog;
		for (Index k = 0; k < nClass_; ++k) {
			std::string currClassLog = class_[k].checkSampleCondition(classInd(k));
			if (0 < currClassLog.size()) {
				classLog += "Class: " + std::to_string(k) + ": " + currClassLog;
			}
		}

		if (0 < classLog.size()) {
			return "Error(s) in variable: " + idName_ + " with Func_CS model. The errors in the various classes are: " + eol + classLog;
		}

		return "";
	}

	std::string mStep(const Vector<std::set<Index> >& classInd) {
		std::string warnLog;

		for (Index k = 0; k < nClass_; ++k) {
			std::string currLog;
			currLog = class_[k].mStep(classInd(k));
			if (0 < currLog.size()) {
				warnLog += "Error in class " + std::to_string(k) + "." + eol + currLog;
			}
		}

		return warnLog;
	}
	;

	void storeSEMRun(Index iteration, Index iterationMax) {
		for (Index k = 0; k < nClass_; ++k) {
			class_[k].sampleParam(iteration, iterationMax);
		}

		if (iteration == iterationMax) {
			for (Index k = 0; k < nClass_; ++k) {
				class_[k].setExpectationParam();
			}
		}
	}
	;

	void storeGibbsRun(Index i, Index iteration, Index iterationMax) {
		// nothing until missing data is supported
	}
	;

	Real lnCompletedProbability(Index i, Index k) const {
		return vecInd_(i).lnCompletedProbability(class_[k].alpha(), class_[k].beta(), class_[k].sd());
	}

	Real lnObservedProbability(Index i, Index k) const {
		return vecInd_(i).lnObservedProbability(class_[k].alpha(), class_[k].beta(), class_[k].sd());
	}

	Index nbFreeParameter() const {
		return nClass_ * ((nSub_ - 1) * 2 // alpha (nSub_ -1 since alpha's are the parameters of a multivariate logistic regeression)
		+ nSub_ * nCoeff_ // beta
		+ nSub_); // sigma
	}

	void writeParameters() const {
		std::stringstream sstm;

		for (int k = 0; k < nClass_; ++k) {
			sstm << "Class: " << k << std::endl;
			sstm << "alpha: " << std::endl << class_[k].alpha() << std::endl;
			sstm << "beta: " << std::endl << class_[k].beta() << std::endl;
			sstm << "sigma: " << itString(class_[k].sd()) << std::endl;
		}

#ifdef MC_VERBOSE
		std::cout << sstm.str() << std::endl;
#endif
	}
	;

	/**
	 * Note that two paramStr are considered. One is provided at creation, by the createMixture function, and the other is read at prediction.
	 */
	std::string setDataParam(RunMode mode) {
		std::string warnLog;
		NamedVector<Real> alpha, beta, sd;

		std::vector<std::string> dataVecStr;
		dataG_.get_payload( { }, idName_, dataVecStr); // get the raw vector of strings

		if (mode == prediction_) { // prediction mode, linearized versions of the parameters are fetched, and then distributed to the classes
			paramG_.get_payload( { idName_ }, "paramStr", paramStr_);

			paramG_.get_payload( { idName_, "alpha" }, "stat", alpha);
			paramG_.get_payload( { idName_, "beta" }, "stat", beta);
			paramG_.get_payload( { idName_, "sd" }, "stat", sd);
		}

		// get the value of nSub_ and nCoeff_ by parsing paramStr_
		std::string paramReStr = std::string("nSub: *") + strPositiveInteger + std::string(", nCoeff: *") + strPositiveInteger;
		std::regex paramRe(paramReStr);
		std::smatch matches;
		if (std::regex_match(paramStr_, matches, paramRe)) { // value is present
			nSub_ = str2type<Index>(matches[1].str());
			nCoeff_ = str2type<Index>(matches[2].str());

			for (Index k = 0; k < nClass_; ++k) { // call setSize on each class
				class_[k].setSize(nSub_, nCoeff_);
			}
		} else {
			std::stringstream sstm;
			sstm << "Variable: " << idName_ << " has no parameter description. This description is required, and must take the form " << "\"nSub: x, nCoeff: y\"" << std::endl;
			warnLog += sstm.str();
		}

		if (mode == prediction_ && warnLog.size() == 0) { // prediction mode, linearized versions of the parameters are fetched, and then distributed to the classes
			Matrix<Real> alphaCurr(nSub_, 2);
			Matrix<Real> betaCurr(nSub_, nCoeff_);
			Vector<Real> sdCurr(nSub_);
			for (Index k = 0; k < nClass_; ++k) {
				for (Index s = 0; s < nSub_; ++s) {
					for (Index c = 0; c < 2; ++c) {
						alphaCurr(s, c) = alpha.vec_(k * nSub_ * 2 + s * 2 + c);
					}
				}

				for (Index s = 0; s < nSub_; ++s) {
					for (Index c = 0; c < nCoeff_; ++c) {
						betaCurr(s, c) = beta.vec_(k * nSub_ * nCoeff_ + s * nCoeff_ + c);
					}
				}

				for (Index s = 0; s < nSub_; ++s) {
					sdCurr(s) = sd.vec_(k * nSub_ + s);
				}

				class_[k].setParam(alphaCurr, betaCurr, sdCurr);
				class_[k].setParamStorage();
			}
		}

		warnLog += parseFunctionalStr(nSub_, nCoeff_, dataVecStr, vecInd_); // convert the vector of strings to ranks
		warnLog += checkMissingType();
		if (warnLog.size() > 0) {
			return warnLog;
		}

		globalQuantile(vecInd_, quantile_);

		// TODO: datastat will be setup here when partially observed value will be supported
		// should lnObservedProbability be computed here in prediction, as it is done for Ordinal and Rank ?

		return warnLog;
	}

	/**
	 * Linearize and format the information provided by each class, and send it to the usual extractors, nothing fancy here.
	 */
	void exportDataParam() const {
		std::vector<std::vector<Real>> data(nInd_);
		std::vector<std::vector<Real>> time(nInd_);

		for (Index i = 0; i < nInd_; ++i) {
			Index nTime = vecInd_(i).nTime();
			data[i].resize(nTime);
			time[i].resize(nTime);

			for (Index t = 0; t < nTime; ++t) {
				data[i][t] = vecInd_(i).x()(t);
				time[i][t] = vecInd_(i).t()(t);
			}
		}

		outG_.add_payload( { "variable", "data", idName_ }, "data", data);
		outG_.add_payload( { "variable", "data", idName_ }, "time", time);

		Index sizeClassAlpha = nSub_ * 2;
		Index sizeClassBeta = nSub_ * nCoeff_;
		Index sizeClassSd = nSub_;

		Index nStat = class_[0].alphaParamStat().getStatStorage().cols();

		std::vector<std::string> colNames;
		quantileNames(nStat, confidenceLevel_, colNames);

		NamedMatrix<Real> alphaStat = { alphaParamNames(), colNames, Matrix<Real>(nClass_ * sizeClassAlpha, nStat) };
		NamedMatrix<Real> betaStat = { betaParamNames(), colNames, Matrix<Real>(nClass_ * sizeClassBeta, nStat) };
		NamedMatrix<Real> sdStat = { sdParamNames(), colNames, Matrix<Real>(nClass_ * sizeClassSd, nStat) };

		Index nObs = class_[0].alphaParamStat().getLogStorage().cols();

		NamedMatrix<Real> alphaLog = { alphaParamNames(), std::vector<std::string>(), Matrix<Real>(nClass_ * sizeClassAlpha, nObs) }; // linearized and concatenated version of alpha
		NamedMatrix<Real> betaLog = { betaParamNames(), std::vector<std::string>(), Matrix<Real>(nClass_ * sizeClassBeta, nObs)};
		NamedMatrix<Real> sdLog = { sdParamNames(), std::vector<std::string>(), Matrix<Real>(nClass_ * sizeClassSd, nObs)};

		for (Index k = 0; k < nClass_; ++k) {
			alphaStat.mat_.block(k * sizeClassAlpha, 0, sizeClassAlpha, nStat) = class_[k].alphaParamStat().getStatStorage();
			alphaLog.mat_.block(k * sizeClassAlpha, 0, sizeClassAlpha, nObs) = class_[k].alphaParamStat().getLogStorage();

			betaStat.mat_.block(k * sizeClassBeta, 0, sizeClassBeta, nStat) = class_[k].betaParamStat().getStatStorage();
			betaLog.mat_.block(k * sizeClassBeta, 0, sizeClassBeta, nObs) = class_[k].betaParamStat().getLogStorage();

			sdStat.mat_.block(k * sizeClassSd, 0, sizeClassSd, nStat) = class_[k].sdParamStat().getStatStorage();
			sdLog.mat_.block(k * sizeClassSd, 0, sizeClassSd, nObs) = class_[k].sdParamStat().getLogStorage();
		}

		outG_.add_payload( { "variable", "param", idName_ }, "paramStr", paramStr_);

		outG_.add_payload( { "variable", "param", idName_, "alpha" }, "stat", alphaStat);
		outG_.add_payload( { "variable", "param", idName_, "beta" }, "stat", betaStat);
		outG_.add_payload( { "variable", "param", idName_, "sd" }, "stat", sdStat);

		outG_.add_payload( { "variable", "param", idName_, "alpha" }, "log", alphaLog);
		outG_.add_payload( { "variable", "param", idName_, "beta" }, "log", betaLog);
		outG_.add_payload( { "variable", "param", idName_, "sd" }, "log", sdLog);
	}
	;

	void initData(Index i) {
		vecInd_(i).removeMissingQuantileMixing(quantile_);
	}
	;

	/**
	 * @param initObs element k contains the index of
	 */
	void initParam() {
		for (Index k = 0; k < nClass_; ++k) {
			class_[k].initParam();
		}
	}
	;

	/**
	 * This is for debug only
	 */
	void printTik() {
		for (Index i = 0; i < nInd_; ++i) {
			Vector<Real> completedProba(nClass_);
			Vector<Real> tik(nClass_);
			for (Index k = 0; k < nClass_; ++k) {
				completedProba(k) = lnCompletedProbability(i, k);
			}
			tik.logToMulti(completedProba);
#ifdef MC_VERBOSE
			std::cout << itString(tik) << std::endl;
#endif
		}
	}

	/** No need to precompute an empirical distribution. */
	void computeObservedProba() {
	}
	;

	void initializeMarkovChain(Index i, Index k) {
	}
	;

	bool sampleApproximationOfObservedProba() {
		return false;
	}
private:
	std::string checkMissingType() {
		std::string warnLog;
		// to be populated with checks. Each Function object must have a checkMissingType
		return warnLog;
	}

	std::vector<std::string> alphaParamNames() const {
		std::vector<std::string> names(nClass_ * nSub_ * 2);
		for (Index k = 0; k < nClass_; ++k) {
			for (Index s = 0; s < nSub_; ++s) {
				std::stringstream sstm0;
				sstm0 << "k: " << k + minModality << ", s: " << s << ", alpha0";
				names[k * nSub_ * 2 + s * 2] = sstm0.str();

				std::stringstream sstm;
				sstm << "k: " << k + minModality << ", s: " << s << ", alpha1";
				names[k * nSub_ * 2 + s * 2 + 1] = sstm.str();
			}
		}
		return names;
	}

	std::vector<std::string> betaParamNames() const {
		std::vector<std::string> names(nClass_ * nSub_ * nCoeff_);
		for (Index k = 0; k < nClass_; ++k) {
			for (Index s = 0; s < nSub_; ++s) {
				for (Index c = 0; c < nCoeff_; ++c) {
					std::stringstream sstm;
					sstm << "k: " << k + minModality << ", s: " << s << ", c: " << c;
					names[k * nSub_ * nCoeff_ + s * nCoeff_ + c] = sstm.str();
				}
			}
		}
		return names;
	}

	std::vector<std::string> sdParamNames() const {
		std::vector<std::string> names(nClass_ * nSub_);
		for (Index k = 0; k < nClass_; ++k) {
			for (Index s = 0; s < nSub_; ++s) {
				std::stringstream sstm;
				sstm << "k: " << k + minModality << ", s: " << s;
				names[k * nSub_ + s] = sstm.str();
			}
		}
		return names;
	}

	Index nSub_;
	Index nCoeff_;
	Real confidenceLevel_;

	/** Data */
	Vector<Function> vecInd_;
	Vector<Real> quantile_;

	const Graph& dataG_;
	const Graph& paramG_;
	Graph& outG_;

	std::vector<FunctionalClass> class_;

	Vector<bool> acceptedType_;

	std::string paramStr_;
};

} // namespace mixt

#endif // FUNCTIONALMIXTURE
