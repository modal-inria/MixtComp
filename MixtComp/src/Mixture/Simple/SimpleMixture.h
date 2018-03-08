/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SIMPLEmodel_H
#define SIMPLEmodel_H

#include "Mixture/mixt_IMixture.h"
#include "Data/mixt_AugmentedData.h"
#include "IO/mixt_IO.h"
#include "Various/mixt_Constants.h"
#include "Param/mixt_ConfIntParamStat.h"

namespace mixt {

/**
 * SimpleMixture is the new interface to add simple models. It is simpler than SimpleMixtureBridge which used class traits for no apparent benefit.
 */
template<
	typename Model,
	typename DataHandler,
	typename DataExtractor,
	typename ParamSetter,
	typename ParamExtractor>
class SimpleMixture : public IMixture {
public:
	/** constructor.
	 *  @param idName id name of the mixture
	 *  @param nbCluster number of cluster
	 **/
	SimpleMixture(
			Index indexMixture,
			std::string const& idName,
			Index nbClass,
			const DataHandler* p_handler,
			DataExtractor* p_extractor,
			const ParamSetter* p_paramSetter,
			ParamExtractor* p_paramExtractor,
			Real confidenceLevel) :
				IMixture(indexMixture, idName),
				nbClass_(nbClass),
				param_(), // must be initialized here, as will immediately be resized in model_ constructor
				model_(
						idName,
						nbClass,
						param_),
						augData_(),
						nbInd_(0),
						confidenceLevel_(confidenceLevel),
				sampler_(augData_, param_, nbClass),
						dataStat_(augData_, confidenceLevel),
						paramStat_(param_, confidenceLevel),
				likelihood_(
						param_,
						augData_,
						nbClass),
						p_handler_(p_handler),
						p_dataExtractor_(p_extractor),
						p_paramSetter_(p_paramSetter),
						p_paramExtractor_(p_paramExtractor) {}

	/**
	 * This function will be defined to set the data into your data containers.
	 * To facilitate data handling, framework provide templated functions,
	 * that can be called directly to get the data.
	 */
	std::string setDataParam(RunMode mode) {
		std::string warnLog;
		warnLog += p_handler_->getData(
				idName(),
				augData_,
				nbInd_,
				paramStr_,
				(model_.hasModalities()) ? (-minModality) : (0)); // minModality offset for categorical models

		if (warnLog.size() > 0) {
			return warnLog;
		}
		augData_.computeRange();
		std::string tempLog  = augData_.checkMissingType(model_.acceptedType()); // check if the missing data provided are compatible with the model

		if(tempLog.size() > 0) { // check on the missing values description
			std::stringstream sstm;
			sstm << "Variable " << idName() << " has a problem with the descriptions of missing values." << std::endl << tempLog;
			warnLog += sstm.str();
		}

		if (mode == prediction_) {
			p_paramSetter_->getParam(
					idName_, // parameters are set using results from previous run
					"NumericalParam",
					param_,
					paramStr_); // note that in the prediction case, the eventual paramStr_ obtained from p_handler_->getData is overwritten by the one provided by the parameter structure from the learning

			paramStat_.setParamStorage(); // paramStatStorage_ is set now, using dimensions of param_, and will not be modified during predict run by the paramStat_ object for some mixtures, there will be errors if the range of the data in prediction is different from the range of the data in learning in the case of modalities, this can not be performed earlier, as the max val is computed at model_.setModalities(nbParam)
		}

		warnLog +=
				model_.setData( // checks on data bounds are made here
						paramStr_,
						augData_,
						mode);

		dataStat_.setNbIndividual(nbInd_);

		return warnLog;
	}

	virtual void sampleUnobservedAndLatent(Index ind, Index k) {
		sampler_.samplingStepNoCheck(
				ind,
				k);
	}

	/**
	 * Estimate parameters by maximum likelihood
	 */
	virtual void mStep(const Vector<std::set<Index> >& classInd) {
		model_.mStep(classInd);
	}

