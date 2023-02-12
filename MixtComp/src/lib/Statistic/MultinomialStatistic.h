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
 *  Created on: January 20, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MULTINOMIALSTATISTIC_H
#define MULTINOMIALSTATISTIC_H

#include <iostream>
#include <ctime>
#include <random>
#include <Various/Constants.h>
#include "../LinAlg/LinAlg.h"
#include "../IO/IO.h"
#include "Statistic/RNG.h"

namespace mixt {

class MultinomialStatistic {

private:
/** Random number generator */
std::mt19937 rng_;

std::uniform_real_distribution<Real> uni_;

// std::random_number_generator<std::mt19937> g_;

// boost::variate_generator<boost::random::mt19937&,
// 		boost::random::uniform_real_distribution<> > generator_;

public:
	MultinomialStatistic()
	: rng_(seed(this)),
	  uni_(0.0, 1.0) {}

	/** Sample a value from a binomial law with  */
	int sampleBinomial(Real proportion) {
		if (uni_(rng_) < proportion) {
			return 1;
		} else {
			return 0;
		}
	}

	/** Sample a value from a multinomial law with coefficient of modalities provided */
	template<typename T>
	int sample(const T& proportion) {
		Real x = uni_(rng_);

		Real cumProb = 0.; // cumulative probability
		int index = 0;

		for (int i = 0, ie = proportion.size(); i < ie; ++i) {
			cumProb += proportion.coeff(i);
			if (x < cumProb) {
				return index;
			}
			++index;
		}
		return -1;
	}

	template<typename T>
	void shuffle(T& data) {
		std::shuffle(data.begin(), data.end(), rng_);
	}

	template<typename T>
	Real entropy(const T& data) {
		Real h = 0.;
		for (typename T::const_iterator it = data.begin(); it != data.end();
				++it) {
			h += *it * std::log(*it);
		}
		return h;
	}

	/**
	 * Uniform int sample
	 * @param low lower bound of the support
	 * @param high higher bound of the support
	 * @return integer uniformly sampled from [low:high]
	 */
	int sampleInt(int low, int high);
};

} // namespace mixt

#endif // MULTINOMIALSTATISTIC_H
