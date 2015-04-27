/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014 - 2015

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
 *  Created on: April 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/random/uniform_int_distribution.hpp>
#include "mixt_MultinomialStatistic.h"

namespace mixt
{

int MultinomialStatistic::sampleInt(int low, int high)
{
  boost::random::uniform_int_distribution<> uni(low,
                                                high);
  boost::variate_generator<boost::random::mt19937&,
                           boost::random::uniform_int_distribution<> > generator(rng_,
                                                                                 uni);
  int x = generator();
  return x;
}

} // namespace mixt
