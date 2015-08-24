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
 *  Created on: August 20, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Rank.h"

namespace mixt
{

Rank::Rank() :
    nbPos_(0)
{

}

void Rank::setNbPos(int nbPos)
{
  nbPos_ = nbPos;
  obsData_.resize(nbPos);
  x_.resize(nbPos);

  y_.resize(nbPos);
  for (int p = 0; p < nbPos_; ++p)
  {
    y_(p) = p;
  }
}

void Rank::removeMissing()
{
  multi_.shuffle(y_);
}

Real Rank::xGen(const Vector<int>& muP,
                Real pi)
{
  Real logProba = 0.;

  Real goodlp = std::log(     pi);
  Real badlp  = std::log(1. - pi);

  std::vector<int> x(1); // vector is suboptimal for insertion, but provides contiguous memory storage which will fit in CPU cache
  x.reserve(nbPos_);

  x[0] = y_(0);

  for (int j = 1; j < nbPos_; ++j) // current element in the presentation order, or current size of the x vector
  {
    int currY = y_(j);
    bool yPlaced = false;
    for (int i = 0; i < j; ++i)
    {
      bool comparison = muP(currY) < muP(x[i]); // true if curr elem is correctly ordered

      if (multi_.sampleBinomial(pi)) // is the comparison correct ?
      {
        logProba += goodlp;
      }
      else
      {
        comparison = !comparison;
        logProba += badlp;
      }

      if (comparison) // element j must be placed here
      {
        x.insert(x.begin() + i, currY);
        yPlaced = true;
        break; // no need to test further position for j element
      }
    }
    if (!yPlaced)
    {
      x.push_back(currY); // if element j has not been placed yet, it goes at the end of x
    }
  }

  for (int p = 0; p < nbPos_; ++p)
  {
    x_(p) = x[p];
  }

  return logProba;
}

void Rank::switchRepresentation(const Vector<int>& mu ,
                                      Vector<int>& muP) const
{
  muP.resize(mu.size());
  for (int p = 0; p < nbPos_; ++p)
  {
    muP(mu(p)) = p;
  }
}

} // namespace mixt
