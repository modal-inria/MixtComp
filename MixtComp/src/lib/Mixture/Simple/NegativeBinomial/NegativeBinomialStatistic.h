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
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#ifndef NEGATIVEBINOMIALSTATISTIC_H
#define NEGATIVEBINOMIALSTATISTIC_H

#include <LinAlg/LinAlg.h>
#include <Statistic/UniformStatistic.h>

#include <boost/random/mersenne_twister.hpp>

namespace mixt {

// f(x;n,p) = Γ(x+n)/(Γ(n) x!) p^n (1-p)^x
// with x a non negative integer
// 0 < p < 1
// n : number of failure (positive integer)

//\f$\displaystyle P(i) = {k+i-1\choose i}p^k(1-p)^i\f$.
class NegativeBinomialStatistic {
public:
	NegativeBinomialStatistic();

	/** pdf evaluated at individual x */
	Real pdf(int x, Real n, Real p) const;

	Real lpdf(int x, Real n, Real p) const;

	/** cdf evaluated at individual x */
	Real cdf(int x, Real n, Real p) const;

	/** Sample a value from a Negative Binomial Law with parameter n and p */
	int sample(Real n, Real p);

    /**
     * Sample a value from a Negative Binomial Law with parameter n and p
     * conditioned on being in the interval [infBound; +inf]
     * */
    int sampleIB(Real n, Real p, int infBound);

    /**
     * Sample a value from a Negative Binomial Law with parameter n and p
     * conditioned on being in the interval [infBound; supBound]
    * */
    int sampleI(Real n, Real p, int infBound, int supBound);


    /** quantile corresponding to proba alpha */
    int quantile(Real n, Real p, Real alpha) const;
    int quantileIB(Real n, Real p, int infBound, Real alpha) const;
    int quantileI(Real n, Real p, int infBound, int supBound, Real alpha) const;


private:
	/** Random number generator */
	boost::mt19937 rng_;

	/** Uniform sampler used for nonZeroSample */
	UniformStatistic uniform_;
};

} // namespace mixt

#endif // NEGATIVEBINOMIALSTATISTIC_H
