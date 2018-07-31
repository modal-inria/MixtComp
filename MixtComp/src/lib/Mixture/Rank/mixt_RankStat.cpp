/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 3, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_RankStat.h"

#include <algorithm>
#include <iostream>
#include <map>

namespace mixt {

RankStat::RankStat(RankVal& mu, Real confidenceLevel) :
		mu_(mu), confidenceLevel_(confidenceLevel) {
}

void RankStat::sample(int iteration) {
	logStorageMu_(iteration) = mu_;
}

void RankStat::sampleValue(int iteration, int iterationMax) {
	if (iteration == 0) {
		logStorageMu_.resize(iterationMax + 1);

		sample(0); // first sampling, on each parameter
	} else if (iteration == iterationMax) {
		sample(iterationMax); // last sampling

		std::map<RankVal, int> stat; // sparse counting of the occurrences of mu
		for (int i = 0; i < iterationMax + 1; ++i) {
			stat[logStorageMu_(i)] += 1;
		}
		int nbMu = stat.size();

		Vector<RankVal> mu(nbMu); // transform sparse storage into contiguous storage which will be sortable
		Vector<int> nb(nbMu); // number of occurrences of current value of mu

		int m = 0;
		for (std::map<RankVal, int>::const_iterator it = stat.begin(), // loop on values of mu
		itEnd = stat.end(); it != itEnd; ++it, ++m) {
			mu(m) = it->first;
			nb(m) = it->second;
		}

		Vector<int> index;
		nb.sortIndex(index);

		Real cumSum = 0.;
		for (int muPos = nbMu - 1; muPos > -1; --muPos) { // loop from the most to the less frequent values of mu
			int m = index(muPos); // index of current value of mu

			Real proba = Real(nb(m)) / Real(iterationMax + 1);

			statStorageMu_.push_back(std::pair<RankVal, Real>(mu(m), proba));

			cumSum += proba;

			if (cumSum > confidenceLevel_)
				break;
		}

		logStorageMu_.resize(0); // clear memory
	} else {
		sample(iteration);
	}
}

void RankStat::setExpectationParam() {
	mu_ = statStorageMu_.front().first;
}

void RankStat::setParamStorage() {
	statStorageMu_.push_back(std::pair<RankVal, Real>(mu_, 1.));
}

} // namespace mixt
