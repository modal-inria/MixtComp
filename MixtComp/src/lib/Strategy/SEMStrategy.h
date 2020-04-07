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
 *  Created on: Sept 3, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file SEMStrategy.h
 *  @brief In this file we define the strategy to use in order to estimate a
 *  mixture model.
 **/

#ifndef LIB_STRATEGY_SEMSTRATEGY_H
#define LIB_STRATEGY_SEMSTRATEGY_H

#include <Composer/MixtureComposer.h>
#include <Various/Timer.h>
#include <Various/Various.h>

namespace mixt {

/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
template<typename Graph>
class SemStrategy {

public:
	/** default constructor.
	 *  @param p_composer the model to estimate
	 **/
	SemStrategy(MixtureComposer& composer, const Graph& algo) :
			composer_(composer), algo_(algo) {
	}

	/** run the strategy */
	std::string run(std::pair<Real, Real>& time) {
		std::string warnLog;

		Index nSemTry = algo_.template get_payload<Index>( { }, "nSemTry");
		Index nInitPerClass = algo_.template get_payload<Index>( { }, "nInitPerClass");
		Index nbBurnInIter = algo_.template get_payload<Index>( { }, "nbBurnInIter");
		Index nbIter = algo_.template get_payload<Index>( { }, "nbIter");
		Index nStableCriterion = algo_.template get_payload<Index>( { }, "nStableCriterion");
		Real ratioStableCriterion = algo_.template get_payload<Real>( { }, "ratioStableCriterion");

		try {
			for (Index n = 0; n < nSemTry; ++n) {
#ifdef MC_VERBOSE
				std::cout << "SemStrategy::run, attempt n: " << n << std::endl;
#endif
				warnLog.clear(); // only the last warn log can be sent

				composer_.initData(); // complete missing values without using models (uniform samplings in most cases), as no mStep has been performed yet
				warnLog = composer_.checkNbIndPerClass(); // useless because a new check is performed in initParam TODO: remove if really useless
				if (0 < warnLog.size()) {
#ifdef MC_VERBOSE
					std::cout << "Not enough individuals per class." << std::endl;
#endif
					continue;
				}
				//		p_composer_->printClassInd();

				composer_.initParam(); // initialize iterative estimators
				warnLog = composer_.initParamSubPartition(nInitPerClass); // initialize parameters for each model, usually singling out an observation as the center of each class
				if (0 < warnLog.size()) {
#ifdef MC_VERBOSE
					std::cout << "initParam failed." << std::endl;
#endif
					continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further
				}

#ifdef MC_VERBOSE
				std::cout << "SemStrategy::run, initParam succeeded." << std::endl;
				composer_.writeParameters(); // for debugging purposes
#endif

				warnLog = composer_.initializeLatent(); // use observed probability to initialize classes
				if (0 < warnLog.size()) {
					continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further
				}

#ifdef MC_VERBOSE
				std::cout << "SemStrategy::run initializeLatent succeeded." << std::endl;
				std::cout << "SEM initialization complete. SEM run can start." << std::endl;
#endif

				warnLog = runSEM(burnIn_, nbBurnInIter, 0, 3, ratioStableCriterion, nStableCriterion, time.first); // group, groupMax
				if (0 < warnLog.size())
					continue; // a non empty warnLog signals a problem in the SEM run, hence there is no need to push the execution further

				warnLog = runSEM(run_, nbIter, 1, 3, ratioStableCriterion, nStableCriterion, time.second); // group, groupMax
				if (0 < warnLog.size())
					continue;

				return ""; // at the moment, stop the loop at the first completed run, this will evolve later
			}
		} catch (const std::string& str) {
			warnLog = str;
		}

		return warnLog;
	}

	/**
	 * run the algorithm
	 * @return string describing the problem in case of soft degeneracy */
	std::string runSEM(RunType runType, Index nIter, int group, int groupMax, Real ratioStableCriterion, Index nStableCriterion, Real& time) {
		std::string warnLog;

		Timer myTimer;
		if (runType == burnIn_) {
			myTimer.setName("SEM: burn-in");
		} else if (runType == run_) {
			myTimer.setName("SEM: run");
		}

		composer_.stabilityReset();

		for (Index iter = 0; iter < nIter; ++iter) {
			myTimer.iteration(iter, nIter - 1);

#ifdef MC_PROGRESS
			writeProgress(group, groupMax, iter, nIter - 1);
#endif

			composer_.eStepCompleted();
			composer_.sampleZ(); // no checkSampleCondition performed, to increase speed of sampling
			composer_.sampleUnobservedAndLatent();

			std::string warnLog = composer_.checkSampleCondition(); // since we are not in initialization, no need for log
			if (0 < warnLog.size()) {
#ifdef MC_VERBOSE
				std::cout << "runSEM, checkSampleCondition failed." << std::endl;
#endif
				return warnLog;
			}

			warnLog += composer_.mStep(); // biased or unbiased does not matter, as there has been a check on sampling conditions previously
			if (0 < warnLog.size()) {
#ifdef MC_VERBOSE
				std::cout << "runSEM, mStep failed." << std::endl;
#endif
				return warnLog;
			}
			//		p_composer_->writeParameters();

			if (composer_.isPartitionStable(ratioStableCriterion, nStableCriterion)) {
#ifdef MC_VERBOSE
				std::cout << "runSEM, partition has been stable for " << nStableCriterion << " iterations." << std::endl;
#endif
				composer_.storeSEMRun(iter, iter, runType); // note that the last iteration is set as the current one, and not as nIter-1
				break; // no need for further iterations
			} else {
				composer_.storeSEMRun(iter, nIter - 1, runType);
			}
		}

		time = myTimer.finish();

		return "";
	}

private:
	/** reference on the main model */
	MixtureComposer& composer_;

	const Graph& algo_;
};

}  // namespace mixt

#endif /* SEMSTRATEGY_H_ */
