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
 *  Created on: December 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <ctime>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/math/distributions/normal.hpp>
#include <Statistic/GaussianStatistic.h>
#include <Statistic/RNG.h>
#include <Various/Constants.h>

namespace mixt {

GaussianStatistic::GaussianStatistic() :
		rng_(seed(this)) {
}

Real GaussianStatistic::cdf(Real x, Real mean, Real sd) const {
	boost::math::normal norm(mean, sd);
	Real proba = boost::math::cdf(norm, x);
	return proba;
}

/** log pdf evaluated at individual x */
Real GaussianStatistic::lpdf(Real x, Real mean, Real sd) const {
	return -log(sd) - logsqrt2pi - 0.5 * pow((x - mean) / sd, 2);
}

Real GaussianStatistic::pdf(Real x, Real mean, Real sd) const {
	boost::math::normal norm(mean, sd);
	Real proba = boost::math::pdf(norm, x);
	return proba;
}

Real GaussianStatistic::sample(Real mean, Real sd) {
	boost::normal_distribution<> norm(mean, sd);
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > generator(
			rng_, norm);
	Real x = generator();
	return x;
}

Real GaussianStatistic::sampleI(Real mean, Real sd, Real infBound,
		Real supBound) {
	Real z;

	Real lower = (infBound - mean) / sd;
	Real upper = (supBound - mean) / sd;

	if (upper < 0) {
		z = -sideSampler(-upper, -lower);
	} else if (lower < 0 && 0 < upper) {
		if (upper - lower < sqrt(2. * pi)) { // tight spread, need to use specific algorithm
			z = lrbSampler(lower, upper);
		} else { // spread between bounds not important enough, traditional rejection sampling should work quickly
			do {
				z = sample(0., 1.);
			} while (z < lower || upper < z);
		}
	} else if (0 < lower) {
		z = sideSampler(lower, upper);
	} else {
		z = std::numeric_limits<Real>::signaling_NaN();
		; // the case is impossible
	}

	return z * sd + mean;
}

Real GaussianStatistic::sampleIB(Real mean, Real sd, Real infBound) {
	Real z;
	Real lower = (infBound - mean) / sd;
	z = lbSampler(lower);
	return z * sd + mean;
}

Real GaussianStatistic::sampleSB(Real mean, Real sd, Real supBound) {
	Real z;
	Real upper = (supBound - mean) / sd;
	z = -lbSampler(-upper);
	return z * sd + mean;
}

Real GaussianStatistic::lbSampler(Real lower) {
	Real alpha = (lower + sqrt(pow(lower, 2) + 4.)) / 2.;
	Real z, u, rho;
	if (lower < 0) { // less than 0.5 proba of rejection using regular sampling
		do {
			z = sample(0., 1.);
		} while (z < lower);
	} else {
		do {
			z = exponential_.sample(alpha) + lower;
			rho = exp(-pow((z - alpha), 2) / 2.);
			u = uniform_.sample(0., 1.);
		} while (u > rho);
	}
	return z;
}

Real GaussianStatistic::lrbSampler(Real lower, Real upper) {
	Real z, u;
	Real rho = 0.;
	do {
		z = uniform_.sample(lower, upper);

		if (lower < 0. && 0. < upper) {
			rho = exp(-pow(z, 2));
		} else if (upper < 0.) { // lrbSampler is called from sideSampler for that case to occur
			rho = exp((pow(upper, 2) - pow(z, 2)) / 2);
		} else if (0. < lower) {
			rho = exp((pow(lower, 2) - pow(z, 2)) / 2);
		} else {
			throw("NormalStatistic::lrbSampler, unexpected case, please report to the maintainer.");
		}

		u = uniform_.sample(0., 1.);
	} while (u > rho);

	return z;
}

Real GaussianStatistic::sideSampler(Real lower, Real upper) {
	Real alpha = (lower + sqrt(pow(lower, 2) + 4.)) / 2.;
	Real z;

	if (log(alpha) + alpha * lower / 2.
			> pow(lower, 2) / 2. - log(upper - lower)) { // test carried out in log, to avoid overflow
		do {
			z = lbSampler(lower); // if upper is far from lower, use lbSampler with rejection
		} while (upper < z);
	} else { // use true bounded sampler
		z = lrbSampler(lower, upper);
	}

	return z;
}

void GaussianStatistic::expSigmaTruncated(Real mu, Real sigma, Real a, Real b,
		Real& truncMu, Real& truncSigma) {
	Real alpha = (a - mu) / sigma;
	Real beta = (b - mu) / sigma;

	Real phiAlpha = pdf(alpha, 0., 1.);
	Real phiBeta = pdf(beta, 0., 1.);

	Real PhiAlpha = cdf(alpha, 0., 1.);
	Real PhiBeta = cdf(beta, 0., 1.);

	Real z = PhiBeta - PhiAlpha;

	truncMu = mu + (phiAlpha - phiBeta) / z * sigma;
	truncSigma = sqrt(
			pow(sigma, 2)
					* (1. + (alpha * phiAlpha - beta * phiBeta) / z
							- pow((phiAlpha - phiBeta) / z, 2)));
}

} // namespace mixt
