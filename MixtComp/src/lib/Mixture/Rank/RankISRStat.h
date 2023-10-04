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

#ifndef RANKISRSTAT_H
#define RANKISRSTAT_H

#include <list>

#include <Mixture/Rank/RankVal.h>
#include <LinAlg/LinAlg.h>

namespace mixt {

/** Compute statistics for a single class. Use a vector of those for a mixture. */
class RankISRStat {
public:
	RankISRStat(RankVal& mu, Real confidenceLevel);

	/** There used to be a tuple here, dedicated to Quentin Grimonprez, proud forefather of its usage among us, mere Modal Team member.
	 * But it was deemed unnecessary leading to its demise and ultimate obliteration with extreme prejudice.
	 * Of the tuple. Not of Quentin. */
	const std::list<std::pair<RankVal, Real> >& statStorageMu() const {
		return statStorageMu_;
	}

	const Vector<RankVal>& logStorageMu() const {
		return logStorageMu_;
	}

	void sampleValue(int iteration, int iterationMax);

	/** Set mu and pi with the estimated value, at the end of the SEM. The estimated value of mu is the mode, and the
	 * estimated value of pi is the mode among the values of pi sampled for the estimated mu. */
	void setExpectationParam();

	void setParamStorage();

	int nbPos() const {
		return mu_.nbPos();
	}
private:
	void sample(int iteration);

	RankVal& mu_;

	Real confidenceLevel_;

	/** Each element in the list contains a triplet <mu, frequency of mu, Vector (median, left quantile, right quantile) of pi>.
	 * The confidenceLevel parameter act as a cutoff both for mu and pi. */
	std::list<std::pair<RankVal, Real> > statStorageMu_;

	Index initialNIter_;

	/** Storage for iterations results, 1 value per iteration */
	Vector<RankVal> logStorageMu_;
};

} // namespace mixt

#endif // RANKISRSTAT_H
