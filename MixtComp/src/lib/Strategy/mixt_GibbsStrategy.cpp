/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#include "mixt_GibbsStrategy.h"
#include "../Various/mixt_Timer.h"
#include "../Various/mixt_Various.h"

namespace mixt {

/** default constructor */
GibbsStrategy::GibbsStrategy(MixtureComposer* p_composer,
		const StrategyParam& param, Index startGroup) :
		p_composer_(p_composer), param_(param), startGroup_(startGroup) {
}

std::string GibbsStrategy::run() {
	std::string warnLog;

	try {
		for (Index n = 0; n < param_.nSemTry_; ++n) {

			p_composer_->initData(); // TODO: check that this is really necessary in Gibbs
			warnLog = p_composer_->initializeLatent();
			if (0 < warnLog.size())
				continue;

			runGibbs(burnIn_, param_.nbGibbsBurnInIter_, 0 + startGroup_, // group
			1 + startGroup_); // groupMax

			runGibbs(run_, param_.nbGibbsIter_, 1 + startGroup_, // group
			1 + startGroup_); // groupMax

			return "";
		}
	} catch (const std::string& str) {
		warnLog = str;
	}

	return warnLog;
}

void GibbsStrategy::runGibbs(RunType runType, Index nIter, Index group,
		Index groupMax) {
	Timer myTimer;
	if (runType == burnIn_) {
		myTimer.setName("Gibbs: burn-in");
	} else {
		myTimer.setName("Gibbs: run");
	}

	for (Index iterGibbs = 0; iterGibbs < nIter; ++iterGibbs) {
		myTimer.iteration(iterGibbs, nIter - 1);
		writeProgress(group, groupMax, iterGibbs, nIter - 1);

		p_composer_->eStepCompleted();
		p_composer_->sampleZ();
		p_composer_->sampleUnobservedAndLatent();

		if (runType == run_) {
			p_composer_->storeGibbsRun(iterGibbs, nIter - 1);
		}
	}
}

} // namespace mixt