	/** This function should be used to store any intermediate results during
	 *  various iterations after the burn-in period.
	 *  @param iteration Provides the iteration number beginning after the burn-in period.
	 */
	virtual void storeSEMRun(Index iteration,
			Index iterationMax) {
		paramStat_.sampleParam(iteration,
				iterationMax);
		if (iteration == iterationMax) {
			paramStat_.normalizeParam(paramStr_); // enforce that estimated proportions sum to 1, but only if paramStr is of the form "nModality: x"
			paramStat_.setExpectationParam(); // set parameter to mode / expectation
		}
	}

	virtual void storeGibbsRun(Index sample,
			Index iteration,
			Index iterationMax) {
		dataStat_.sampleVals(sample,
				iteration,
				iterationMax);
		if (iteration == iterationMax) {
			dataStat_.imputeData(sample); // impute the missing values using empirical mean or mode, depending of the model
		}
	}

	/**
	 * This function must be defined to return the completed likelihood, using the current values for
	 * unknown values
	 * @return the completed log-likelihood
	 */
	virtual Real lnCompletedProbability(Index i, Index k) const {
		return likelihood_.lnCompletedProbability(i, k);
	}

	/**
	 * This function must be defined to return the observed likelihood
	 * @return the observed log-likelihood
	 */
	virtual Real lnObservedProbability(Index i, Index k) const {
		return likelihood_.lnObservedProbability(i, k);
	}

	/** This function must return the number of free parameters.
	 *  @return Number of free parameters
	 */
	virtual Index nbFreeParameter() const {
		return model_.computeNbFreeParameters();
	}

	/** This function can be used to write summary of parameters on to the output stream.
	 * @param out Stream where you want to write the summary of parameters.
	 */
	virtual void writeParameters() const {
		model_.writeParameters();
	}

	virtual void exportDataParam() const {
		p_dataExtractor_->exportVals(
				indexMixture_,
				model_.hasModalities(),
				idName_,
				augData_,
				dataStat_.getDataStatStorage()); // export the obtained data using the DataExtractor
		p_paramExtractor_->exportParam(
				indexMixture_,
				idName(),
				"NumericalParam",
				paramStat_.getStatStorage(),
				paramStat_.getLogStorage(),
				model_.paramNames(),
				confidenceLevel_,
				paramStr_);
	}

	void initData(Index i) {
		augData_.removeMissing(i);
	}

	std::string initParam() {
		model_.initParam();
		return "";
	};

	std::string checkSampleCondition(const Vector<std::set<Index> >& classInd) const {
		std::string warnLog = model_.checkSampleCondition(classInd);
		if (0 < warnLog.size()) {
			return "checkSampleCondition, error in variable " + idName_ + eol + warnLog;
		}
		return "";
	}

	/**
	 * The observed probability is fast to compute for simple models. There is no need to
	 * precompute an empirical distribution. */
	virtual void computeObservedProba() {};

	virtual void initializeMarkovChain() {};

	std::vector<bool> parametersInInterior() {return model_.parametersInInterior();}

private:
protected:
	/** Number of classes */
	int nbClass_;

	/** Current parameters of the model_ */
	Vector<Real> param_;

	/** The simple model to bridge with the composer */
	Model model_;

	/** The augmented data set */
	AugmentedData<typename Model::Data> augData_;

	/** Parameters transmitted by the user */
	std::string paramStr_;

	/** number of samples in the data set*/
	Index nbInd_;

	/** confidence level used in computation of parameters and missing values statistics */
	Real confidenceLevel_;

	/** Sampler to generate values */
	typename Model::Sampler sampler_;

	/** Statistics computer for missing data */
	typename Model::DataStat dataStat_;

	/** Statistics storage for parameters */
	ConfIntParamStat<Vector<Real> > paramStat_;

	/** Computation of the observed likelihood */
	typename Model::Likelihood likelihood_;

	/** Pointer to the data handler */
	const DataHandler* p_handler_;

	/** Pointer to the data extractor */
	DataExtractor* p_dataExtractor_;

	/** Pointer to the param setter */
	const ParamSetter* p_paramSetter_;

	/** Pointer to the parameters extractor */
	ParamExtractor* p_paramExtractor_;
};

}

#endif /* MIXT_MIXTUREBRIDGE_H */
