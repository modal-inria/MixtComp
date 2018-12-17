/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */
/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include <boost/random/variate_generator.hpp>
#include <boost/random/negative_binomial_distribution.hpp>
#include <boost/math/distributions/negative_binomial.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include "NegativeBinomialStatistic.h"
#include "../../../LinAlg/mixt_Math.h"
#include "../../../Various/mixt_Constants.h"
#include "../../../Statistic/mixt_RNG.h"

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

Real NegativeBinomialStatistic::cdf(int x, Real n, Real p) const {
	boost::math::negative_binomial nBinom(n, p);
	Real proba = boost::math::cdf(nBinom, x);
	return proba;
}

int NegativeBinomialStatistic::sample(Real n, Real p) {
	boost::random::negative_binomial_distribution<> nBinom(n, p);
	boost::variate_generator<boost::mt19937&, boost::random::negative_binomial_distribution<> > generator(rng_, nBinom);
	int x = generator();

	return x;
}


int NegativeBinomialStatistic::sampleIB(Real n, Real p, int infBound)
{
	Real u = uniform_.sample(0., 1.);
	return quantileIB(n, p, infBound, u);
}


int NegativeBinomialStatistic::sampleI(Real n, Real p, int infBound, int supBound)
{
	Real u = uniform_.sample(0., 1.);
	return quantileI(n, p, infBound, supBound, u);
}


int NegativeBinomialStatistic::quantile(Real n, Real p, Real alpha) const
{
	/*
	 * use of round up quantile negative binomial in order to use the inverse transform sample methods for discrete random variable
	 * http://www3.eng.cam.ac.uk/~ss248/G12-M01/Week1/ITM.pdf
	 * https://www.boost.org/doc/libs/1_69_0/libs/math/doc/html/math_toolkit/pol_tutorial/understand_dis_quant.html
	 *
	 */
	boost::math::negative_binomial_distribution<
	            double,
				boost::math::policies::policy<boost::math::policies::discrete_quantile<boost::math::policies::integer_round_up> > > nBinom(n, p);

	int q = boost::math::quantile(nBinom, alpha);
	return q;
}

int NegativeBinomialStatistic::quantileIB(Real n, Real p, int infBound, Real alpha) const
{
	Real u = (1.0 - alpha) * cdf(infBound, n, p) + alpha;
	return quantile(n, p, u);
}

int NegativeBinomialStatistic::quantileI(Real n, Real p, int infBound, int supBound, Real alpha) const
{
	Real u = alpha * cdf(supBound, n, p) + (1.0 - alpha) * cdf(infBound, n, p);
	return quantile(n, p, u);
}

} // namespace mixt
