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
		p_composer_->initData();
		p_composer_->initializeLatent();

		p_composer_->gibbsSampling(doNotSampleData_, param_.nbGibbsBurnInIter_,
				0 + startGroup_, // group
				1 + startGroup_); // groupMax

		p_composer_->gibbsSampling(sampleData_, param_.nbGibbsIter_,
				1 + startGroup_, // group
				1 + startGroup_); // groupMax
	} catch (const std::string& str) {
		warnLog = str;
	}

	return warnLog;
}

} // namespace mixt
