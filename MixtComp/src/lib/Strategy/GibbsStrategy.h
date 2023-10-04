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
 *  Created on: Nov 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef LIB_STRATEGY_GIBBSSTRATEGY_H
#define LIB_STRATEGY_GIBBSSTRATEGY_H

#include <Composer/MixtureComposer.h>
#include <Strategy/GibbsStrategy.h>
#include <Various/Timer.h>
#include <Various/Various.h>

namespace mixt {
/**
 *  A GibbsStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 *
 *  The goal is to impute missing data and latent variables with parameters estimated from a SEM. *
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
	std::string run(std::pair<Real, Real>& time) {
		std::string warnLog;
		Index nSemTry = algo_.template get_payload<Index>( { }, "nSemTry");
		Index nbGibbsBurnInIter = algo_.template get_payload<Index>( { }, "nbGibbsBurnInIter");
		Index nbGibbsIter = algo_.template get_payload<Index>( { }, "nbGibbsIter");

		try {
			for (Index n = 0; n < nSemTry; ++n) {
				warnLog = composer_.initializeLatent();
				if (0 < warnLog.size())
					continue;

				runGibbs(burnIn_, nbGibbsBurnInIter, 0 + startGroup_, 1 + startGroup_, time.first);

				runGibbs(run_, nbGibbsIter, 1 + startGroup_, 1 + startGroup_, time.second);

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
	void runGibbs(RunType runType, Index nIter, Index group, Index groupMax, Real& time) {
		Timer myTimer;
		if (runType == burnIn_) {
			myTimer.setName("Gibbs: burn-in");
		} else {
			myTimer.setName("Gibbs: run");
		}

		for (Index iterGibbs = 0; iterGibbs < nIter; ++iterGibbs) {
			myTimer.iteration(iterGibbs, nIter - 1);

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

		time = myTimer.finish();
	}

protected:
	/** reference on the main model */
	MixtureComposer& composer_;

	const Graph& algo_;

	/** To help differentiate between a starting Gibbs, and a Gibbs that follows a SEM */
	Index startGroup_;
};

}  // namespace mixt

#endif /* GIBBSSTRATEGY_H */
