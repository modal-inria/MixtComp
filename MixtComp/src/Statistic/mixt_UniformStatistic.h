/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: December 16, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_UNIFORMSTATISTIC_H
#define MIXT_UNIFORMSTATISTIC_H

#include "../LinAlg/mixt_LinAlg.h"
#include <boost/random/mersenne_twister.hpp>

namespace mixt
{

class UniformStatistic
{
  public:
    UniformStatistic();

    /** cdf evaluated at individual x */
    Real cdf(Real x,
             Real min,
             Real max) const;

    /** pdf evaluated at individual x */
    Real pdf(Real x,
             Real min,
             Real max) const;

    /**
     * Sample a Real from a uniform distribution
     * @param min lower bound of the support
     * @param max higher bound of the support
     * @return Real uniformly sampled from [low:high]
     */
    Real sample(Real min,
                Real max);
  private:
    /** Random number generator */
    boost::random::mt19937 rng_;
};

} // namespace mixt

#endif // MIXT_UNIFORMSTATISTIC_H
