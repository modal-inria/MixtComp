/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: March 23, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_ORDINAL
#define MIXT_ORDINAL

#include <list>
#include <utility>
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

namespace OrdinalProba
{

/** Factor in the probability of each possible value of y_j
 *  Call order: y -> z -> e */
void y(std::list<std::pair<int, Real> >& probList,
       const int mu,
       const Real pi,
       Real proba,
       const std::vector<int>& eVal);

/** Factor in the probability of the comparison being perfect */
void z(std::list<std::pair<int, Real> >& probList,
       const int mu,
       const Real pi,
       Real proba,
       const std::vector<int>& eVal,
       int yVal);

/** Factor in probability of choosing each of the three possible e_j from the results of the last computes the values of each */
void e(std::list<std::pair<int, Real> >& probList,
       const int mu,
       const Real pi,
       Real proba,
       const std::vector<int>& eVal,
       int yVal,
       int zVal);

} // namespace OrdinalProba

} // namespace mixt

#endif // MIXT_ORDINAL
