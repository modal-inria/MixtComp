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

#include "mixt_OrdinalProba.h"

namespace mixt
{

namespace OrdinalProba
{

// Possible optimization: first look at the values of e beginning with the end, if at any moment x is not inside the interval, then the probability of the chain is 0.
// If the SEM Gibbs is initialized correctly, it will never stray into 0 probability event, so safeguards of that sort are useless.
// vector c contains a series of [y, z, e, y, z, e, ... ]
// format of interval e is a vector with bounds included: [1, 3] corresponds to the set {1, 2, 3}

Real yProba(const std::pair<int, int>& e)
{
  return 1. / Real(e.second - e.first + 1.) ; // conditional probability of y, which only depends of the size of the interval
}

Real zProba(int z,
            Real pi)
{
  Real proba;
  if (z == 1) // comparison is perfect
  {
    proba = pi;
  }
  else
  {
    proba = (1. - pi); // comparison is blind
  }
  return proba;
}

Real eProba(std::pair<int, int>& eVal, // the subinterval will be modified in this function, hence the non const reference
            int y,
            int z,
            int e,
            int mu,
            Real pi)
{
  Real proba;
  Vector<std::pair<int, int> > partition(3); // list of candidates for next e_j. Candidates on first dimension, bounds on second dimension
  std::cout << partition.size() << std::endl;
  if (eVal.first != y) // is the left interval non-empty ? If not, partition element will be an empty vector
  {
    partition(0).first  = eVal.first;
    partition(0).second = y - 1;
  }
  else
  {
    partition(0).first = -1; // impossible value used to designate an empty set
  }
  partition(1).first  = y; // center interval always contains the center element
  partition(1).second = y;
  if (eVal.second != y) // is the right interval non-empty ? If not, partition element will be an empty vector
  {
    partition(2).first  = y + 1;
    partition(2).second = eVal.second;
  }
  else
  {
    partition(2).first = -1; // impossible value used to designate an empty set
  }
  int closestSegment = -1; // index in partition of the closest segment
  Real disClosestSegment; // distance between mu and closest segment
  for (int s = 0; s < 3; ++s) // computation of the closest segment
  {
    if (partition(s).first > 0) // empty vectors are ignored, as they describe an empty segment
    {
      Real disCurrSegment = std::min(std::abs(mu - partition(s).first),
                                     std::abs(mu - partition(s).second));
      if ( disCurrSegment < disClosestSegment || s < 0) // a new closest segment has been found
      {
        closestSegment = s;
        disClosestSegment = disCurrSegment;
      }
    }
  }

  if (z == 1) // if comparison is perfect, anything but the best interval has a 0 conditional probability
  {
    if (e == closestSegment)
    {
      proba = 1.;
    }
    else
    {
      proba = 0.;
    }
  }
  else // comparison is imperfect, probability of the selected subinterval is proportional to its size
  {
    proba = Real(partition(e).second - partition(e).first) / Real(eVal.second - eVal.first);
  }

  eVal = partition(e); // segment is updated

  return proba;
}

Real computeProba(const Vector<int>& c,
                  int x,
                  std::pair<int, int> eVal,
                  int mu,
                  Real pi)
{
  Real proba = 1.; // The initial probability of being in any of the member of the input interval is 1

  for (int i = 0; i < c.size() / 3; ++i) // loop over triplets of path variables
  {
    int y = c(i    ); // breakpoint
    int z = c(i + 1); // accuracy
    int e = c(i + 2); // segment

    // probability of y
    proba *= yProba(eVal);

    // probability of z
    proba *= zProba(z, pi);

    // probability of e
    proba *= eProba(eVal,
                    y,
                    z,
                    e,
                    mu,
                    pi);
  }

  if (eVal.first != x) // if the content of the segment is different from x, then the proba to get x is null
  {
    proba = 0.;
  }

  return proba;
}

} // namespace OrdinalProba

} // namespace mixt
