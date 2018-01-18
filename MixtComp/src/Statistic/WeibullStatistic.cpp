/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 18th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "WeibullStatistic.h"
#include "mixt_RNG.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/weibull_distribution.hpp>

namespace mixt {

WeibullStatistic::WeibullStatistic() :
    rng_(seed(this))
{}

Real WeibullStatistic::sample(Real lambda, Real k) {
  boost::random::weibull_distribution<> w(k, lambda);
  boost::variate_generator<boost::mt19937&, boost::random::weibull_distribution<> > generator(rng_, w);
  return generator();
}

}
