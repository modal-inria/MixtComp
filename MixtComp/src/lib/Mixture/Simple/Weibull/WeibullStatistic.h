/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 18th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLSTATISTIC_H
#define WEIBULLSTATISTIC_H

#include "../../../LinAlg/mixt_LinAlg.h"
#include "../../../Statistic/mixt_UniformStatistic.h"
#include <boost/random/mersenne_twister.hpp>

namespace mixt {

class WeibullStatistic {
  public:
    WeibullStatistic();

    /** cdf evaluated at individual x */
    Real cdf(Real x, Real k, Real lambda) const;

    /** cdf evaluated at individual x, when the distribution is truncated in the interval [infBound; +inf]. */
    Real cdfIB(Real x, Real k, Real lambda, Real infBound) const;

    Real lcdf(Real x, Real k, Real lambda) const;

    Real pdf(Real x, Real k, Real lambda) const;

    Real lpdf(Real x, Real k, Real lambda) const;

    /** Sample a value from a Weibull Law with parameters lambda and k. */
    Real sample(Real k, Real lambda);

    /**
     * Sample a value from a Weibull Law with parameters lambda and k
     * conditioned on being in the interval [infBound; +inf]
     * */
    Real sampleIB(Real k, Real lambda, Real infBound);

    /**
     * Sample a value from a Weibull Law with parameters lambda and k
     * conditioned on being in the interval [infBound; supBound]
    * */
    Real sampleI(Real k, Real lambda, Real infBound, Real supBound);

    /** quantile corresponding pour proba p */
    Real quantile(Real k, Real lambda, Real p) const;
    Real quantileIB(Real k, Real lambda, Real infBound, Real p) const;
    Real quantileI(Real k, Real lambda, Real infBound, Real supBound, Real p) const;


  private:
    /** Random number generator */
    boost::random::mt19937 rng_;

    UniformStatistic uniform_;
};

}

#endif
