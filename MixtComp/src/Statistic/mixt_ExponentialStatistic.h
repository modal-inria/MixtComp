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

#ifndef MIXT_EXPONENTIALSTATISTIC_H
#define MIXT_EXPONENTIALSTATISTIC_H

#include "../LinAlg/mixt_LinAlg.h"
#include <boost/random/mersenne_twister.hpp>

namespace mixt
{

class ExponentialStatistic
{
  public:
    typedef STK::Real Type;

    ExponentialStatistic();
    ~ExponentialStatistic();

    /** cdf evaluated at individual x */
    STK::Real cdf(Type x,
                  STK::Real lambda) const;

    /** pdf evaluated at individual x */
    STK::Real pdf(Type x,
                  STK::Real lambda) const;

    /** Sample a value from an Exponential law with rate lambda */
    Type sample(STK::Real lambda);
  private:
    /** Random number generator */
    boost::random::mt19937 rng_;
};

} // namespace mixt

#endif // MIXT_EXPONENTIALSTATISTIC_H
