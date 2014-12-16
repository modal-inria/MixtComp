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
 *  Created on: December 15, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_NORMALSTATISTIC_H
#define MIXT_NORMALSTATISTIC_H

#include "STKernel/include/STK_Real.h"
#include <boost/random/mersenne_twister.hpp>

namespace mixt
{

class NormalStatistic
{
  public:
    NormalStatistic();
    ~NormalStatistic();

    /** cdf evaluated at individual x */
    STK::Real cdf(int x,
                  STK::Real mean,
                  STK::Real sd) const;

    /** pdf evaluated at individual x */
    STK::Real pdf(int x,
                  STK::Real mean,
                  STK::Real sd) const;

    /** Sample a value from a Normal Law with parameters mean and sd */
    int sample(STK::Real mean,
               STK::Real sd);
  private:
    /** Random number generator */
    boost::mt19937 rng_;
};

} // namespace mixt

#endif // MIXT_NORMALSTATISTIC_H
