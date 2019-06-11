/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: April 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Statistic/RNG.h>
#include "MultinomialStatistic.h"

namespace mixt {

MultinomialStatistic::MultinomialStatistic() :
		rng_(seed(this)), uni_(0., 1.), g_(rng_), generator_(rng_, uni_) {
}
;

int MultinomialStatistic::sampleInt(int low, int high) {
	boost::random::uniform_int_distribution<> uni(low, high);
	boost::variate_generator<boost::random::mt19937&,
			boost::random::uniform_int_distribution<> > generator(rng_, uni);
	int x = generator();
	return x;
}

} // namespace mixt
