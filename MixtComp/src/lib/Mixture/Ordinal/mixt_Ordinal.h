/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 22, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_ORDINAL
#define MIXT_ORDINAL

#include "boost/regex.hpp"

#include <algorithm>

#include "Data/mixt_AugmentedData.h"
#include "Data/mixt_ConfIntDataStat.h"
#include "Param/mixt_ConfIntParamStat.h"
#include "Mixture/mixt_IMixture.h"
#include "mixt_BOSPath.h"

namespace mixt {

template<typename DataHandler, typename DataExtractor, typename ParamSetter,
		typename ParamExtractor>
class Ordinal: public IMixture {

public:
	/** constructor **/
	Ordinal(Index indexMixture, std::string const& idName, Index nbClass,
			const DataHandler* p_handler, DataExtractor* p_extractor,
			const ParamSetter* p_paramSetter, ParamExtractor* p_paramExtractor,
			Real confidenceLevel) :
			IMixture(indexMixture, idName), nClass_(nbClass), nModality_(0), augData_(), nbInd_(
					0), // number of individuals will be set during setDataParam
			confidenceLevel_(confidenceLevel), mu_(nbClass), pi_(nbClass), dataStatComputer_(
					augData_, confidenceLevel), muParamStatComputer_(mu_,
					confidenceLevel), piParamStatComputer_(pi_,
					confidenceLevel), p_handler_(p_handler), p_dataExtractor_(
					p_extractor), p_paramSetter_(p_paramSetter), p_paramExtractor_(
					p_paramExtractor) {
	}

	/* Debug constructor with direct data set */
	Ordinal(Index nbClass, Index nbInd, Index nbModalities, int mu, Real pi) :
			IMixture(0, "dummy"), nClass_(nbClass), nModality_(nbModalities), nbInd_(
					nbInd), mu_(nbClass, mu), pi_(nbClass, pi), dataStatComputer_(
					augData_, 1.), muParamStatComputer_(mu_, 1.), piParamStatComputer_(
					pi_, 1.), p_handler_(NULL), p_dataExtractor_(NULL), p_paramSetter_(
			NULL), p_paramExtractor_(NULL) {
		path_.resize(nbInd);
		for (int i = 0; i < nbInd; ++i) { // initialization of the paths
			path_(i).setInit(0, nbModalities - 1);
			path_(i).setEnd(0, nbModalities - 1); // no constraint on values
			path_(i).initPath(); // random init, with uniform z = 0

			for (int n = 0; n < nbGibbsIniBOS; ++n) {
				path_(i).samplePath(mu, pi, sizeTupleBOS);
			}
		}
	}

	const Vector<int>& mu() const {
		return mu_;
	}
	const Vector<Real>& pi() const {
		return pi_;
	}

