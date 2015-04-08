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

/**
 * Structure containing the values of an iteration of the BOS algorithm
 */
struct ItBOS
{
    int y_;
    int z_;
    std::pair<int, int> e_;
};

Real yProba(const std::pair<int, int>& e);
Real zProba(int z,
            Real pi);
Real eProba(int y,
            int z,
            const std::pair<int, int>& ePr,
            const std::pair<int, int>& eCurr,
            int mu,
            Real pi);

Real computeProba(const std::pair<int, int>& eInit,
                  const Vector<ItBOS>& c,
                  int x,
                  int mu,
                  Real pi);

/**
 * Multinomial probability distribution for the variable y at a specific index
 *
 * @param c a constant reference to a vector containing the current search path
 * @param eVal a constant reference to a vector containing the first interval of the search path
 * @param proba a reference to the vector with the probability distribution of the sampled variable
 * @param index localization of the segment in which the value to be sampled resides
 */
//void multinomialY(const Vector<int>& c,
//                  const std::pair<int, int>& eVal,
//                  Vector<Real>& proba,
//                  int index);

} // namespace OrdinalProba

} // namespace mixt

#endif // MIXT_ORDINAL
