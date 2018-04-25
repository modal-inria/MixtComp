/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 2, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *
 **/

#ifndef MIXT_MIXTURECOMPOSER_H
#define MIXT_MIXTURECOMPOSER_H

#include <set>
#include <vector>

#include "../Data/mixt_ZClassInd.h"
#include "../Data/mixt_ClassDataStat.h"
#include "../Sampler/mixt_ClassSampler.h"
#include "../Mixture/mixt_IMixture.h"
#include "../Various/mixt_Enum.h"
#include "../Param/mixt_ConfIntParamStat.h"

namespace mixt {

typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
typedef std::vector<IMixture*>::iterator MixtIterator;

class MixtureComposer {
public:
	/** Constructor.
	 * @param nbCluster,nbSample,nbVariable number of clusters, samples and Variables
	 */
	MixtureComposer(Index nbInd, Index nbClass, Real confidenceLevel);

	/** copy constructor.
	 *  @param composer the composer to copy
	 */
	MixtureComposer(MixtureComposer const& composer);

	/** The registered mixtures will be deleted there.*/
	~MixtureComposer();

	Index nbClass() const {
		return nClass_;
	}

	Index nbInd() const {
		return nInd_;
	}

	Index nbVar() const {
		return nVar_;
	}

	/** @return  the zi class label */
	const Vector<Index>* p_zi() const {
		return &(zClassInd_.zi().data_);
	}

	/** @return  the zi class label */
	const Vector<std::set<Index> >& classInd() const {
		return zClassInd_.classInd();
	}

	/** @return a constant reference on the vector of mixture */
	const std::vector<IMixture*>& v_mixtures() const {
		return v_mixtures_;
	}

	/** Compute the proportions and the model parameters given the current tik
	 *  mixture parameters.
	 *  @param[out] worstDeg worst degeneracy type incountered among all mixtures for this mStep
	 **/
	std::string mStep(const Vector<std::set<Index>>& classInd);
	std::string mStep();

	/** Compute proportions using the ML estimator, default implementation. Set
	 *  as virtual in case we impose fixed proportions in derived model. Only called
	 *  by mStep
	 **/
	void mStepPi();

	/**
	 * Simulate zi accordingly to tik and replace tik by zik by calling cStep().
	 * @return the minimal value of individuals in a class
	 **/
	void sampleZ();
	void sampleZ(int i);

	/** compute Tik */
	void eStepCompleted();

	/**
	 * The return object is a list of the variables for which the completed probability is null
	 * in every class. This only occurs if there is an implementation bug in the model. The kind of
	 * checks that are performed are similar to those performed in eStepObservedInd
	 */
	bool eStepCompletedInd(int i);

	/** @return the value of the probability of the i-th sample in the k-th component.
	 *  @param i index of the sample
	 *  @param k index of the component
	 **/
	Real lnCompletedProbability(int i, int k) const;

	Real lnObservedProbability(int i, int k) const;

	/** @return the value of the observed likelihood */
	Real lnObservedLikelihood();

	/** @return the value of the completed likelihood */
	Real lnCompletedLikelihood();

	/** write the parameters of the model in the stream os. */
	void writeParameters() const;

	/** @brief compute the number of free parameters of the model.
	 *  lookup on the mixtures and sum the nbFreeParameter.
	 **/
	int nbFreeParameters() const;

	/**
	 * Perform a sampling step
	 * @param checkSampleCondition indicates if there is a need to perform a check on the data or not
	 */
	void sampleUnobservedAndLatent();
	void sampleUnobservedAndLatent(int i);

	/**
	 * Check if the data conditions are verified for all mixtures. Providing a log is required during
	 * initialization. The rejection sampling / Gibbs sampling will ensure that at later steps the
	 * data conditions are verified, hence there is no need to provide information to the user, and no
	 * log is required.
	 * @param[out] warnLog provides information on what condition has not been met
	 * */
	std::string checkSampleCondition(
			const Vector<std::set<Index>>& classInd) const;
	std::string checkSampleCondition() const;

	/**
	 * Check if there are enough individual in each class. Called by checkSampleCondition.
	 * @param[out] warnLog provides information on what condition has not been met
	 * */
	std::string checkNbIndPerClass(
			const Vector<std::set<Index>>& classInd) const;
	std::string checkNbIndPerClass() const;

	/**@brief This step can be used to signal to the mixtures that they must
	 * store results. This is usually called after a burn-in phase.
	 **/
	void storeSEMRun(int iteration, int iterationMax, RunType runType);

