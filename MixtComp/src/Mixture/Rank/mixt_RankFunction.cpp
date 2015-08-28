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
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <set>

#include "../LinAlg/mixt_LinAlg.h"
#include "../LinAlg/mixt_Math.h"

namespace mixt
{

namespace RankFunction
{

void index2Vector(Vector<std::pair<Vector<int>, Real> >& res,
                  Vector<int>& vec,
                  const std::set<int>& remainingMod,
                  int firstElem,
                  int nbElem,
                  int currPos,
                  int nbPos)
{
  if (currPos == nbPos) // no more modalities to add in the vector
  {
    res(firstElem) = std::pair<Vector<int>, Real>(vec, 12.); // register current vector and its value
  }
  else
  {
    int nextNbElem = nbElem / (nbPos - currPos);
    int indexMod = 0;
    for (std::set<int>::const_iterator it = remainingMod.begin();
         it != remainingMod.end();
         ++it, ++indexMod) // iteration over the modality that have not yet been included in the vector
    {
      std::set<int> remainingModNew = remainingMod; // computation of the modalities available for the remaining of vec
      for (std::set<int>::iterator itNew = remainingModNew.begin();
           itNew != remainingModNew.end();
           ++itNew)
      {
        if (*itNew == *it) // the current modality will not be available further down the vector
        {
          remainingModNew.erase(itNew);
          break;
        }
      }

      vec(currPos) = *it;
      index2Vector(res,
                   vec,
                   remainingModNew,
                   firstElem + indexMod * nextNbElem,
                   nextNbElem,
                   currPos + 1,
                   nbPos);
    }
  }
}

} // namespace RankFunction

} // namespace mixt
