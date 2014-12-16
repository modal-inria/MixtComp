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
 *  Created on: December 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <ctime>
#include "mixt_NormalStatistic.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/math/distributions/normal.hpp>
#include "../Various/mixt_Constants.h"

namespace mixt
{

NormalStatistic::NormalStatistic() :
    rng_(time(0))
{}

NormalStatistic::~NormalStatistic()
{}

STK::Real NormalStatistic::cdf(int x,
                               STK::Real mean,
                               STK::Real sd) const
{
  boost::math::normal norm(mean,
                           sd);
  STK::Real proba = boost::math::cdf(norm,
                                     x);
  return proba;
}

STK::Real NormalStatistic::pdf(int x,
                               STK::Real mean,
                               STK::Real sd) const
{
  boost::math::normal norm(mean,
                           sd);
  STK::Real proba = boost::math::pdf(norm,
                                     x);
  return proba;
}

int NormalStatistic::sample(STK::Real mean,
                             STK::Real sd)
{
  boost::normal_distribution<> norm(mean,
                                    sd);
  boost::variate_generator<boost::mt19937&,
                           boost::normal_distribution<> > generator(rng_,
                                                                    norm);
  STK::Real x = generator();
  return x;
}

} // namespace mixt
