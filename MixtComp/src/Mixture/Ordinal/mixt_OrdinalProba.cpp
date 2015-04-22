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

  if (y < e.first || y > e.second) // if y is not in the interval, all elements of the partition are empty
  {
    part(0).first  = -1;
    part(0).second = -1;
    part(1).first  = -1;
    part(1).second = -1;
    part(2).first  = -1;
    part(2).second = -1;
  }
  else
  {
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
#ifdef MC_DEBUG
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
#ifdef MC_DEBUG
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
#ifdef MC_DEBUG
  std::cout << "eProba" << std::endl;
#endif
  Real eProba;

  if (z == 1) // comparison is perfect, and only the best segment has a nonzero probability
  {
#ifdef MC_DEBUG
    std::cout << "z == 1" << std::endl;
#endif
    int closestSegment = -1; // index in partition of the closest segment
    Real disClosestSegment; // distance between mu and closest segment
    for (int s = 0; s < 3; ++s) // computation of the closest segment
    {
  #ifdef MC_DEBUG
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
  #ifdef MC_DEBUG
          std::cout << "\t\tdisCurrSegment: " << disCurrSegment << std::endl;
          std::cout << "\t\tdisClosestSegment: " << disClosestSegment << std::endl;
  #endif
        }
      }
    }

    if (closestSegment > -1 && e == part(closestSegment)) // the closest segment must be defined, and e must then be equal to it.
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
#ifdef MC_DEBUG
    std::cout << "z == 0" << std::endl;
#endif
    int sizePart = 0; // total size of the partition
    eProba = 0.; // by default the segment is assumed absent from the partition, and hence having a null probability
    for (int s = 0; s < 3; ++s) // test if e is among the partition. If this is the case, computation of proba using the size, otherwise proba is zero
    {
      if (part(s).first > -1) // test if current segment is nonempty
      {
        sizePart += part(s).second - part(s).first + 1;
        if (part(s) == e)
        {
          eProba = Real(part(s).second - part(s).first + 1);
        }
      }
    }
    if (sizePart > 0) // the case sizePart = 0 means that y was not in the segment e of the previous iteration. Case is possible during a Gibbs sampling.
    {
      eProba /= Real(sizePart);
    }
    else
    {
      eProba = 0.;
    }
  }

#ifdef MC_DEBUG
    std::cout << "eProba: " << eProba << std::endl;
#endif
  return eProba;
}

Real computeProba(const std::pair<int, int>& eInit,
                  const Vector<ItBOS>& c,
                  int mu,
                  Real pi)
{
#ifdef MC_DEBUG
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

#ifdef MC_DEBUG
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

  return proba;
}

void yMultinomial(const std::pair<int, int>& eInit,
                  const Vector<ItBOS>& c,
                  int mu,
                  Real pi,
                  int index,
                  Vector<Real>& proba,
                  int& minVal)
{
#ifdef MC_DEBUG
  std::cout << "multinomialY" << std::endl;
  std::cout << "index: " << index << std::endl;
#endif
  const std::pair<int, int>* p_preSeg; // const pointer to the segment used to compute partition
  int maxVal;
  if (index == 0)
  {
    p_preSeg = &eInit;
    minVal = eInit.first;
    maxVal = eInit.second;
  }
  else
  {
    p_preSeg = &c(index - 1).e_;
    minVal = c(index - 1).e_.first;
    maxVal = c(index - 1).e_.second;
  }
  int nbVal = maxVal - minVal + 1;
  proba.resize(nbVal);

  for (int y = minVal; y < maxVal + 1; ++y) // loop over values of y allowed by previous segment
  {
    Vector<std::pair<int, int> > part; // partition generated by current y value
    partition(*p_preSeg, // computation of the partition, according to the new breaking point y
              c(index).y_,
              part);
    Real yP = yProba(*p_preSeg,
                     y);
    Real eP = eProba(c(index).z_,
                     part,
                     c(index).e_,
                     mu,
                     pi);
#ifdef MC_DEBUG
    std::cout << "y: " << y << "c(index).z_: " << c(index).z_ << std::endl;
    std::cout << "yP: " << yP << ", eP: " << eP << std::endl;
#endif
    proba(y - minVal) = yP * eP;
  }
  Real sumProba = proba.sum();
  proba /= sumProba; // renormalization of probability vector
#ifdef MC_DEBUG
  std::cout << "sumProba: " << sumProba << std::endl;
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
}

