/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 2, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/


#ifndef MIXT_IMIXTURE_H
#define MIXT_IMIXTURE_H

#include <iostream>
#include <set>
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Enum.h"

namespace mixt {

class IMixture {
  public:
    /**
     * Constructor with identification character
     *
     * @param idName Identification string of the mixture provided by the framework
     * */
    IMixture(Index indexMixture, std::string const& idName) :
      indexMixture_(indexMixture),
      idName_(idName) {};

    /** Virtual destructor. Needed as IMixture will only be used as a base class. */
    virtual ~IMixture() {};

    /**
     * Return the Id of the mixture
     * @return Id of the mixture
     * */
    std::string const& idName() const {
      return idName_;
    }

    /**
     * Simulation of latent variables and partially observed data without verifying
     * checkSampleCondition to speed-up the process.
     *
     * @param ind index of the individual which data must be sampled
     * */
    virtual void sampleUnobservedAndLatent(Index ind, Index k) = 0;

    /**
     * Check if conditions on data are verified. For example, for a categorical model one must check that each modality
     * is present at least one time in each class. This is invoked to avoid degeneracy.
     * @return 0 if condition not verified and 1 if condition verified
     * */
    virtual std::string checkSampleCondition(const Vector<std::set<Index>>& classInd) const = 0;

    /**
     * Maximum-Likelihood estimation of the mixture parameters
     *
     * @return empty string if mStep successful, or a detailed description of the eventual error
     * */
    virtual void mStep(const Vector<std::set<Index>>& classInd) = 0;

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
    virtual void storeGibbsRun(
        Index i,
        Index iteration,
        Index iterationMax) = 0;

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
     * Should be modified to take a vector of indices, one per class, indicating which individual to use
     * */
    virtual std::string initParam(const Vector<std::set<Index>>& classInd, const Vector<Index>& initObs) = 0;

    /**
     * Compute and cache the empirical observed distribution for the models that need it.
     * This is currently the case for the BOS and ISR algorithms.
     * */
    virtual void computeObservedProba() = 0;

    /** Initialize the Markov Chain for models that contains one for their latent variables. */
    virtual void initializeMarkovChain() = 0;

    /** Check if the real parameters are within the interior of their domain. */
    virtual std::vector<bool> parametersInInterior() = 0;
  protected:
    /** Index of the mixture, useful to write the results at the correct place in th output. */
    Index indexMixture_;

    /** Id name of the mixture */
    std::string idName_;
};

} // namespace mixt

#endif /* MIXT_IMIXTURE_H */
