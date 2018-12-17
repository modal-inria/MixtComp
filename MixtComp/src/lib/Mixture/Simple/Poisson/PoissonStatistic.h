/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_POISSONSTATISTIC_H
#define MIXT_POISSONSTATISTIC_H

#include <boost/random/mersenne_twister.hpp>
#include "../../../LinAlg/mixt_LinAlg.h"
#include "../../../Statistic/mixt_UniformStatistic.h"

namespace mixt {

class PoissonStatistic {
public:
	PoissonStatistic();

	/** pdf evaluated at individual x */
	Real pdf(int x, Real lambda) const;

	Real lpdf(int x, Real lambda) const;

    /** cdf evaluated at individual x */
	Real cdf(int x, Real lambda) const;

	/** Sample a value from a Poisson Law with parameter lambda */
	int sample(Real lambda);

    /**
     * Sample a value from a Poisson Law with parameters lambda
     * conditioned on being in the interval [infBound; +inf]
     * */
    Real sampleIB(Real lambda, Real infBound);

    /**
     * Sample a value from a Poisson Law with parameters lambda
     * conditioned on being in the interval [infBound; supBound]
    * */
    Real sampleI(Real lambda, Real infBound, Real supBound);

    /** Sample a value from a Poisson Law with parameters lambda
     * conditioned on being in the interval [-inf; supBound]
     * */
    Real sampleSB(Real lambda, Real supBound);

    /** quantile corresponding pour proba p */
    Real quantile(Real lambda, Real p) const;
    Real quantileIB(Real lambda, Real infBound, Real p) const;
    Real quantileSB(Real lambda, Real supBound, Real p) const;
    Real quantileI(Real lambda, Real infBound, Real supBound, Real p) const;

    /** Sample a value from a Weibull Law with parameters lambda and k. */
    Real sample(Real k, Real lambda);


	/**
	 * Sample a non-zero value
	 * */
	int nonZeroSample(Real lambda);
private:
	/** Random number generator */
	boost::mt19937 rng_;

	/** Uniform sampler used for nonZeroSample */
	UniformStatistic uniform_;
};

} // namespace mixt

#endif // MIXT_POISSONSTATISTIC_H
