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
 *  Created on: December 15, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef GAUSSIANSTATISTIC_H
#define GAUSSIANSTATISTIC_H

#include <boost/random.hpp>
#include <Statistic/ExponentialStatistic.h>
#include <Statistic/UniformStatistic.h>
#include <LinAlg/LinAlg.h>

namespace mixt {

class GaussianStatistic {
  public:
	GaussianStatistic();

    /** cdf evaluated at individual x */
    Real cdf(Real x,
             Real mean,
             Real sd) const;

    /** log pdf evaluated at individual x */
    Real lpdf(Real x,
              Real mean,
              Real sd) const;

    /** pdf evaluated at individual x */
    Real pdf(Real x,
             Real mean,
             Real sd) const;

    /** Sample a value from a Normal Law with parameters mean and sd */
    Real sample(Real mean,
                Real sd);

    /** Sample a value from a Normal Law with parameters mean and sd,
     * conditionned on being in the interval [infBound; supBound] */
    Real sampleI(Real mean,
                 Real sd,
                 Real leftBound,
                 Real rightBound);

    /** Sample a value from a Normal Law with parameters mean and sd,
     * conditionned on being in the interval [infBound; +inf] */
    Real sampleIB(Real mean,
                  Real sd,
                  Real infBound);

    /** Sample a value from a Normal Law with parameters mean and sd,
     * conditionned on being in the interval [-inf; supBound] */
    Real sampleSB(Real mean,
                  Real sd,
                  Real supBound);

    /** Compute expectation of a truncated normal law. Used in sampler debug for example. */
    void expSigmaTruncated(Real mu,
                           Real sigma,
                           Real a,
                           Real b,
                           Real& truncMean,
                           Real& truncVar);

    /** helper function to help sample on intervals */
    Real lbSampler(Real lower);

    /** helper function to help sample on intervals */
    Real lrbSampler(Real lower, Real upper);

    /** when lower bound > 0, selects wether to used lbSampler or lrbSampler
     * based on the spread between bounds*/
    Real sideSampler(Real lower, Real upper);

  private:
    /** Random number generator */
    boost::mt19937 rng_;

    UniformStatistic uniform_;

    ExponentialStatistic exponential_;
};

} // namespace mixt

#endif // GAUSSIANSTATISTIC_H
