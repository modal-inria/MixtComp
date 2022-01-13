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
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef POISSONSTATISTIC_H
#define POISSONSTATISTIC_H

#include <boost/random/mersenne_twister.hpp>
#include "../../../LinAlg/LinAlg.h"
#include "../../../Statistic/UniformStatistic.h"

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
	int sampleIB(Real lambda, int infBound);

    /**
     * Sample a value from a Poisson Law with parameters lambda
     * conditioned on being in the interval [infBound; supBound]
    * */
    int sampleI(Real lambda, int infBound, int supBound);


    /** quantile corresponding to proba p */
    int quantile(Real lambda, Real p) const;
    int quantileIB(Real lambda, int infBound, Real p) const;
    int quantileI(Real lambda, int infBound, int supBound, Real p) const;


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

#endif // POISSONSTATISTIC_H