	/** @brief This step can be used to signal to the mixtures that they
	 * must store data. This is usually called after the long algo, to
	 * store data generated using the estimated parameters during a Gibbs sampling
	 */
	void storeGibbsRun(Index iteration, Index iterationMax);

	/** DataHandler is injected to take care of setting the values of the latent classes.
	 * This avoids templating the whole composer with DataHandler type, as is currently done
	 * with the individual IMixtures
	 * @param checkInd should be set to 1 if a minimum number of individual per class should be
	 * enforced at sampling (true in learning, false in prediction) */
	template<typename ParamSetter, typename DataHandler>
	std::string setDataParam(const ParamSetter& paramSetter,
			const DataHandler& dataHandler, RunMode mode) {
		std::string warnLog;

		for (ConstMixtIterator it = v_mixtures_.begin();
				it != v_mixtures_.end(); ++it) {
			warnLog += (*it)->setDataParam(mode);
		}

		warnLog += setProportion(paramSetter);

		for (int i = 0; i < nInd_; ++i) {
			tik_.row(i) = prop_.transpose();
		}

		warnLog += setZi(dataHandler); // dataHandler getData is called to fill zi_

		if (mode == prediction_) { // in prediction, paramStatStorage_ will not be modified later during the run
			paramStat_.setParamStorage(); // paramStatStorage_ is set now, and will not be modified further during predict run
		}

		paramStr_ = "nModality: " + std::to_string(nClass_);

		dataStat_.setNbIndividual(nInd_);

		return warnLog;
	}

	/**
	 * ParamSetter is injected to take care of setting the values of the proportions.
	 * This avoids templating the whole composer with DataHandler type, as is currently done
	 * with the individual IMixtures.
	 */
	template<typename ParamSetter>
	std::string setProportion(const ParamSetter& paramSetter) {
		std::string warnLog;

		std::string dummy;
		paramSetter.getParam("z_class", "pi", prop_, dummy); // paramStr_ is manually set at the end of setDataParam

		return warnLog;
	}

	/**
	 * DataHandler is injected to take care of setting the values of the latent classes.
	 * This avoids templating the whole composer with DataHandler type, as is currently done
	 * with the IMixture subtypes.
	 * @param checkInd should be set to 1 if a minimum number of individual per class should be
	 * enforced at sampling (true in learning, false in prediction)
	 */
	template<typename DataHandler>
	std::string setZi(const DataHandler& dataHandler) {
		std::string warnLog;
		std::string dummyParam;

		if (dataHandler.info().find("z_class") == dataHandler.info().end()) { // z_class was not provided
			zClassInd_.setAllMissing(); // set every value state to missing_
		} else { // z_class was provided and its value is acquired in zi_
			warnLog += zClassInd_.setZi(dataHandler);
		}

		std::string tempLog = zClassInd_.checkMissingType(); // check if the missing data provided are compatible with the model
		if (tempLog.size() > 0) {
			std::stringstream sstm;
			sstm << "Variable " << idName_
					<< " contains latent classes and has unsupported missing value types.\n"
					<< tempLog;
			warnLog += sstm.str();
		}
		zClassInd_.computeRange(); // compute effective range of the data for checking, min and max will be set to 0 if data is completely missing
		if (zClassInd_.zi().dataRange_.min_ < 0) { // Since z is currently described using unsigned integer, there is no need for this check HOWEVER it might come in handy shall this condition changes
			std::stringstream sstm;
			sstm
					<< "The z_class latent class variable has a lowest provided value of: "
					<< minModality + zClassInd_.zi().dataRange_.min_
					<< " while the minimal value has to be: " << minModality
					<< ". Please check the encoding of this variable to ensure proper bounds."
					<< std::endl;
			warnLog += sstm.str();
		}
		if (zClassInd_.zi().dataRange_.hasRange_ == true
				|| zClassInd_.zi().dataRange_.max_ > nClass_ - 1) {
			std::stringstream sstm;
			sstm
					<< "The z_class latent class variable has a highest provided value of: "
					<< minModality + zClassInd_.zi().dataRange_.max_
					<< " while the maximal value can not exceed the number of class: "
					<< minModality + nClass_ - 1
					<< ". Please check the encoding of this variable to ensure proper bounds."
					<< std::endl;
			warnLog += sstm.str();
		}
		zClassInd_.setRange(0, nClass_ - 1, nClass_);

		return warnLog;
	}
	;

