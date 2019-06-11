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

