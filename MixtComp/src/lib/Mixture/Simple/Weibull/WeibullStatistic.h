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
 *  Created on: 18th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLSTATISTIC_H
#define WEIBULLSTATISTIC_H

#include <boost/random/mersenne_twister.hpp>

#include <Statistic/UniformStatistic.h>
#include <LinAlg/LinAlg.h>

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
