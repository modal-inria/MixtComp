/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SIMPLEmodel_H
#define SIMPLEmodel_H

#include <Mixture/mixt_IMixture.h>
#include <Data/mixt_AugmentedData.h>
#include <IO/IOFunctions.h>
#include <IO/mixt_IO.h>
#include <Various/mixt_Constants.h>
#include <Param/mixt_ConfIntParamStat.h>

namespace mixt {

/**
 * SimpleMixture is the new interface to add simple models. It is simpler than SimpleMixtureBridge which used class traits for no apparent benefit.
 */
template<typename Model>
class SimpleMixture: public IMixture {
public:
	/** constructor.
	 *  @param idName id name of the mixture
	 *  @param nbCluster number of cluster
	 **/
	SimpleMixture(std::string const& idName, Index nbClass, Index nInd, Real confidenceLevel, const std::string& paramStr) :
			IMixture(idName, Model::name, nbClass, nInd), nbClass_(nbClass), param_(), model_(idName, nbClass, param_), augData_(), paramStr_(paramStr), nbInd_(nInd), confidenceLevel_(confidenceLevel), sampler_(
					augData_, param_, nbClass), dataStat_(augData_, confidenceLevel), paramStat_(param_, confidenceLevel), likelihood_(param_, augData_, nbClass) {
	}

	/**
	 * This function will be defined to set the data into your data containers.
	 * To facilitate data handling, framework provide templated functions,
	 * that can be called directly to get the data.
	 */
	std::string setDataParam(RunMode mode, const std::vector<std::string>& data, const SGraph& param) {
		std::string warnLog;
//		warnLog += p_handler_->getData(idName(), augData_, nbInd_, paramStr_, (model_.hasModalities()) ? (-minModality) : (0)); // minModality offset for categorical models
		warnLog += StringToAugmentedData(idName_, data, augData_, (model_.hasModalities()) ? (-minModality) : (0));

		if (warnLog.size() > 0) {
			return warnLog;
		}
		augData_.computeRange();
		std::string tempLog = augData_.checkMissingType(model_.acceptedType()); // check if the missing data provided are compatible with the model

		if (tempLog.size() > 0) { // check on the missing values description
			std::stringstream sstm;
			sstm << "Variable " << idName() << " has a problem with the descriptions of missing values." << std::endl << tempLog;
			warnLog += sstm.str();
		}

		if (mode == prediction_) {
//			p_paramSetter_->getParam(idName_, "NumericalParam", param_, paramStr_); // parameters are set using results from previous run, note that in the prediction case, the eventual paramStr_ obtained from p_handler_->getData is overwritten by the one provided by the parameter structure from the learning

			const NamedMatrix<Real>& stat = param.get_child("NumericalParam").get_payload<NamedMatrix<Real>>("stat");
			Index nrow = stat.mat_.rows();
			paramStr_ = param.get_child("NumericalParam").get_payload<std::string>("paramStr");

			param_.resize(nrow);
			for (Index i = 0; i < nrow; ++i) {
				param_(i) = stat.mat_(i, 0); // only the mode / expectation is used, quantile information is discarded
			}

			paramStat_.setParamStorage(); // paramStatStorage_ is set now, using dimensions of param_, and will not be modified during predict run by the paramStat_ object for some mixtures, there will be errors if the range of the data in prediction is different from the range of the data in learning in the case of modalities, this can not be performed earlier, as the max val is computed at model_.setModalities(nbParam)
		}

		warnLog += model_.setData(paramStr_, augData_, mode); // checks on data bounds are made here, if paramStr_.size() = 0, it might be completed here, for example using the number of modalities found in the data

		dataStat_.setNbIndividual(nbInd_);

		return warnLog;
	}

	void sampleUnobservedAndLatent(Index ind, Index k) {
		sampler_.samplingStepNoCheck(ind, k);
	}

	/**
	 * Estimate parameters by maximum likelihood
	 */
	std::string mStep(const Vector<std::set<Index> >& classInd) {
		return model_.mStep(classInd);
	}

	/** This function should be used to store any intermediate results during
	 *  various iterations after the burn-in period.
	 *  @param iteration Provides the iteration number beginning after the burn-in period.
	 */
	void storeSEMRun(Index iteration, Index iterationMax) {
		paramStat_.sampleParam(iteration, iterationMax);
		if (iteration == iterationMax) {
			paramStat_.normalizeParam(paramStr_); // enforce that estimated proportions sum to 1, but only if paramStr is of the form "nModality: x"
			paramStat_.setExpectationParam(); // set parameter to mode / expectation
		}
	}

	void storeGibbsRun(Index sample, Index iteration, Index iterationMax) {
		dataStat_.sampleVals(sample, iteration, iterationMax);
		if (iteration == iterationMax) {
			dataStat_.imputeData(sample); // impute the missing values using empirical mean or mode, depending of the model
		}
	}

	/**
	 * This function must be defined to return the completed likelihood, using the current values for
	 * unknown values
	 * @return the completed log-likelihood
	 */
	Real lnCompletedProbability(Index i, Index k) const {
		return likelihood_.lnCompletedProbability(i, k);
	}

	/**
	 * This function must be defined to return the observed likelihood
	 * @return the observed log-likelihood
	 */
	Real lnObservedProbability(Index i, Index k) const {
		return likelihood_.lnObservedProbability(i, k);
	}

	/** This function must return the number of free parameters.
	 *  @return Number of free parameters
	 */
	Index nbFreeParameter() const {
		return model_.computeNbFreeParameters();
	}

	/** This function can be used to write summary of parameters on to the output stream.
	 * @param out Stream where you want to write the summary of parameters.
	 */
	void writeParameters() const {
		model_.writeParameters();
	}

	void exportDataParam(SGraph& data, SGraph& param) const {
		NamedVector<typename Model::Data::Type> dataOut;
		dataOut.vec_ = augData_.data_; // not that no row names are provided
		if (model_.hasModalities()) {
			dataOut.vec_ += minModality;
		}
		data.add_payload("completed", dataOut);

		Index ncol = paramStat_.getStatStorage().cols();
		std::vector<std::string> colNames(ncol);
		Real alpha = (1. - confidenceLevel_) / 2.;

		if (ncol == 1) { // predict
			colNames[0] = "value";
		} else { // learn
			colNames[0] = "median";
			colNames[1] = std::string("q ") + std::to_string((alpha * 100.)) + "%";
			colNames[2] = std::string("q ") + std::to_string(((1. - alpha) * 100.)) + "%";
		}

		SGraph numericalParam;
		NamedMatrix<Real> paramOut; // all parameters are real at the moment,
		paramOut.mat_ = paramStat_.getStatStorage();
		paramOut.rowNames_ = model_.paramNames();
		paramOut.colNames_ = colNames;
		numericalParam.add_payload("stat", paramOut);
		numericalParam.add_payload("paramStr", paramStr_);

		param.add_child("NumericalParam", numericalParam);
	}

	void initData(Index i) {
		augData_.removeMissing(i);
	}

	void initParam() {
		model_.initParam();
	}
	;

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
};

}

#endif /* MIXT_MIXTUREBRIDGE_H */
