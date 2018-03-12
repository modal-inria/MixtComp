/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sept 3, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff, serge.iovleff@stkpp.org,
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file mixt_MixtureStrategy.h
 *  @brief In this file we define the strategy to use in order to estimate a
 *  mixture model.
 **/

#ifndef MIXT_MIXTURESTRATEGY_H
#define MIXT_MIXTURESTRATEGY_H

#include "../Composer/mixt_MixtureComposer.h"
#include "../Strategy/mixt_StrategyParam.h"

namespace mixt {

/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
class SemStrategy {

public:
	/** default constructor.
	 *  @param p_composer the model to estimate
	 **/
	SemStrategy(MixtureComposer* p_composer, const StrategyParam& param);

	/** run the strategy */
	std::string run();

	/**
	 * run the algorithm, only kept during the transition, as an archive
	 * @return string describing the problem in case of soft degeneracy */
	std::string runSEM(RunType runType, Index nIter, int group, int groupMax);

private:
	/** reference on the main model */
	MixtureComposer* p_composer_;

	const StrategyParam& param_;
};

}  // namespace mixt

#endif /* MIXT_MIXTURESTRATEGY_H_ */
