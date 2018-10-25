/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef LIB_STRATEGY_MIXT_GIBBSSTRATEGY_H
#define LIB_STRATEGY_MIXT_GIBBSSTRATEGY_H

#include <Composer/mixt_MixtureComposer.h>
#include <Various/mixt_Timer.h>
#include <Various/mixt_Various.h>
#include "mixt_GibbsStrategy.h"

namespace mixt {
/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
template<typename Graph>
class GibbsStrategy {
public:
	/** default constructor.
	 *  @param p_composer the model to estimate
	 **/
	GibbsStrategy(MixtureComposer& composer, const Graph& algo, Index startGroup) :
			composer_(composer), algo_(algo), startGroup_(startGroup) {
	}

	/** run the strategy */
	std::string run() {
		std::string warnLog;
		Index nSemTry = algo_.template get_payload<Index>({}, "nSemTry");
		Index nbGibbsBurnInIter = algo_.template get_payload<Index>({}, "nbGibbsBurnInIter");
		Index nbGibbsIter = algo_.template get_payload<Index>({}, "nbGibbsIter");

		try {
			for (Index n = 0; n < nSemTry; ++n) {

				composer_.initData(); // TODO: check that this is really necessary in Gibbs
				warnLog = composer_.initializeLatent();
				if (0 < warnLog.size())
					continue;

				runGibbs(burnIn_, nbGibbsBurnInIter, 0 + startGroup_, // group
				1 + startGroup_); // groupMax

				runGibbs(run_, nbGibbsIter, 1 + startGroup_, // group
				1 + startGroup_); // groupMax

				return "";
			}
		} catch (const std::string& str) {
			warnLog = str;
		}

		return warnLog;
	}

	/**
	 * run the algorithm, only kept during the transition, as an archive
	 * @return string describing the problem in case of soft degeneracy */
	void runGibbs(RunType runType, Index nIter, Index group, Index groupMax) {
#ifdef MC_VERBOSE
		Timer myTimer;
		if (runType == burnIn_) {
			myTimer.setName("Gibbs: burn-in");
		} else {
			myTimer.setName("Gibbs: run");
		}
#endif

		for (Index iterGibbs = 0; iterGibbs < nIter; ++iterGibbs) {
#ifdef MC_VERBOSE
			myTimer.iteration(iterGibbs, nIter - 1);
#endif

#ifdef MC_PROGRESS
			writeProgress(group, groupMax, iterGibbs, nIter - 1);
#endif

			composer_.eStepCompleted();
			composer_.sampleZ();
			composer_.sampleUnobservedAndLatent();

			if (runType == run_) {
				composer_.storeGibbsRun(iterGibbs, nIter - 1);
			}
		}

#ifdef MC_VERBOSE
		myTimer.finish();
#endif
	}

protected:
	/** reference on the main model */
	MixtureComposer& composer_;

	const Graph& algo_;

	/** To help differentiate between a starting Gibbs, and a Gibbs that follows a SEM */
	Index startGroup_;
};

}  // namespace mixt

#endif /* MIXT_GIBBSSTRATEGY_H */
