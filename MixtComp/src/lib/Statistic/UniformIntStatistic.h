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
 * UniformIntStatistic.h
 *
 *  Created on: 17 déc. 2018
 *      Author: Quentin Grimonprez
 */

#ifndef LIB_STATISTIC_UNIFORMINTSTATISTIC_H_
#define LIB_STATISTIC_UNIFORMINTSTATISTIC_H_


#include <boost/random/mersenne_twister.hpp>
#include <LinAlg/LinAlg.h>

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
