/*
 * UniformIntStatistic.h
 *
 *  Created on: 17 déc. 2018
 *      Author: Quentin Grimonprez
 */

#ifndef LIB_STATISTIC_UNIFORMINTSTATISTIC_H_
#define LIB_STATISTIC_UNIFORMINTSTATISTIC_H_


#include "../LinAlg/mixt_LinAlg.h"
#include <boost/random/mersenne_twister.hpp>

namespace mixt
{

class UniformIntStatistic
{
  public:
    UniformIntStatistic();

    /** cdf evaluated at individual x */
    Real cdf(int x, int min, int max) const;

    /** pdf evaluated at individual x */
    Real pdf(int x, int min, int max) const;

    /**
     * Sample an integer from a uniform distribution
     * @param min lower bound of the support
     * @param max higher bound of the support
     * @return integer uniformly sampled from [low:high]
     */
    int sample(int min, int max);

  private:
    /** Random number generator */
    boost::random::mt19937 rng_;
};

} // namespace mixt


#endif /* LIB_STATISTIC_UNIFORMINTSTATISTIC_H_ */