	std::string setDataParam(RunMode mode) {
		std::string warnLog;

		warnLog += p_handler_->getData(idName(), augData_, nbInd_, paramStr_,
				-minModality); // ordinal data are modalities, offset enforces 0-based encoding through the whole mixture

		if (warnLog.size() > 0) {
			return warnLog;
		}

		augData_.computeRange();
		std::string missingLog = augData_.checkMissingType(acceptedType()); // check if the missing data provided are compatible with the model

		if (missingLog.size() > 0) { // check on the missing values description
			std::stringstream sstm;
			sstm << "Variable " << idName_
					<< " with Ordinal model has a problem with the descriptions of missing values."
					<< std::endl << missingLog;
			warnLog += sstm.str();
		}

		if (mode == prediction_) { // prediction mode
			setParam(); // set mu_ and pi_ using p_paramSetter_. paramStr_ from getData might be overwritten here. nModality_ is not set here, as this will depend on the content of paramStr_

			muParamStatComputer_.setParamStorage();
			piParamStatComputer_.setParamStorage();
		}

		if (paramStr_.size() == 0) { // no paramStr_ provided in learning, parameters space is deduced from data. paramStr_ is generated.
			nModality_ = augData_.dataRange_.max_ + 1;

			std::stringstream sstm;
			sstm << "nModality: " << nModality_;
			paramStr_ = sstm.str(); // paramStr_ must be generated from the data, for future use and export for prediction
		} else { // paramStr_ has been provided. It must be parsed, and data validity must be checked, and data ranges must be extended to be compatible with
			std::string nModStr = std::string("nModality: *")
					+ strPositiveInteger; // parse paramStr here. If empty, deduce from data, if not empty, check that data UPPER BOUND is compatible with this information
			boost::regex nModRe(nModStr);
			boost::smatch matchesVal;

			if (boost::regex_match(paramStr_, matchesVal, nModRe)) { // value is present
				nModality_ = str2type<int>(matchesVal[1].str());
			} else {
				std::stringstream sstm;
				sstm << "Variable: " << idName_
						<< " parameter string is not in the correct format, which should be \"nModality: x\" "
						<< "with x the number of modalities in the variable."
						<< std::endl;
				warnLog += sstm.str();
			}

			if (nModality_ <= augData_.dataRange_.max_) {
				std::stringstream sstm;
				sstm << "Variable: " << idName_
						<< " requires a maximum value of "
						<< minModality + nModality_ - 1 << " "
						<< "for the data during prediction. This maximum value corresponds to the maximum value used during the learning phase. "
						<< "The maximum value in the data provided for prediction is : "
						<< minModality + augData_.dataRange_.max_ << std::endl;
				warnLog += sstm.str();
			}
		}

		if (augData_.dataRange_.min_ < 0) { // modality encoding is 0-based, so in any case, values below 0 are erroneous
			std::stringstream sstm;
			sstm << "Variable: " << idName_
					<< " is described by an Ordinal model which requires a minimum value of 0 in either provided values or bounds. "
					<< "The minimum value currently provided is : "
					<< augData_.dataRange_.min_ + minModality << std::endl;
			warnLog += sstm.str();
		}

		if (nModality_ < 3) {
			std::stringstream sstm;
			sstm << "Variable: " << idName_
					<< " requires a minimum of 3 modalities. If you have less modalities than that, you must use a "
					<< "Categorical model." << std::endl;
			warnLog += sstm.str();
		}

		augData_.dataRange_.min_ = 0; // Once everything has been set, adjust the range of data to align with the parameter space
		augData_.dataRange_.max_ = nModality_ - 1;
		augData_.dataRange_.range_ = nModality_;

		setPath(); // initialize the BOSPath vector elements with data gathered from the AugmentedData
		dataStatComputer_.setNbIndividual(nbInd_);

		return warnLog;
	}

	/**
	 * Use information in AugmentedData to set the values of every path in path_. Called at the end
	 * of setDataParam
	 * */
	void setPath() {
#ifdef MC_DEBUG
		std::cout << "Ordinal::setPath" << std::endl;
		std::cout << "path_.size(): " << path_.size() << std::endl;
#endif
		path_.resize(nbInd_);

		for (int i = 0; i < nbInd_; ++i) {
			path_(i).setInit(0, augData_.dataRange_.max_); // every initial segment is the same and spans all the modalities
			if (augData_.misData_(i).first == present_) { // final value is set
				path_(i).setEnd(augData_.data_(i), augData_.data_(i));
			} else if (augData_.misData_(i).first == missing_) {
				path_(i).setEnd(0, augData_.dataRange_.max_); // final interval is the same as initial interval
			} else if (augData_.misData_(i).first == missingIntervals_) {
				path_(i).setEnd(augData_.misData_(i).second[0],
						augData_.misData_(i).second[1]); // bounds of the interval are provided
			}
		}
	}

	/** get parameters from single table, then dispatch it to mu_ and pi_ */
	void setParam() {
		Vector<Real> param;
		p_paramSetter_->getParam(idName(), // parameters are set using results from previous run
				"muPi", param, paramStr_);
		mu_.resize(nClass_);
		pi_.resize(nClass_);
		for (int k = 0; k < nClass_; ++k) {
			mu_(k) = param(2 * k);
			pi_(k) = param(2 * k + 1);
		}
	}

	/**
	 * The sampling sequence is:
	 * - Ordinal::samplingStepNoCheck
	 * - Ordinal::GibbsSampling
	 * - either BOSPath::forwardSamplePath
	 * - or BOSPath::samplePath, samples a set of latent variables, from the distribution provided by BOSPath::tupleMultinomial
	 *   - BOSPath::tupleMultinomial, generates the various cases by calling tupleMultinomial, then compute a multinomial distribution
	 *   - nodeMultinomial, recursive function, computes the probability of a path
	 *   - endMultinomial, returns the probability of current configuration
	 * */
	void sampleUnobservedAndLatent(Index ind, Index k) {
		GibbsSampling(ind, mu_(k), pi_(k)); // in samplingStepCheck, each sampling must result in a valid state
		copyToData(ind);
	}

