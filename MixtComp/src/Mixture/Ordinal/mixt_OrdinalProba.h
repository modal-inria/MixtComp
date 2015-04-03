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

Real yProba(const std::pair<int, int>& e);
Real zProba(int z,
            Real pi);
Real eProba(std::pair<int, int>& eVal, // the subinterval will be modified in this function, hence the non const reference
            int y,
            int z,
            int e,
            int mu,
            Real pi);

Real computeProba(const Vector<int>& c,
                  int x,
                  std::pair<int, int> eVal,
                  int mu,
                  Real pi);

} // namespace OrdinalProba

} // namespace mixt

#endif // MIXT_ORDINAL
