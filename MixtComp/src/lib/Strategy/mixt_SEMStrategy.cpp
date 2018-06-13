/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sept 3, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#include "mixt_SEMStrategy.h"
#include "../Various/mixt_Timer.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Various.h"
#include "../Various/mixt_Constants.h"

namespace mixt {

/** default constructor */
SemStrategy::SemStrategy(
		MixtureComposer* p_composer,
		const StrategyParam& param)
: p_composer_(p_composer),
  param_(param) {}

std::string SemStrategy::run() {
	std::string warnLog;

	for (Index n = 0; n < param_.nSemTry_; ++n) {
		std::cout << "SemStrategy::run, attempt n: " << n << std::endl;
		warnLog.clear(); // only the last warn log can be sent

		p_composer_->initData(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
		warnLog = p_composer_->checkNbIndPerClass();
		if (0 < warnLog.size()) {
			std::cout << "Not enough individuals per class." << std::endl;
			continue;
		}
//		p_composer_->printClassInd();

		p_composer_->initParam(); // initialize iterative estimators
		warnLog = p_composer_->initParamSubPartition(param_.nInitPerClass_); // initialize parameters for each model, usually singling out an observation as the center of each class
		if (0 < warnLog.size()) {
			std::cout << "initParam failed." << std::endl;
			continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further
		}

		std::cout << "SemStrategy::run, initParam succeeded." << std::endl;

		p_composer_->writeParameters(); // for debugging purposes

		warnLog = p_composer_->initializeLatent(); // use observed probability to initialize classes
		if (0 < warnLog.size()) {
			continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further
		}

		std::cout << "SemStrategy::run initializeLatent succeeded." << std::endl;
		std::cout << "SEM initialization complete. SEM run can start." << std::endl;

		warnLog = runSEM(
				burnIn_,
				param_.nbBurnInIter_,
				0, // group
				3); // groupMax
		if (0 < warnLog.size()) continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further

		warnLog = runSEM(
				run_,
				param_.nbIter_,
				1, // group
				3); // groupMax
		if (0 < warnLog.size()) continue;

		return ""; // at the moment, stop the loop at the first completed run, this will evolve later
	}

	return warnLog;
}

std::string SemStrategy::runSEM(
		RunType runType,
		Index nIter,
		int group,
		int groupMax) {
	std::string warnLog;

	Timer myTimer;

	if (runType == burnIn_) {
	  std::cout << "Burnin Iterations begin" << std::endl;
		myTimer.setName("SEM: burn-in");
	}
	else if (runType == run_) {
    std::cout << "Iterations begins" << std::endl;
		myTimer.setName("SEM: run");
	}

	for (Index iter = 0; iter < nIter; ++iter) {
	  std::cout << "Iteration " << iter+1 << std::endl;
		myTimer.iteration(iter, nIter - 1);
		writeProgress(
				group,
				groupMax,
				iter,
				nIter - 1);

		p_composer_->eStepCompleted();

		p_composer_->sampleZ(); // no checkSampleCondition performed, to increase speed of sampling
		p_composer_->sampleUnobservedAndLatent();

		std::string warnLog = p_composer_->checkSampleCondition(); // since we are not in initialization, no need for log
		if (0 < warnLog.size()) {
			std::cout << "runSEM, checkSampleCondition failed." << std::endl;
			return warnLog;
		}

		warnLog += p_composer_->mStep(); // biased or unbiased does not matter, as there has been a check on sampling conditions previously
		if (0 < warnLog.size()) {
			std::cout << "runSEM, mStep failed." << std::endl;
			return warnLog;
		}
//		p_composer_->writeParameters();

		p_composer_->storeSEMRun(
				iter,
				nIter - 1,
				runType);
	}

	return "";
}

} // namespace mixt
