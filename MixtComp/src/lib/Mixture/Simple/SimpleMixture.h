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
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SIMPLEMIXTURE_H
#define SIMPLEMIXTURE_H

#include <Data/AugmentedData.h>
#include <IO/IO.h>
#include <IO/IOFunctions.h>
#include <IO/NamedAlgebra.h>
#include <Mixture/IMixture.h>
#include <Param/ConfIntParamStat.h>
#include <Various/Constants.h>

namespace mixt {

/**
 * SimpleMixture is the new interface to add simple models. It is simpler than SimpleMixtureBridge which used class traits for no apparent benefit.
 */
template<typename Graph, typename Model>
class SimpleMixture: public IMixture {
public:
	/** constructor.
	 *  @param idName id name of the mixture
	 *  @param nbCluster number of cluster
	 **/
	SimpleMixture(const Graph& data, const Graph& param, Graph& out, std::string const& idName, Index nbClass, Index nInd, Real confidenceLevel, const std::string& paramStr) :
			IMixture(idName, Model::name, nbClass, nInd), dataG_(data), paramG_(param), outG_(out), param_(), model_(idName, nbClass, param_), augData_(), paramStr_(paramStr), confidenceLevel_(
					confidenceLevel), sampler_(augData_, param_, nbClass), dataStat_(augData_, confidenceLevel), paramStat_(param_, confidenceLevel), likelihood_(param_, augData_, nbClass) {
	}

	/**
	 * This function will be defined to set the data into your data containers.
	 * To facilitate data handling, framework provide templated functions,
	 * that can be called directly to get the data.
	 */
	std::string setDataParam(RunMode mode) {
		std::string warnLog;

		std::vector<std::string> dataVecStr;
		dataG_.get_payload( { }, idName_, dataVecStr);
		warnLog += StringToAugmentedData(idName_, dataVecStr, augData_, (model_.hasModalities()) ? (-minModality) : (0));

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
			NamedMatrix<Real> stat;
			paramG_.get_payload( { idName_ }, "stat", stat);
			Index nrow = stat.mat_.rows();

			paramG_.get_payload( { idName_ }, "paramStr", paramStr_);

			param_.resize(nrow);
			for (Index i = 0; i < nrow; ++i) {
				param_(i) = stat.mat_(i, 0); // only the mode / expectation is used, quantile information is discarded
			}

			paramStat_.setParamStorage(); // paramStatStorage_ is set now, using dimensions of param_, and will not be modified during predict run by the paramStat_ object for some mixtures, there will be errors if the range of the data in prediction is different from the range of the data in learning in the case of modalities, this can not be performed earlier, as the max val is computed at model_.setModalities(nbParam)
		}

		warnLog += model_.setData(paramStr_, augData_, mode); // checks on data bounds are made here, if paramStr_.size() = 0, it might be completed here, for example using the number of modalities found in the data

		dataStat_.setNbIndividual(nInd_);
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

	void exportDataParam() const {
		Graph g;
		convertDataStat(dataStat_.getDataStatStorage(), g);
		outG_.addSubGraph( { "variable", "data" }, idName_, g);

		NamedVector<typename Model::Data::Type> dataOut;
		dataOut.vec_ = augData_.data_; // note that no row names are provided
		if (model_.hasModalities()) {
			dataOut.vec_ += minModality;
		}
		outG_.add_payload( { "variable", "data", idName_ }, "completed", dataOut);

		Index ncol = paramStat_.getStatStorage().cols();
		std::vector<std::string> colNames(ncol);

		quantileNames(ncol, confidenceLevel_, colNames);

		NamedMatrix<Real> paramStatOut; // all parameters are real at the moment,
		paramStatOut.mat_ = paramStat_.getStatStorage();
		paramStatOut.rowNames_ = model_.paramNames();
		paramStatOut.colNames_ = colNames;

		NamedMatrix<Real> paramLogOut;
		paramLogOut.mat_ = paramStat_.getLogStorage();
		paramLogOut.rowNames_ = model_.paramNames();

		outG_.add_payload( { "variable", "param", idName_ }, "stat", paramStatOut);
		outG_.add_payload( { "variable", "param", idName_ }, "log", paramLogOut);
		outG_.add_payload( { "variable", "param", idName_ }, "paramStr", paramStr_);
	}




	/*
	 * convert missing data statistics for multinomial model
	 *
	 * dataStat is the output of getDataStatStorage() from MultinomialDataStat
	 */
	void convertDataStat(Vector<std::vector<std::pair<int, Real> > > const& dataStat, Graph & g) const {
		for (int i = 0; i < augData_.data_.rows(); ++i) {
			if (augData_.misData_(i).first != present_) {
				NamedMatrix<Real>  dataStatOut(dataStat(i).size(), 2, true);
				dataStatOut.colNames_ = {"modality", "probability"};
				for (std::vector<std::pair<int, Real> >::const_iterator itVec = dataStat(i).begin(); itVec != dataStat(i).end(); ++itVec) {
					dataStatOut.mat_(itVec - dataStat(i).begin(), 0) = itVec->first + minModality; // current modality
					dataStatOut.mat_(itVec - dataStat(i).begin(), 1) = itVec->second; // probability of the modality
				}
				g.add_payload({"stat"}, std::to_string(i), dataStatOut);
			}
		}
	}

	/*
	 * convert missing data statistics for other simple models
	 *
	 * dataStat is the output of getDataStatStorage() from ConfIntDataStat
	 */
	template<typename Type>
	void convertDataStat(Vector<RowVector<Type> > const& dataStat, Graph & g) const {
		int nbMiss = augData_.nbSample_ - augData_.misCount_(present_);
		NamedMatrix<Type> dataStatOut(nbMiss, 4, true);

		Real alpha = (1. - confidenceLevel_) / 2.;
		dataStatOut.colNames_[0] = "index";
		dataStatOut.colNames_[1] = "median";
		dataStatOut.colNames_[2] = std::string("q ") + std::to_string((alpha * 100.)) + "%";
		dataStatOut.colNames_[3] = std::string("q ") + std::to_string(((1. - alpha) * 100.)) + "%";


		int j = 0;
		for (int i = 0; i < augData_.data_.rows(); ++i) {
			if (augData_.misData_(i).first != present_) {
				dataStatOut.mat_(j, 0) = i;
				dataStatOut.mat_(j, 1) = dataStat(i)(0); // median
				dataStatOut.mat_(j, 2) = dataStat(i)(1); // left bound
				dataStatOut.mat_(j, 3) = dataStat(i)(2); // right bound
				++j;
			}
		}

		g.add_payload( { }, "stat", dataStatOut);

	}

	void initData(Index i) {
		augData_.removeMissingSample(i);
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
	const Graph& dataG_;
	const Graph& paramG_;
	Graph& outG_;

	/** Current parameters of the model_ */
	Vector<Real> param_;

	/** The simple model to bridge with the composer */
	Model model_;

	/** The augmented data set */
	AugmentedData<typename Model::Data> augData_;

	/** Parameters transmitted by the user */
	std::string paramStr_;

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

#endif /* SIMPLEMIXTURE_H */
