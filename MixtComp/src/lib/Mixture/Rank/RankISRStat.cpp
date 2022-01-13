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
 *  Created on: September 3, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "RankISRStat.h"

#include <algorithm>
#include <iostream>
#include <map>

namespace mixt {

RankISRStat::RankISRStat(RankVal& mu, Real confidenceLevel) :
		mu_(mu), confidenceLevel_(confidenceLevel), initialNIter_(0) {
}

void RankISRStat::sample(int iteration) {
	logStorageMu_(iteration) = mu_;
}

void RankISRStat::sampleValue(int iteration, int iterationMax) {
	if (iteration == 0) {
		initialNIter_ = iterationMax + 1;
		logStorageMu_.resize(initialNIter_);

		sample(0); // first sampling, on each parameter
	} else if (iteration == iterationMax) {
		sample(iterationMax); // last sampling

		if (iterationMax + 1 != initialNIter_) {
			logStorageMu_ = logStorageMu_.block(0, 0, iterationMax + 1, 1).eval(); // if partition is stable, iterationMax has been reduced in comparison to initialNIter_
		}

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

//		logStorageMu_.resize(0); // clear memory
	} else {
		sample(iteration);
	}
}

void RankISRStat::setExpectationParam() {
	mu_ = statStorageMu_.front().first;
}

void RankISRStat::setParamStorage() {
	statStorageMu_.push_back(std::pair<RankVal, Real>(mu_, 1.));
}

} // namespace mixt
