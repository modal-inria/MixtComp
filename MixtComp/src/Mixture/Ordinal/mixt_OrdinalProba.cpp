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

// format of interval e is a vector with bounds included: [1, 3] corresponds to the set {1, 2, 3}

void partition(const std::pair<int, int>& e,
               int y,
               Vector<std::pair<int, int> >& part)
{
  part.resize(3); // list of candidates for next e_j. Candidates on first dimension, bounds on second dimension
  if (e.first != y) // is the left interval non-empty ? If not, partition element will be an empty vector
  {
    part(0).first  = e.first;
    part(0).second = y - 1;
  }
  else
  {
    part(0).first  = -1; // impossible value used to designate an empty set
    part(0).second = -1;
  }
  part(1).first  = y; // center interval always contains the center element
  part(1).second = y;
  if (e.second != y) // is the right interval non-empty ? If not, partition element will be an empty vector
  {
    part(2).first  = y + 1;
    part(2).second = e.second;
  }
  else
  {
    part(2).first  = -1; // impossible value used to designate an empty set
    part(2).second = -1;
  }
}

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

Real eProba(int z,
            const Vector<std::pair<int, int> >& part,
            const std::pair<int, int>& e,
            int mu,
            Real pi)
{
#ifdef MC_DEBUG_NEW
  std::cout << "eProba" << std::endl;
#endif
  Real eProba;

  if (z == 1) // comparison is perfect, and only the best segment has a nonzero probability
  {
    int closestSegment = -1; // index in partition of the closest segment
    Real disClosestSegment; // distance between mu and closest segment
    for (int s = 0; s < 3; ++s) // computation of the closest segment
    {
  #ifdef MC_DEBUG_NEW
      std::cout << "\ts: " << s << ", part(s).first: " << part(s).first << ", part(s).second: " << part(s).second << std::endl;
  #endif
      if (part(s).first > -1) // pair containing {-1, -1} are ignored, as they describe an empty segment
      {
        Real disCurrSegment = std::min(std::abs(mu - part(s).first),
                                       std::abs(mu - part(s).second));
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

    if (e == part(closestSegment))
    {
      eProba = 1.;
    }
    else
    {
      eProba = 0.;
    }
  }
  else // comparison is blind, and proba is based on sizes of segments
  {
    int sizePart = 0; // total size of the partition
    eProba = 0.; // by default the segment is assumed absent from the partition, and hence having a null probability
    for (int s = 0; s < 3; ++s) // test if e is among the partition. If this is the case, computation of proba using the size, otherwise proba is zero
    {
      sizePart += part(s).second - part(s).first + 1;
      if (part(s) == e)
      {
        eProba = Real(part(s).second - part(s).first + 1);
      }
    }
    eProba /= Real(sizePart);
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
      ePr = c(i - 1).e_; // last iteration segment
    }
    const Vector<std::pair<int, int> >& part = c(i).part_; // current iteration partition
    const std::pair<int, int>& e = c(i).e_; // current iteration segment segment

#ifdef MC_DEBUG_NEW
    std::cout << "i: " << i
              << ", y: " << y
              << ", z: " << z
              << ", ePr.first: " << ePr.first << ", ePr.second: " << ePr.second
              << ", e.first: " << e.first << ", e.second: " << e.second << std::endl;
#endif
    proba *= yProba(ePr,
                    y);

    proba *= zProba(z,
                    pi);

    proba *= eProba(z,
                    part,
                    e,
                    mu,
                    pi);
  }

  // probability of x
  int lastElem = c.size() - 1;
  const std::pair<int, int>& lastSeg = c(lastElem).e_; // last iteration segment
  proba *= xProba(x,
                  lastSeg); // comparison of the last segment with the sampled x value

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
#ifdef MC_DEBUG_NEW
  std::cout << "multinomialY" << std::endl;
  std::cout << "sumProba: " << sumProba << std::endl;
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
  c(index).y_ = yBack; // initial y value is restored
}

void multinomialZ(const std::pair<int, int>& eInit,
                  Vector<ItBOS>& c,
                  int x,
                  int mu,
                  Real pi,
                  Vector<Real>& proba,
                  int index)
{
  int nbVal = 2; // only two possible values for z
  int zBack = c(index).z_; // current z value is backed-up
  proba.resize(nbVal);
  Vector<int> vals(nbVal);
  vals(0) = 0;
  vals(1) = 1;

  for (int i = 0; i < nbVal; ++i)
  {
    c(index).z_ = i; // z value is replaced in-place in the path
    proba(i) = computeProba(eInit,
                            c,
                            x,
                            mu,
                            pi);
  }
  Real sumProba = proba.sum();
  proba /= sumProba; // renormalization of probability vector
#ifdef MC_DEBUG_NEW
  std::cout << "multinomialZ" << std::endl;
  std::cout << "sumProba: " << sumProba << std::endl;
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
  c(index).z_ = zBack; // initial z value is restored
}

} // namespace OrdinalProba

} // namespace mixt
