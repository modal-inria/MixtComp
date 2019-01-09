/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 16, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef EXPONENTIALSTATISTIC_H
#define EXPONENTIALSTATISTIC_H

#include "../LinAlg/LinAlg.h"
#include <boost/random/mersenne_twister.hpp>

namespace mixt
{

class ExponentialStatistic
{
  public:
    ExponentialStatistic();

    /** cdf evaluated at individual x */
    Real cdf(Real x,
             Real lambda) const;

    /** pdf evaluated at individual x */
    Real pdf(Real x,
             Real lambda) const;

    /** Sample a value from an Exponential law with rate lambda */
    Real sample(Real lambda);
  private:
    /** Random number generator */
    boost::random::mt19937 rng_;
};

} // namespace mixt

#endif // EXPONENTIALSTATISTIC_H
