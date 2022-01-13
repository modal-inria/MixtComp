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
 *  Created on: July 2, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef LIB_MIXTURE_IMIXTURE_H
#define LIB_MIXTURE_IMIXTURE_H

#include <LinAlg/LinAlg.h>
#include <iostream>
#include <set>
#include <Various/Enum.h>

namespace mixt {

class IMixture {
public:
	/**
	 * Constructor with identification character
	 *
	 * @param idName Identification string of the mixture provided by the framework
	 * */
	IMixture(const std::string& idName, const std::string& modelType, Index nClass, Index nInd) :
			idName_(idName), modelType_(modelType), nClass_(nClass), nInd_(nInd) {
	}
	;

	/** Virtual destructor. Needed as IMixture will only be used as a base class. */
	virtual ~IMixture() {
	}
	;

	/**
	 * Return the Id of the mixture
	 * @return Id of the mixture
	 * */
	const std::string& idName() const {
		return idName_;
	}

	const std::string& modelType() const {
		return modelType_;
	}

	/**
	 * Simulation of latent variables and partially observed data without verifying
	 * checkSampleCondition to speed-up the process.
	 *
	 * @param ind index of the individual which data must be sampled
	 * */
	virtual void sampleUnobservedAndLatent(Index ind, Index k) = 0;


	/**
	 * Check if conditions on data are verified. For example, for a multinomial model one must check that each modality
	 * is present at least one time in each class. This is invoked to avoid degeneracy.
	 * @return 0 if condition not verified and 1 if condition verified
	 * */
	virtual std::string checkSampleCondition(const Vector<std::set<Index>>& classInd) const = 0;

	/**
	 * Maximum-Likelihood estimation of the mixture parameters
	 *
	 * @return empty string if mStep successful, or a detailed description of the eventual error
	 * */
	virtual std::string mStep(const Vector<std::set<Index>>& classInd) = 0;

	/**
	 * Storage of mixture parameters during SEM run phase
	 *
	 * @param iteration SEM run iteration number
	 * @param iterationMax maximum number of iterations
	 * period.
	 * */
	virtual void storeSEMRun(Index iteration, Index iterationMax) = 0;

	/**
	 * Storage of mixture parameters during SEM run phase
	 *
	 * @param i individual
	 * @param iteration Gibbs iteration
	 * @param iterationMax maximum number of iterations
	 * */
	virtual void storeGibbsRun(Index i, Index iteration, Index iterationMax) = 0;

	/**
	 * Computation of completed likelihood
	 *
	 * @param i individual
	 * @param k class
	 * @return value of the completed likelihood in log scale
	 * */
	virtual Real lnCompletedProbability(Index i, Index k) const = 0;

	/**
	 * Computation of observed likelihood
	 *
	 * @param i individual
	 * @param k class
	 * @return value of the observed likelihood in log scale
	 * */
	virtual Real lnObservedProbability(Index ind, Index k) const = 0;

	/**
	 * Computation of the number of free parameters.
	 *
	 * @return Number of free parameters
	 * */
	virtual Index nbFreeParameter() const = 0;

	/** This function can be used to print a summary of parameters. */
	virtual void writeParameters() const = 0;

	/**
	 * Initialization of the data and parameters
	 *
	 * @param mode run mode, for example learning or prediction
	 * @return empty string if no errors, otherwise errors description
	 * */
	virtual std::string setDataParam(RunMode mode) = 0;

	/**
	 * Export of parameters and data
	 * */
	virtual void exportDataParam() const = 0;

	/**
	 * Initialization of missing / latent data priori to any parameter knowledge. In prediction parameters are known at
	 * initialization, and a samplingStep is used.
	 */
	virtual void initData(Index ind) = 0;


	/**
	 * Initialization of parameters.
	 * Useful for some parameters that use a Markov Chain which needs to be initialized.
	 * Or for models that use an iterator solver that must be initialized before the first iteration.
	 * Should be modified to take a vector of indices, one per class, indicating which individual to use
	 */
	virtual void initParam() = 0;

	/**
	 * Compute and cache the empirical observed distribution for the models that need it.
	 * This is currently the case for the BOS and ISR algorithms.
	 * */
	virtual void computeObservedProba() = 0;

	/** Initialize the Markov Chain for models that contains one for their latent variables. */
	virtual void initializeMarkovChain(Index i, Index k) = 0;

	/**
	 * Some model compute the observed probability using a sampler. It is possible that some values are never sampled,
	 * despite the fact that their probability is not 0. When this case occur, the variable is ignored in the computation
	 * of the observed probability.
	 */
	virtual bool sampleApproximationOfObservedProba() = 0;
protected:
	/** Id name of the mixture */
	std::string idName_;

	/** Type of the model */
	std::string modelType_;

	Index nClass_;
	Index nInd_;
};

} // namespace mixt

#endif /* IMIXTURE_H */
