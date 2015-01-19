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

#include <boost/random/mersenne_twister.hpp>
#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_UniformStatistic.h"
#include "mixt_ExponentialStatistic.h"

namespace mixt
{

class NormalStatistic
{
  public:
    typedef STK::Real Type;

    NormalStatistic();
    ~NormalStatistic();

    /** cdf evaluated at individual x */
    STK::Real cdf(Type x,
                  STK::Real mean,
                  STK::Real sd) const;

    /** log pdf evaluated at individual x */
    STK::Real lpdf(Type x,
                   STK::Real mean,
                   STK::Real sd) const;

    /** pdf evaluated at individual x */
    STK::Real pdf(Type x,
                  STK::Real mean,
                  STK::Real sd) const;

    /** Sample a value from a Normal Law with parameters mean and sd */
    Type sample(STK::Real mean,
                STK::Real sd);

    /** Sample a value from a Normal Law with parameters mean and sd,
     * conditionned on being in the interval [infBound; supBound] */
    Type sampleI(STK::Real mean,
                 STK::Real sd,
                 STK::Real leftBound,
                 STK::Real rightBound);

    /** Sample a value from a Normal Law with parameters mean and sd,
     * conditionned on being in the interval [infBound; +inf] */
    Type sampleIB(STK::Real mean,
                  STK::Real sd,
                  STK::Real infBound);

    /** Sample a value from a Normal Law with parameters mean and sd,
     * conditionned on being in the interval [-inf; supBound] */
    Type sampleSB(STK::Real mean,
                  STK::Real sd,
                  STK::Real supBound);

  private:
    /** helper function to help sample on intervals */
    STK::Real lbSampler(STK::Real lower);

    /** helper function to help sample on intervals */
    STK::Real lrbSampler(STK::Real lower, STK::Real upper);

    /** when lower bound > 0, selects wether to used lbSampler or lrbSampler
     * based on the spread between bounds*/
    STK::Real sideSampler(STK::Real lower, STK::Real upper);

    /** Random number generator */
    boost::mt19937 rng_;

    UniformStatistic uniform_;
    ExponentialStatistic exponential_;
};

} // namespace mixt

#endif // MIXT_NORMALSTATISTIC_H