	/** storeSEMRun sets new parameters at the last iteration of the SEM, before the Gibbs. */
	void storeSEMRun(Index iteration, Index iterationMax) {
		muParamStatComputer_.sampleParam(iteration, iterationMax);
		piParamStatComputer_.sampleParam(iteration, iterationMax);

		if (iteration == iterationMax) { // at last iteration, compute the observed probability distribution logProba_
			muParamStatComputer_.setExpectationParam(); // estimate mu parameter using mode / expectation
			piParamStatComputer_.setExpectationParam(); // estimate pi parameter using mode / expectation
		}
	}

	void computeObservedProba() {
		observedProba_.resize(nClass_, nModality_);
		BOSPath path; // BOSPath used for the various samplings
		path.setInit(0, nModality_ - 1);
		for (int k = 0; k < nClass_; ++k) {
			RowVector<Real> nbInd(nModality_); // observed frequencies
			path.setEnd(k, k);
			nbInd = 0;
			for (int i = 0; i < nbSampleObserved; ++i) {
				path.forwardSamplePath(mu_(k), // complete the individual
				pi_(k)); // to estimate probability distribution, all z can be sampled to 1
				nbInd(path.c()(nModality_ - 2).e_(0)) += 1.; // register the x value, for marginalization
			}
			observedProba_.row(k) = nbInd / Real(nbSampleObserved);
		}
	}

	void storeGibbsRun(Index ind, Index iteration, Index iterationMax) {
		dataStatComputer_.sampleVals(ind, iteration, iterationMax); // ConfIntStat called to sample value
		if (iteration == iterationMax) {
			dataStatComputer_.imputeData(ind); // impute the missing values using empirical mean
		}
	}

	Real lnCompletedProbability(Index i, Index k) const {
		return path_(i).computeLogProba(mu_(k), pi_(k)); // path_(i) contains a completed individual
	}

	/**
	 * This function must be defined to return the observed likelihood
	 * @return the observed log-likelihood
	 */
	Real lnObservedProbability(Index i, Index k) const {
#ifdef MC_DEBUG
		std::cout << "Ordinal::lnobservedProbability" << std::endl;
		std::cout << "observedProba_.rows(): " << observedProba_.rows() << ", observedProba_.cols(): " << observedProba_.cols() << std::endl;
		std::cout << "augData_.data_(i): " << augData_.data_(i) << std::endl;
#endif
		if (augData_.misData_(i).first == present_) { //
			return std::log(observedProba_(k, augData_.data_(i))); // marginalized only over c_i
		} else if (augData_.misData_(i).first == missing_) { // marginalized over c_i and all modalities: proba is 1.
			return 0.;
		} else if (augData_.misData_(i).first == missingIntervals_) { // marginalized over c_i and the observed interval
			Real proba = 0;
			for (int p = augData_.misData_(i).second[0];
					p < augData_.misData_(i).second[1] + 1; ++p) {
				proba += observedProba_(k, p);
			}
			return std::log(proba);
		}
		return std::numeric_limits<Real>::signaling_NaN(); // fail case
	}

	Index nbFreeParameter() const {
		return nClass_; // only the continuous pi_ parameter is taken into account, not the discrete mu_ parameter
	}

	void writeParameters() const {
		std::stringstream sstm;
		for (int k = 0; k < nClass_; ++k) {
			sstm << "Class: " << k << std::endl;
			sstm << "mu: " << mu_(k) << std::endl;
			sstm << "pi: " << pi_(k) << std::endl;
		}

		std::cout << sstm.str() << std::endl;
	}

	void exportDataParam() const {
		int nbColStat = muParamStatComputer_.getStatStorage().cols();
		Matrix<Real> paramStatStorage(2 * nClass_, nbColStat); // aggregates both mu and pi values
		for (int j = 0; j < nbColStat; ++j) {
			for (int k = 0; k < nClass_; ++k) {
				paramStatStorage(2 * k, j) =
						muParamStatComputer_.getStatStorage()(k, j)
								+ minModality;
				paramStatStorage(2 * k + 1, j) =
						piParamStatComputer_.getStatStorage()(k, j);
			}
		}

		int nbColLog = muParamStatComputer_.getLogStorage().cols();
		Matrix<Real> paramLogStorage(2 * nClass_, nbColLog); // aggregates both mu and pi logs
		for (int j = 0; j < nbColLog; ++j) {
			for (int k = 0; k < nClass_; ++k) {
				paramLogStorage(2 * k, j) =
						muParamStatComputer_.getLogStorage()(k, j)
								+ minModality;
				paramLogStorage(2 * k + 1, j) =
						piParamStatComputer_.getLogStorage()(k, j);
			}
		}

		p_dataExtractor_->exportVals(indexMixture_, true, // hasModalities
				idName(), augData_, dataStatComputer_.getDataStatStorage()); // export the obtained data using the DataExtractor
		p_paramExtractor_->exportParam(indexMixture_, idName(), "muPi",
				paramStatStorage, paramLogStorage, paramNames(),
				confidenceLevel_, paramStr_);
	}

