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
