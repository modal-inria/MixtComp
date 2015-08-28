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

#include "mixt_Rank.h"

namespace mixt
{
Rank::Rank(int nbClass) :
    nbClass_(nbClass),
    nbInd_(0),
    pi_(0.)
{}

Rank::Rank(int nbClass,
           int nbInd,
           Vector<Vector<int> >& data,
           const RankVal& mu,
           Real pi) :
    nbClass_(nbClass),
    nbInd_(nbInd),
    mu_(mu),
    pi_(pi)
{
  data_.resize(nbInd);
  for (int i = 0; i < nbInd; ++i)
  {
    data_(i).setNbPos(data(i).size());
    data_(i).setO(data(i));
  }
}

void Rank::removeMissing()
{
  for (int i = 0; i < nbInd_; ++i)
  {
    data_(i).removeMissing();
  }
}
} // namespace mixt
