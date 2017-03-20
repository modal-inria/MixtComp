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
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Enum.h"

namespace mixt {

enum EstimatorType {
  biased_,
  unBiased_
};

enum SamplerInitialization {
  performInitialization_,
  doNotPerformInitialization_,
};

class IMixture {
  public:
    /**
     * Constructor with identification character
     *
     * @param idName Identification string of the mixture provided by the framework
     */
    IMixture(Index indexMixture, std::string const& idName) :
      indexMixture_(indexMixture),
      idName_(idName) {};

    /** Virtual destructor. Needed as IMixture will only be used as a base class. */
    virtual ~IMixture() {};

    /**
     * Return the Id of the mixture
     * @return Id of the mixture*/
    std::string const& idName() const {
      return idName_;
    }

    /**
     * Simulation of latent variables and partially observed data with values that
     * verify checkSampleCondition.
     *
     * @param ind index of the individual which data must be sampled
     */
    virtual void samplingStepCheck(Index ind) = 0;

    /**
     * Simulation of latent variables and partially observed data without verifying
     * checkSampleCondition to speed-up the process.
     *
     * @param ind index of the individual which data must be sampled
     */
    virtual void samplingStepNoCheck(SamplerInitialization init, Index ind) = 0;

    /**
     * Check if conditions on data are verified. For example, for a categorical model one must check that each modality
     * is present at least one time in each class. This is invoked to avoid degeneracy.
     * @return 0 if condition not verified and 1 if condition verified
     * */
    virtual Index checkSampleCondition(std::string* warnLog = NULL) const = 0;

    /**
     * Maximum-Likelihood estimation of the mixture parameters
     *
     * @return empty string if mStep successful, or a detailed description of the eventual error
     */
    virtual void mStep(EstimatorType bias) = 0;

    /**
     * Storage of mixture parameters during SEM run phase
     *
     * @param iteration SEM run iteration number
     * @param iterationMax maximum number of iterations
     * period.
     */
    virtual void storeSEMRun(Index iteration,
                             Index iterationMax) = 0;

    /**
     * Storage of mixture parameters during SEM run phase
     *
     * @param i individual
     * @param iteration Gibbs iteration
     * @param iterationMax maximum number of iterations
     */
    virtual void storeGibbsRun(Index i,
                               Index iteration,
                               Index iterationMax) = 0;

    /**
     * Computation of completed likelihood
     *
     * @param i individual
     * @param k class
     * @return value of the completed likelihood in log scale
     */
    virtual Real lnCompletedProbability(Index i, Index k) = 0;

    /**
     * Computation of observed likelihood
     *
     * @param i individual
     * @param k class
     * @return value of the observed likelihood in log scale
     */
    virtual Real lnObservedProbability(Index ind, Index k) = 0;

    /**
     * Computation of the number of free parameters.
     *
     *  @return Number of free parameters
     */
    virtual Index nbFreeParameter() const = 0;

    /**
     * This function can be used to write summary of parameters on to the output stream.
     *
     *  @param[out] stream to write the summary of parameters to
     */
    virtual void writeParameters() const = 0;

    /**
     * Initialization of the data and parameters
     *
     * @param mode run mode, for example learning or prediction
     * @return empty string if no errors, otherwise errors description
     */
    virtual std::string setDataParam(RunMode mode) = 0;

    /**
     * Export of parameters and data
     */
    virtual void exportDataParam() const = 0;

    /**
     * Initialization of missing / latent data priori to any parameter knowledge. In prediction parameters are known at
     * initialization, and a samplingStep is used.
     */
    virtual void initData(Index ind) = 0;

    /**
     * Initialization of parameters. Useful for some parameters that use a Markov Chain which needs to be initialized.
     */
    virtual void initParam() = 0;

    virtual bool observedCorrection() = 0;
  protected:
    /** Index of the mixture, useful to write the results at the correct place in th output. */
    Index indexMixture_;

    /** Id name of the mixture */
    std::string idName_;
};

} // namespace mixt

#endif /* MIXT_IMIXTURE_H */
