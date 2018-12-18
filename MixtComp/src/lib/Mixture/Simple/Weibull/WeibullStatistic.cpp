/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 18th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "WeibullStatistic.h"
#include "../../../Statistic/mixt_RNG.h"
#include <boost/math/distributions/weibull.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/weibull_distribution.hpp>

namespace mixt {

WeibullStatistic::WeibullStatistic() :
		rng_(seed(this)) {
}

Real WeibullStatistic::quantile(Real k, Real lambda, Real p) const {
	boost::math::weibull weib(k, lambda);
	Real x = boost::math::quantile(weib, p);
	return x;
}

Real WeibullStatistic::quantileIB(Real k, Real lambda, Real infBound, Real p) const {
	Real u = (1.0 - p) * cdf(k, lambda, infBound) + p;
	return quantile(k, lambda, u);
}


Real WeibullStatistic::quantileI(Real k, Real lambda, Real infBound, Real supBound, Real p) const {
	Real u = p * cdf(k, lambda, supBound) + (1.0 - p) * cdf(k, lambda, infBound);
	return quantile(k, lambda, u);
}

Real WeibullStatistic::sample(Real k, Real lambda) {
	boost::random::weibull_distribution<> w(k, lambda);
	boost::variate_generator<boost::mt19937&,
			boost::random::weibull_distribution<> > generator(rng_, w);
	return generator();
}

Real WeibullStatistic::sampleIB(Real k, Real lambda, Real infBound) {
	Real u = uniform_.sample(0., 1.);
	return quantileIB(k, lambda, infBound, u);
}

Real WeibullStatistic::sampleI(Real k, Real lambda, Real infBound, Real supBound) {
	Real u = uniform_.sample(0., 1.);
	return quantileI(k, lambda, infBound, supBound, u);
}

Real WeibullStatistic::pdf(Real k, Real lambda, Real x) const {
	boost::math::weibull weib(k, lambda);
	Real proba = boost::math::pdf(weib, x);
	return proba;
}

Real WeibullStatistic::lpdf(Real k, Real lambda, Real x) const {
	Real logLambda = std::log(lambda);
	return std::log(k) - logLambda + (k - 1.0) * (std::log(x) - logLambda)
			- std::pow(x / lambda, k);
}

Real WeibullStatistic::cdf(Real k, Real lambda, Real x) const {
	return 1.0 - std::exp(-std::pow(x / lambda, k));
}

Real WeibullStatistic::cdfIB(Real k, Real lambda, Real a, Real x) const {
	Real cdfa = cdf(k, lambda, a);
	return (cdf(k, lambda, x) - cdfa) / (1.0 - cdfa);
}

Real WeibullStatistic::lcdf(Real k, Real lambda, Real x) const {
	return std::log(cdf(k, lambda, x));
}

}