	std::vector<std::string> paramNames() const {
		std::vector<std::string> names(nClass_ * 2);
		for (int k = 0; k < nClass_; ++k) {
			std::stringstream sstmMean, sstmSd;
			sstmMean << "k: " << k + minModality << ", mu: ";
			sstmSd << "k: " << k + minModality << ", pi";
			names[2 * k] = sstmMean.str();
			names[2 * k + 1] = sstmSd.str();
		}
		return names;
	}

	/**
	 * initData is called at the beginning of of SemStrategy or GibbsStrategy. It does not use
	 * any parameters. Since BOSPath::initPath initializes all BOSPath with z = 0 to enforce
	 * validity, initializeMarkovChain which is called after will ensure more variability in the
	 * latent variables.
	 * */
	void initData(Index i) {
		path_(i).initPath(); // remove missing use to initialize learn, and should therefore use BOSPath::initPath() which is parameters free. Problem is that z = 0 everywhere.
	}
	;

	void initParam() {
		for (Index k = 0; k < nClass_; ++k) {
			mu_(k) = 0;
			pi_(k) = 0.75;
		}
	}

	void initializeMarkovChain(Index i, Index k) {
		initBOS(i, k); // Gibbs sampling iterations to avoid estimating too closely to 0 in first iterations
	}

	/**
	 * During initialization of observation, z is set to 0, to avoid generating 0 probability individuals.
	 * However, a mStep performed on such a data set would always yield an estimated value of 0 for z. */
	void initBOS(Index i, Index k) {
		for (int n = 0; n < nbGibbsIniBOS; ++n) { // n rounds of Gibbs sampling to increase variability on z
			GibbsSampling(i, mu_(k), pi_(k));
		}
		copyToData(i);
	}

	Vector<bool> acceptedType() {
		Vector<bool> at(nb_enum_MisType_);
		at(0) = true; // present_,
		at(1) = true; // missing_,
		at(2) = false; // missingFiniteValues_,
		at(3) = true; // missingIntervals_,
		at(4) = false; // missingLUIntervals_,
		at(5) = false; // missingRUIntervals_,
		return at;
	}

	std::string checkSampleCondition(
			const Vector<std::set<Index> >& classInd) const {
		if (degeneracyAuthorizedForNonBoundedLikelihood)
			return "";

		for (Index k = 0; k < nClass_; ++k) {
			bool allZ0 = true; // are all z = 0 in the current class ?
			bool allZ1 = true; // are all z = 1 in the current class ?
			for (std::set<Index>::const_iterator it = classInd(k).begin(), itE =
					classInd(k).end(); it != itE; ++it) {
				switch (path_(*it).allZ()) { // what can be deduced from the current path ?
				case allZ0_: {
					allZ1 = false;
				}
					break;

				case allZ1_: {
					allZ0 = false;
				}
					break;

				case mixZ0Z1_: { // this ensure an immediate end of testing
					allZ0 = false;
					allZ1 = false;
				}
					break;
				}

				if (allZ0 == false && allZ1 == false) { // there is enough variability on z in this class to ensure that pi will be estimated inside the open support
					goto itKEnd;
					// no need to further examine the content of individual paths
				}
			} // end of the loop on all observations

			return "Error in variable: " + idName_
					+ " with Ordinal model. A latent variable (the accuracy z) is uniformly 0 or 1 in at least one class. If the number of modalities is quite low, try using a categorical model instead."
					+ eol;

			itKEnd: ; // reached only if and only if all values of z have been observed in the current class
		}

		return "";
	}

	std::string mStep(const Vector<std::set<Index> >& classInd) {
		mStepMu(classInd);
		mStepPi(classInd);

		return "";
	}

