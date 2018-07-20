/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */
/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include <ctime>
#include "NegativeBinomialStatistic.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/negative_binomial_distribution.hpp>
#include <boost/math/distributions/negative_binomial.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include "../LinAlg/mixt_Math.h"
#include "../Various/mixt_Constants.h"
#include "mixt_RNG.h"

namespace mixt {

NegativeBinomialStatistic::NegativeBinomialStatistic() :
		rng_(seed(this)) {
}

Real NegativeBinomialStatistic::pdf(int x, Real n, Real p) const {
	boost::math::negative_binomial nBinom(n, p);
	Real proba = boost::math::pdf(nBinom, x);
	return proba;
}

Real NegativeBinomialStatistic::lpdf(int x, Real n, Real p) const {
	if (p < 1.0) {
		return boost::math::lgamma(x + n) - boost::math::lgamma(n) - logFac(x) + Real(n) * std::log(p) + Real(x) * std::log(1.-p);
	} else {
		if(realEqual(p, 1.))
		{
			if(x == 0)
				return 0.;
			else
				return minInf;
		}

		return minInf;
	}
}

int NegativeBinomialStatistic::sample(Real n, Real p) {
		boost::random::negative_binomial_distribution<> nBinom(n, p);
		boost::variate_generator<boost::mt19937&, boost::random::negative_binomial_distribution<> > generator(rng_, nBinom);
		int x = generator();

		return x;
}



} // namespace mixt
