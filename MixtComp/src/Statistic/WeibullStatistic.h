/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 18th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLSTATISTIC_H
#define WEIBULLSTATISTIC_H

#include "../LinAlg/mixt_LinAlg.h"
#include <boost/random/mersenne_twister.hpp>
#include "mixt_UniformStatistic.h"

namespace mixt
{

class WeibullStatistic
{
public:
	WeibullStatistic();

    /** Sample a value from a Weibull Law with parameters lambda and k. */
    Real sample(Real lambda, Real k);
private:
	/** Random number generator */
	boost::random::mt19937 rng_;
};

}

#endif
