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
 * UniformIntStatistic.cpp
 *
 *  Created on: 17 déc. 2018
 *      Author: Quentin Grimonprez, Vincent Kubicki
 */

#include "UniformIntStatistic.h"

#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <Statistic/RNG.h>

namespace mixt
{

UniformIntStatistic::UniformIntStatistic() :
    rng_(seed(this))
{}

Real UniformIntStatistic::cdf(int x, int min, int max) const
{
	Real proba = std::max(std::min(max - min + 1., x - min + 1.), 0.)/(max - min + 1.);

    return proba;
}

Real UniformIntStatistic::pdf(int x, int min, int max) const
{
	if((x < min) | (x > max))
		return 0.;

	return 1./(max - min + 1.);
}

int UniformIntStatistic::sample(int min, int max)
{
	boost::random::uniform_int_distribution<> uni(min, max);

	boost::variate_generator<boost::random::mt19937&,
	                         boost::random::uniform_int_distribution<> > generator(rng_, uni);
	Real x = generator();
  	return x;
}

} // namespace mixt

