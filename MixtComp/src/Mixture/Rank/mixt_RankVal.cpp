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
 *  Created on: August 26, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_RankVal.h"

namespace mixt
{
RankVal::RankVal() :
    nbPos_(0)
{}

RankVal::RankVal(int nbPos) :
    nbPos_(nbPos)
{
  ordering_.resize(nbPos);
  ranking_.resize(nbPos);
}

bool RankVal::operator==(const RankVal& rv) const
{
  if (rv.ordering_ == ordering_)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RankVal::operator<(const RankVal& rv) const
{
  return (ordering_ < rv.ordering_);
}

void RankVal::setNbPos(int nbPos)
{
  nbPos_ = nbPos;
  ordering_.resize(nbPos);
  ranking_.resize(nbPos);
}

void RankVal::switchRepresentation(const Vector<int>& mu ,
                                         Vector<int>& muP) const
{
  for (int p = 0; p < nbPos_; ++p)
  {
    muP(mu(p)) = p;
  }
}

void RankVal::permutation(int firstElem)
{
  int dummyO = ordering_(firstElem);
  int dummyR = ranking_(ordering_(firstElem));

  ranking_(ordering_(firstElem    )) = ranking_(ordering_(firstElem + 1));
  ranking_(ordering_(firstElem + 1)) = dummyR;

  ordering_(firstElem    ) = ordering_(firstElem + 1);
  ordering_(firstElem + 1) = dummyO;
}

} // namespace mixt
