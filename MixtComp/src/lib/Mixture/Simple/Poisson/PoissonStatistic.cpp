/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/
/*
 *  Project:    MixtComp
 *  Created on: December 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <ctime>
#include <boost/random/variate_generator.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/math/distributions/poisson.hpp>
#include <LinAlg/Math.h>
#include <Statistic/RNG.h>
#include <Various/Constants.h>
#include "PoissonStatistic.h"

namespace mixt {

PoissonStatistic::PoissonStatistic() :
		rng_(seed(this)) {
}

Real PoissonStatistic::pdf(int x, Real lambda) const {
	boost::math::poisson pois(lambda);
	Real proba = boost::math::pdf(pois, x);
	return proba;
}

Real PoissonStatistic::lpdf(int x, Real lambda) const {
	if (0.0 < lambda) {
		return Real(x) * std::log(lambda) - lambda - logFac(x);
	} else {
		if (x == 0)
			return 0.0;
		else
			return minInf;
	}
}

Real PoissonStatistic::cdf(int x, Real lambda) const {
	boost::math::poisson pois(lambda);
	Real proba = boost::math::cdf(pois, x);
	return proba;
}


int PoissonStatistic::sample(Real lambda) {
	if (0.0 < lambda) {
		boost::poisson_distribution<> pois(lambda);
		boost::variate_generator<boost::mt19937&, boost::poisson_distribution<> > generator(
				rng_, pois);
		int x = generator();
		return x;
	} else {
		return 0;
	}
}


int PoissonStatistic::nonZeroSample(Real lambda) {
	Real u = uniform_.sample(0., 1.);
	Real cdf = 0.;
	int currMod = 0; // modality 0 is ignored
	Real coeff = 1. / (1. - pdf(0, lambda)); // coeff to take the condition that x != 0 into account
	while (cdf < u) {
		++currMod;
		cdf += pdf(currMod, lambda) * coeff;
	}
	return currMod;
}


int PoissonStatistic::sampleIB(Real lambda, int infBound) {
	Real u = uniform_.sample(0., 1.);
	return quantileIB(lambda, infBound, u);
}


int PoissonStatistic::sampleI(Real lambda, int infBound, int supBound) {
	Real u = uniform_.sample(0., 1.);
	return quantileI(lambda, infBound, supBound, u);
}

int PoissonStatistic::quantile(Real lambda, Real p) const {
	/*
	 * use of round up quantile poisson in order to use the inverse transform sample methods for discrete random variable
	 * http://www3.eng.cam.ac.uk/~ss248/G12-M01/Week1/ITM.pdf
	 * https://www.boost.org/doc/libs/1_69_0/libs/math/doc/html/math_toolkit/pol_tutorial/understand_dis_quant.html
	 *
	 */
	boost::math::poisson_distribution<
	            double,
				boost::math::policies::policy<boost::math::policies::discrete_quantile<boost::math::policies::integer_round_up> > > pois(lambda);
	int q = boost::math::quantile(pois, p);
	return q;
}

int PoissonStatistic::quantileIB(Real lambda, int infBound, Real p) const {
	Real u = (1.0 - p) * cdf(infBound, lambda) + p;
	return quantile(lambda, u);
}

int PoissonStatistic::quantileI(Real lambda, int infBound, int supBound, Real p) const {
	Real u = p * cdf(supBound, lambda) + (1.0 - p) * cdf(infBound, lambda);
	return quantile(lambda, u);
}



} // namespace mixt