	/**@brief This step can be used to ask each mixture to export its model parameters
	 * and data
	 **/
	template<typename DataExtractor, typename ParamExtractor>
	void exportDataParam(DataExtractor& dataExtractor,
			ParamExtractor& paramExtractor) const {

		dataExtractor.exportVals(0, "z_class", zClassInd_.zi(), tik_);
		paramExtractor.exportParam(0, "z_class", "pi",
				paramStat_.getStatStorage(), paramStat_.getLogStorage(),
				paramName(), confidenceLevel_, "");

		for (ConstMixtIterator it = v_mixtures_.begin();
				it != v_mixtures_.end(); ++it) {
			(*it)->exportDataParam();
		}

	}
	;

	/** register a mixture to the composer.
	 *  When a mixture is registered, the composer:
	 *  - assign composer pointer (itself) to the mixture
	 *  - add it to v_mixtures_
	 *  @note the mixture is not initialized, so don't forget to call
	 **/
	void registerMixture(IMixture* mixture);

	/** @return names of the parameters */
	std::vector<std::string> paramName() const;

	/**
	 * @return names of the mixtures
	 * */
	std::vector<std::string> mixtureName() const;

	void initData();

	/**
	 * Initialize the parameters for models that requires it. For example models where the parameters areestimated through an iterative
	 * process, either an optimization (functional data model), or a sampling (rank data model).
	 */
	void initParam();

	/**
	 * Generate a partition of a subset of the data set. This is used to perform a partial mStep that adds variability to
	 * the initialization procedure.
	 * @param ratio n elements in subset / n elements in data set
	 */
	std::string initParamSubPartition(Real ratio);

	/**
	 * Compute the "raw" class ID matrix E_kj
	 *
	 *@param[out] ekj matrix containing E_kj
	 * */
	void E_kj(Matrix<Real>& ekj) const;
	/**
	 * Added by Matt (Compute the matrix delta measuring the similarities between variables)
	 *
	 *@param[out] delta matrix of the distances between variables
	 **/
	void Delta(Matrix<Real>& delta) const;

	/**
	 * Compute the normalized IDClass matrix, using
	 *
	 *@param[out] idc matrix containing the class id description
	 * */
	void IDClass(Matrix<Real>& idc) const;

	void lnProbaGivenClass(Matrix<Real>& idc) const;

	void printTik() const;

	void observedTik(Vector<Real>& observedTik) const;

	void setObservedProbaCache();

	/**
	 * Ask for every model that needs it to compute an empirical observed distribution.
	 */
	void computeObservedProba();

	/**
	 * Every steps to be completed to initialize latent variables once parameters are known
	 * and uniform initialization of data has been performed. */
	std::string initializeLatent();

	/**
	 * Similar to eStep, except that observed probability are used. Useful during initialization
	 * when individuals have not been completed using the real model.
	 */
	std::string eStepObserved();
	bool eStepObservedInd(Index i);

	/** Call initializeMarkovChain on all variables. */
	void initializeMarkovChain();

	const Vector<Real>& completedProbabilityLogBurnIn() {
		return completedProbabilityLogBurnIn_;
	}
	const Vector<Real>& completedProbabilityLogRun() {
		return completedProbabilityLogRun_;
	}
	void printClassInd() const;

private:
	/** name of the latent class variable */
	std::string idName_;

	std::string paramStr_;

	/** number of classes */
	Index nClass_;

	/** Number of samples */
	Index nInd_;

	/** Number of variables */
	Index nVar_;

	/** The proportions of each class */
	Vector<Real> prop_;

	/** The tik probabilities */
	Matrix<Real> tik_;

	/** Combination of zi_ and ClassInd_ */
	ZClassInd zClassInd_;

	/** class sampler */
	ClassSampler sampler_;

	/** vector of pointers to the mixtures components */
	std::vector<IMixture*> v_mixtures_;

	/** computer parameters statistics */
	ConfIntParamStat<Vector<Real> > paramStat_;

	/** computer of the statistics on latent variables */
	ClassDataStat dataStat_;

	/** confidence level used for the computation of statistics */
	Real confidenceLevel_;

	/**
	 * Cached observed log probability. The access is done via:
	 * observedProbabilityCache_(variable)(individual, class) to more or less match the t_ik access
	 * pattern.
	 * */
	Vector<Matrix<Real> > observedProbabilityCache_;

	/** Cached completed log probability for each individual, can be used to export the evolution of the completed likelihood of the data, iteration after iteration. */
	Vector<Real> completedProbabilityCache_;

	/** Cached completed log probability for each individual, can be used to export the evolution of the completed likelihood of the data, iteration after iteration. */
	Vector<Real> completedProbabilityLogBurnIn_;

	/** Cached completed log probability for each individual, can be used to export the evolution of the completed likelihood of the data, iteration after iteration. */
	Vector<Real> completedProbabilityLogRun_;
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H */