	bool sampleApproximationOfObservedProba() {return true;}
private:
	/**
	 * Perform one iteration of Gibbs sampling, insuring proper implementation of allZOneAuthorized flag
	 *
	 * @param sampleAZ indicate if the condition all z = 0 or all z = 1 is authorized for the class containing ind
	 * */
	void GibbsSampling(int ind, int mu, Real pi) {
		if (augData_.misData_(ind).first == missing_) { // if individual is completely missing, use samplePathForward instead of samplePath to accelerate computation
			path_(ind).forwardSamplePath(mu, pi);
		} else { // perform one round of Gibbs sampler for the designated individual
			path_(ind).samplePath(mu, pi, sizeTupleBOS);
		}
	}

	/**
	 * Estimation of mu is performed by an exhaustive
	 * computation of the likelihood for every value of mu to get the argmax
	 */
	void mStepMu(const Vector<std::set<Index> >& classInd) {
		for (Index k = 0; k < nClass_; ++k) {
			Vector<Real> logLik(nModality_, 0.);
			Real currPi = pi_(k);
			for (std::set<Index>::const_iterator it = classInd(k).begin(),
					itEnd = classInd(k).end(); it != itEnd; ++it) {
				for (Index mu = 0; mu < nModality_; ++mu) {
					logLik(mu) += path_(*it).computeLogProba(mu, currPi);
				}
			}

			Vector<Real> proba;
			proba.logToMulti(logLik);
			mu_(k) = multi_.sample(proba);
		}
	}

	/**
	 * Estimation of pi by maximum likelihood in all classes
	 * */
	void mStepPi(const Vector<std::set<Index> >& classInd) {
		for (Index k = 0; k < nClass_; ++k) {
			mStepPiK(classInd, k);
		}
	}

	/**
	 * Estimation of pi by maximum likelihood in a particular class
	 *
	 * @param k class for which the parameter pi should be estimated
	 * */
	void mStepPiK(const Vector<std::set<Index> >& classInd, Index k) {
		Real nodePerClass = 0.; // total number of nodes in class k
		Real zPerClass = 0.; // total of nodes with z = 1 in class k

		for (std::set<Index>::const_iterator it = classInd(k).begin(), itEnd =
				classInd(k).end(); it != itEnd; ++it) {
			zPerClass += path_(*it).nbZ(); // add only z = 1 nodes of the individual
			nodePerClass += path_(*it).nbNode(); // add all nodes of the individual
		}

		pi_(k) = zPerClass / nodePerClass; // from accounts to frequencies of z -> maximum likelihood estimate of pi
	}

	/** update the data using the last segment in c_. to be used after sampling in the BOSPath. */
	void copyToData(int ind) {
		augData_.data_(ind) = path_(ind).c()(nModality_ - 2).e_(0); // copy of the data from last element of path to augData, which will be useful for the dataStatComputer_ to compute statistics
	}

	/** Number of classes */
	int nClass_;

	/** Number of modalities */
	int nModality_;

	/** The augmented data set */
	AugmentedData<Vector<int> > augData_;

	/** Vector containing path for individuals */
	Vector<BOSPath> path_;

	/** Matrix containing observed log probability distribution
	 * Modalities in rows
	 * Classes in columns */
	Matrix<Real> observedProba_;

	/** Number of samples in the data set*/
	Index nbInd_;

	/** Optional string parameters */
	std::string paramStr_;

	/** Confidence level used in computation of parameters and missing values statistics */
	Real confidenceLevel_;

	/** Mode parameter, one element per class */
	Vector<int> mu_;

	/** Precision parameter, one element per class */
	Vector<Real> pi_;

	/** Sampler for reinitialization of parameters after degeneracy */
	MultinomialStatistic multi_;

	/** Compute the statistics on missing data during GibbsRun phase */
	ConfIntDataStat<int> dataStatComputer_;

	/** Compute the statistics on mu parameter */
	ConfIntParamStat<Vector<int> > muParamStatComputer_;

	/** Compute the statistics on pi parameter */
	ConfIntParamStat<Vector<Real> > piParamStatComputer_;

	/** Pointer to the data handler */
	const DataHandler* p_handler_;

	/** Pointer to the data extractor */
	DataExtractor* p_dataExtractor_;

	/** Pointer to the param setter */
	const ParamSetter* p_paramSetter_;

	/** Pointer to the parameters extractor */
	ParamExtractor* p_paramExtractor_;
};

} // namespace mixt

#endif // MIXT_ORDINAL
