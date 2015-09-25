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

void RankVal::operator=(const RankVal& rv)
{
  nbPos_ = rv.nbPos_;
  ordering_ = rv.ordering_;
  ranking_ = rv.ranking_;
}

RankVal::RankVal(const std::initializer_list<int>& il)
{
  nbPos_ = il.size();
  ordering_.resize(nbPos_);
  ranking_ .resize(nbPos_);

  int i = 0;
  for (std::initializer_list<int>::const_iterator it = il.begin(), itEnd = il.end();
       it != itEnd;
       ++it)
  {
    ordering_(i) = *it;
    ++i;
  }

  switchRepresentation(ordering_,
                       ranking_);
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

std::ostream& operator<<(std::ostream& os, const RankVal& rv)
{
  os << rv.ordering_(0);
  for (int i = 1, size = rv.ordering_.size(); i < size; ++i)
  {
    os << " " << rv.ordering_(i);
  }
  return os;
}

} // namespace mixt
