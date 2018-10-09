/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALSHAREDALPHAMIXTURE
#define MIXT_FUNCTIONALSHAREDALPHAMIXTURE

#include <vector>
#include <set>
#include <Mixture/mixt_IMixture.h>
#include <IO/IOFunctions.h>
#include <IO/NamedAlgebra.h>
#include "mixt_Function.h"
#include "mixt_FunctionalClass.h"
#include "mixt_FunctionalParser.h"

namespace mixt {

template<typename Graph>
class FunctionalSharedAlphaMixture: public IMixture {
public:
	FunctionalSharedAlphaMixture(const Graph& data, const Graph& param, Graph& out, std::string const& idName, Index nClass, Index nObs, Real confidenceLevel,
			const std::string& paramStr) :
			IMixture(idName, "Func_SharedAlpha_CS", nClass, nObs), nSub_(0), nCoeff_(0), confidenceLevel_(confidenceLevel), dataG_(data), paramG_(param), outG_(
					out), paramStr_(paramStr) {
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
			return "Error(s) in variable: " + idName_ + " with Functional model. The errors in the various classes are: " + eol + classLog;
		}

		return "";
	}

	/**
	 * This is the main implementation difference between Functional and FunctionalSharedAlpha. Note that
	 * FunctionalClass::mStep is never called.
	 */
	std::string mStep(const Vector<std::set<Index> >& classInd) {
		std::string warnLog;

		std::set<Index> setAllObs;

		for (Index k = 0; k < nClass_; ++k) { // build the set of all observations
			for (std::set<Index>::const_iterator it = classInd(k).begin(), itEnd = classInd(k).end(); it != itEnd; ++it) {
				setAllObs.insert(*it);
			}
		}

		class_[0].mStepAlpha(setAllObs); // perform the mStep in the first class using all the individuals
		broadcastAlpha(); // broadcast the results to all classes

		for (Index k = 0; k < nClass_; ++k) { // build the set of all observations
			std::string currLog;
			currLog = class_[k].mStepBetaSd(classInd(k));
			if (0 < currLog.size()) {
				warnLog += "Error in class " + std::to_string(k) + "." + eol + currLog;
			}
		}

		return warnLog;
	}
	;

	/**
	 * Broadcast the alpha parameters from the first class to all classes.
	 */
	void broadcastAlpha() {
		Matrix<Real> alpha = class_[0].getAlpha();
		for (Index k = 1; k < nClass_; ++k) {
			class_[k].setAlpha(alpha);
		}
	}

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

		std::cout << sstm.str() << std::endl;
	}
	;

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
		boost::regex paramRe(paramReStr);
		boost::smatch matches;
		if (boost::regex_match(paramStr_, matches, paramRe)) { // value is present
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

//		Matrix<Real> alphaLog(nClass_ * sizeClassAlpha, nObs); // linearized and concatenated version of alpha
//		Matrix<Real> betaLog(nClass_ * sizeClassBeta, nObs);
//		Matrix<Real> sdLog(nClass_ * sizeClassSd, nObs);

		for (Index k = 0; k < nClass_; ++k) {
			alphaStat.mat_.block(k * sizeClassAlpha, 0, sizeClassAlpha, nStat) = class_[k].alphaParamStat().getStatStorage();
//			alphaLog.block(k * sizeClassAlpha, 0, sizeClassAlpha, nObs) = class_[k].alphaParamStat().getLogStorage();

			betaStat.mat_.block(k * sizeClassBeta, 0, sizeClassBeta, nStat) = class_[k].betaParamStat().getStatStorage();
//			betaLog.block(k * sizeClassBeta, 0, sizeClassBeta, nObs) = class_[k].betaParamStat().getLogStorage();

			sdStat.mat_.block(k * sizeClassSd, 0, sizeClassSd, nStat) = class_[k].sdParamStat().getStatStorage();
//			sdLog.block(k * sizeClassSd, 0, sizeClassSd, nObs) = class_[k].sdParamStat().getLogStorage();
		}

		outG_.add_payload( { "variable", "param", idName_ }, "paramStr", paramStr_);

		outG_.add_payload( { "variable", "param", idName_, "alpha" }, "stat", alphaStat);
		outG_.add_payload( { "variable", "param", idName_, "beta" }, "stat", betaStat);
		outG_.add_payload( { "variable", "param", idName_, "sd" }, "stat", sdStat);
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
			std::cout << itString(tik) << std::endl;
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
				sstm0 << "k: " << k << ", s: " << s << ", alpha0";
				names[k * nSub_ * 2 + s * 2] = sstm0.str();

				std::stringstream sstm;
				sstm << "k: " << k << ", s: " << s << ", alpha1";
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
					sstm << "k: " << k << ", s: " << s << ", c: " << c;
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
				sstm << "k: " << k << ", s: " << s;
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

#endif // MIXT_FUNCTIONALMIXTURE