void zMultinomial(const Vector<ItBOS>& c,
                  int mu,
                  Real pi,
                  int index,
                  Vector<Real>& proba)
{
  int nbVal = 2; // 0: blind, 1: perfect
  proba.resize(nbVal);

  for (int z = 0; z < nbVal; ++z)
  {
    Real zP = zProba(z,
                     pi);
    Real eP = eProba(z,
                     c(index).part_,
                     c(index).e_,
                     mu,
                     pi);
    proba(z) = zP * eP;
  }
  Real sumProba = proba.sum();
  proba /= sumProba; // renormalization of probability vector
#ifdef MC_DEBUG
  std::cout << "multinomialZ" << std::endl;
  std::cout << "sumProba: " << sumProba << std::endl;
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
}

void eMultinomial(const Vector<ItBOS>& c,
                  int mu,
                  Real pi,
                  int index,
                  Vector<Real>& proba)
{
#ifdef MC_DEBUG
  std::cout << "eMultinomial" << std::endl;
#endif
  int nbVal = 3; // partition is always composed of three elements
  proba.resize(nbVal);

  for (int e = 0; e < nbVal; ++e)
  {
    Real eP = eProba(c(index).z_,
                     c(index).part_,
                     c(index).part_(e),
                     mu,
                     pi);
    Real eS; // proba of subsequent element
    if (index != c.size() - 1) // subsequent y proba is impacted by the change in e
    {
      eS = yProba(c(index    ).e_,
                  c(index + 1).y_);
    }
    else // proba of the user-provided constraint is impacted by the change in e
    {
      eS = 1.; // no constraints at the moment
    }
    proba(e) = eP * eS;
  }
  Real sumProba = proba.sum();
  proba /= sumProba; // renormalization of probability vector
#ifdef MC_DEBUG
  std::cout << "multinomialZ" << std::endl;
  std::cout << "sumProba: " << sumProba << std::endl;
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
}

void ySample(const std::pair<int, int>& eInit,
             Vector<ItBOS>& c,
             int mu,
             Real pi,
             int index,
             MultinomialStatistic& multi)
{
  int minVal;
  Vector<Real> proba;
  yMultinomial(eInit, // computation of the conditional probability distribution
               c,
               mu,
               pi,
               index,
               proba,
               minVal);
  Real sampleVal = multi.sample(proba);
  c(index).y_ = sampleVal + minVal; // sampled value replaces the current value in the search path c

  if (index == 0) // partition is updated using eInit segment
  {
    partition(eInit,
              c(index).y_,
              c(index).part_);
  }
  else // partition is updated previous iteration segment
  {
    partition(c(index - 1).e_,
              c(index).y_,
              c(index).part_);
  }

#ifdef MC_DEBUG
  std::cout << "ySample, sampleVal: " << sampleVal << std::endl;
  std::cout << proba << std::endl;
#endif
}

void zSample(const std::pair<int, int>& eInit,
             Vector<ItBOS>& c,
             int mu,
             Real pi,
             int index,
             MultinomialStatistic& multi)
{
  Vector<Real> proba;
  zMultinomial(c,
               mu,
               pi,
               index,
               proba);
  Real sampleVal = multi.sample(proba);
  c(index).z_ = sampleVal; // sampled value replaces the current value in the search path c
#ifdef MC_DEBUG
  std::cout << "zSample, sampleVal: " << sampleVal << std::endl;
  std::cout << proba << std::endl;
#endif
}

void eSample(const std::pair<int, int>& eInit,
             Vector<ItBOS>& c,
             int mu,
             Real pi,
             int index,
             MultinomialStatistic& multi)
{
#ifdef MC_DEBUG
  std::cout << "eSample" << std::endl;
  for (int i = 0; i < 3; ++i)
  {
    std::cout << "c(index).part_(i).first: " << c(index).part_(i).first << std::endl;
    std::cout << "c(index).part_(i).second: " << c(index).part_(i).second << std::endl;
  }
#endif
  Vector<Real> proba;
  eMultinomial(c,
               mu,
               pi,
               index,
               proba);
#ifdef MC_DEBUG
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
  int sampleSegIndex = multi.sample(proba);
  std::pair<int, int> sampleVal(c(index).part_(sampleSegIndex));
  c(index).e_ = sampleVal; // sampled value replaces the current value in the search path c
#ifdef MC_DEBUG
  std::cout << "eSample, sampleVal.first: " << sampleVal.first << ", sampleVal.second: " <<  sampleVal.second << std::endl;
  std::cout << "proba" << std::endl;
  std::cout << proba << std::endl;
#endif
}

void samplePath(const std::pair<int, int>& eInit,
           Vector<ItBOS>& c,
           int mu,
           Real pi,
           MultinomialStatistic& multi)
{
  for (int i = 0; i < c.size(); ++i)
  {
    ySample(eInit,
            c,
            mu,
            pi,
            i,
            multi);
    zSample(eInit,
            c,
            mu,
            pi,
            i,
            multi);
    eSample(eInit,
            c,
            mu,
            pi,
            i,
            multi);
  }
}

} // namespace OrdinalProba

} // namespace mixt
