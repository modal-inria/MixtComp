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

Real yProba(const std::pair<int, int>& e,
            int y)
{
  Real yProba;
  if (e.first <= y && y <= e.second) // y is among the last segment values
  {
    yProba = 1. / Real(e.second - e.first + 1.);
  }
  else
  {
    yProba = 0.;
  }
#ifdef MC_DEBUG_NEW
    std::cout << "yProba: " << yProba << std::endl;
#endif
  return yProba; // conditional probability of y, which only depends of the size of the interval
}

Real zProba(int z,
            Real pi)
{
  Real zProba;
  if (z == 1) // comparison is perfect
  {
    zProba = pi;
  }
  else
  {
    zProba = (1. - pi); // comparison is blind
  }
#ifdef MC_DEBUG_NEW
    std::cout << "zProba: " << zProba << std::endl;
#endif
  return zProba;
}

Real eProba(int y,
            int z,
            const std::pair<int, int>& ePr,
            const std::pair<int, int>& eCurr,
            int mu,
            Real pi)
{
#ifdef MC_DEBUG_NEW
  std::cout << "eProba" << std::endl;
  std::cout << "e.first: " << ePr.first << ", e.second: " << ePr.second << std::endl;
#endif
  Real eProba;
  Vector<std::pair<int, int> > partition(3); // list of candidates for next e_j. Candidates on first dimension, bounds on second dimension
  if (ePr.first != y) // is the left interval non-empty ? If not, partition element will be an empty vector
  {
    partition(0).first  = ePr.first;
    partition(0).second = y - 1;
  }
  else
  {
    partition(0).first  = -1; // impossible value used to designate an empty set
    partition(0).second = -1;
  }
  partition(1).first  = y; // center interval always contains the center element
  partition(1).second = y;
  if (ePr.second != y) // is the right interval non-empty ? If not, partition element will be an empty vector
  {
    partition(2).first  = y + 1;
    partition(2).second = ePr.second;
  }
  else
  {
    partition(2).first  = -1; // impossible value used to designate an empty set
    partition(2).second = -1;
  }
  int closestSegment = -1; // index in partition of the closest segment
  Real disClosestSegment; // distance between mu and closest segment
  for (int s = 0; s < 3; ++s) // computation of the closest segment
  {
#ifdef MC_DEBUG_NEW
    std::cout << "\ts: " << s << ", partition(s).first: " << partition(s).first << ", partition(s).second: " << partition(s).second << std::endl;
#endif
    if (partition(s).first > -1) // pair containing {-1, -1} are ignored, as they describe an empty segment
    {
      Real disCurrSegment = std::min(std::abs(mu - partition(s).first),
                                     std::abs(mu - partition(s).second));
      if (disCurrSegment < disClosestSegment || closestSegment == -1) // a new closest segment has been found, or for the first valid segment
      {
        closestSegment = s;
        disClosestSegment = disCurrSegment;
#ifdef MC_DEBUG_NEW
      std::cout << "\t\tdisCurrSegment: " << disCurrSegment << std::endl;
      std::cout << "\t\tdisClosestSegment: " << disClosestSegment << std::endl;
#endif
      }
    }
  }

#ifdef MC_DEBUG_NEW
  std::cout << "\tclosestSegment: " << closestSegment << std::endl;
  std::cout << "\tdisClosestSegment: " << disClosestSegment << std::endl;
#endif

  if (z == 1) // if comparison is perfect, anything but the best interval has a 0 conditional probability
  {
    if (eCurr == partition(closestSegment))
    {
      eProba = 1.;
    }
    else
    {
      eProba = 0.;
    }
  }
  else // comparison is imperfect, probability of the selected subinterval is proportional to its size
  {
    eProba = Real(partition(closestSegment).second - partition(closestSegment).first + 1) / Real(ePr.second - ePr.first + 1);
  }

#ifdef MC_DEBUG_NEW
    std::cout << "eProba: " << eProba << std::endl;
#endif
  return eProba;
}

Real xProba(int x,
            std::pair<int, int> eVal)
{
#ifdef MC_DEBUG_NEW
  std::cout << "xProba, eVal.first: " << eVal.first << ", eVal.second: " << eVal.second << ", x: " << x << std::endl;
#endif
  Real xProba;
  if (eVal.first == x) // if the content of the segment is different from x, then the proba to get x is null
  {
    xProba = 1.;
  }
  else
  {
    xProba = 0.;
  }
#ifdef MC_DEBUG_NEW
    std::cout << "xProba: " << xProba << std::endl;
#endif
  return xProba;
}

Real computeProba(const std::pair<int, int>& eInit,
                  const Vector<ItBOS>& c,
                  int x,
                  int mu,
                  Real pi)
{
#ifdef MC_DEBUG_NEW
    std::cout << "OrdinalProba::computeProba" << std::endl;
#endif
  Real proba = 1.; // The initial probability of being in any of the member of the input interval is 1

  for (int i = 0; i < c.size(); ++i) // loop over triplets of path variables
  {
    int y = c(i).y_; // breakpoint
    int z = c(i).z_; // accuracy
    std::pair<int, int> ePr; // previous iteration segment
    if (i == 0)
    {
      ePr = eInit;
    }
    else
    {
      ePr = c(i - 1).e_;
    }
    const std::pair<int, int>& eCur = c(i).e_; // current iteration segment

#ifdef MC_DEBUG_NEW
    std::cout << "i: " << i
              << ", y: " << y
              << ", z: " << z
              << ", ePr.first: " << ePr.first << ", ePr.second: " << ePr.second
              << ", eCur.first: " << eCur.first << ", eCur.second: " << eCur.second << std::endl;
#endif

    // probability of y
    proba *= yProba(ePr,
                    y);

    // probability of z
    proba *= zProba(z,
                    pi);

    // probability of e
    proba *= eProba(y,
                    z,
                    ePr,
                    eCur,
                    mu,
                    pi);
  }

  // probability of x
  proba *= xProba(x, c(c.size() - 1).e_); // comparison of the last segment with the sampled x value

  return proba;
}

void multinomialY(const std::pair<int, int>& eInit,
                  Vector<ItBOS>& c,
                  int x,
                  int mu,
                  Real pi,
                  Vector<Real>& proba,
                  int index)
{
  int yBack = c(index).y_; // current y value is backed-up
  int nbVal = eInit.second - eInit.first + 1;
  proba.resize(nbVal);

  for (int i = 0; i < nbVal; ++i)
  {
    c(index).y_ = i; // y value is replaced in-place in the path
    proba(i) = computeProba(eInit,
                            c,
                            x,
                            mu,
                            pi);
  }
  Real sumProba = proba.sum();
  proba /= sumProba; // renormalization of probability vector
  c(index).y_ = yBack; // initial y value is restored
}

} // namespace OrdinalProba

} // namespace mixt
