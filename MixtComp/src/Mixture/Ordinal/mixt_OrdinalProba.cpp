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
#include "../../Various/mixt_Constants.h"

namespace mixt
{

namespace OrdinalProba
{

// format of interval e is a vector with bounds included: [1, 3] corresponds to the set {1, 2, 3}

void initPath(const Vector<int, 2>& initSeg,
              const Vector<int, 2>& endCond,
              MultinomialStatistic& multi,
              Vector<BOSNode>& c)
{
#ifdef MC_DEBUG
  std::cout << "initPath" << std::endl;
#endif
  int nbSegment = initSeg(1) - initSeg(0); // number of segments in the path

  c.resize(nbSegment);

#ifdef MC_DEBUG
  std::cout << "endCond(0): " << endCond(0) << ", endCond(1): " << endCond(1) << std::endl;
#endif
  Vector<int, 2> seg = initSeg;

  for (int i = 0; i < nbSegment; ++i) // loop to fill all the elements of the path
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
#endif
    c(i).y_ = multi.sampleInt(seg(0), seg(1));
    partition(seg,
              c(i).y_,
              c(i).part_);
    c(i).z_ = 0; // comparisons are all blind in initialization
    Vector<Real> segProba(3);
    for (int s = 0; s < 3; ++s) // computation of the allowed segments
    {
      if ( c(i).part_(s)(0) > -1       && // only non-empty segments are considered
           c(i).part_(s)(0) <= endCond(1) && // test if the current segment of the partition can reach any allowed point
           endCond(0)       <= c(i).part_(s)(1) )
        segProba(s) = c(i).part_(s)(1) - c(i).part_(s)(0) + 1; // proba to sample segment is proportional
      else
        segProba(s) = 0.;
    }
    segProba /= segProba.sum();
    int sampleSeg = multi.sample(segProba);
#ifdef MC_DEBUG
    std::cout << "segProba" << std::endl;
    std::cout << segProba << std::endl;
    std::cout << "sampleSeg: " << sampleSeg << std::endl;
#endif
    c(i).e_ = c(i).part_(sampleSeg);
    seg = c(i).e_;
#ifdef MC_DEBUG
    std::cout << "currNode" << std::endl;
    displaySegNode(c(i));
#endif
  }
}

void displaySegNode(const BOSNode& node)
{
  std::cout << "\ty: " << node.y_ << std::endl;
  std::cout << "\tpart: " << std::endl;
  for (int s = 0; s < 3; ++s)
  {
    std::cout << "\t\tpart(" << s << ")(0): "  << node.part_(s)(0)
              << ", part(" << s << ")(1): "   << node.part_(s)(1) << std::endl;
  }
  std::cout << "\tz: " << node.z_ << std::endl;
  std::cout << "\te(0): " << node.e_(0) << ", e(1): " << node.e_(1) << std::endl;
}

void displayPath(const Vector<int, 2>& eInit,
                 const Vector<BOSNode>& c)
{
  std::cout << "eInit(0): " << eInit(0) << ", eInit(1): " << eInit(1) << std::endl;
  for (int node = 0; node < c.size(); ++node)
  {
    std::cout << "node: " << node << std::endl;
    displaySegNode(c(node));
  }
}

void partition(const Vector<int, 2>& e,
               int y,
               Vector<Vector<int, 2>, 3>& part)
{
#ifdef MC_DEBUG
  std::cout << "partition" << std::endl;
#endif
  part.resize(3); // list of candidates for next e_j. Candidates on first dimension, bounds on second dimension

  if (y < e(0) || y > e(1)) // if y is not in the interval, all elements of the partition are empty
  {
    part(0)(0)  = -1;
    part(0)(1) = -1;
    part(1)(0)  = -1;
    part(1)(1) = -1;
    part(2)(0)  = -1;
    part(2)(1) = -1;
  }
  else
  {
    if (e(0) != y) // is the left interval non-empty ? If not, partition element will be an empty vector
    {
      part(0)(0)  = e(0);
      part(0)(1) = y - 1;
    }
    else
    {
      part(0)(0)  = -1; // impossible value used to designate an empty set
      part(0)(1) = -1;
    }
    part(1)(0)  = y; // center interval always contains the center element
    part(1)(1) = y;
    if (e(1) != y) // is the right interval non-empty ? If not, partition element will be an empty vector
    {
      part(2)(0)  = y + 1;
      part(2)(1) = e(1);
    }
    else
    {
      part(2)(0)  = -1; // impossible value used to designate an empty set
      part(2)(1) = -1;
    }
  }
#ifdef MC_DEBUG
  std::cout << "end partition" << std::endl;
#endif
}

Real yProba(const Vector<int, 2>& e,
            int y)
{
  Real yProba;
  if (e(0) <= y && y <= e(1)) // y is among the last segment values
  {
    yProba = 1. / Real(e(1) - e(0) + 1.);
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
            const Vector<Vector<int, 2> >& part,
            const Vector<int, 2>& e,
            int mu,
            Real pi)
{
#ifdef MC_DEBUG
  std::cout << "eProba" << std::endl;
  std::cout << "e(0): " << e(0) << ", e(1): " << e(1) << std::endl;
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
      std::cout << "\ts: " << s << ", part(s)(0): " << part(s)(0) << ", part(s)(1): " << part(s)(1) << std::endl;
  #endif
      if (part(s)(0) > -1) // pair containing {-1, -1} are ignored, as they describe an empty segment
      {
        Real disCurrSegment = std::min(std::abs(mu - part(s)(0)),
                                       std::abs(mu - part(s)(1)));
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
#ifdef MC_DEBUG
      std::cout << "s: " << s << ", part(s)(0): " << part(s)(0) << ", part(s)(1): " << part(s)(1) << std::endl;
#endif
      if (part(s)(0) > -1) // test if current segment is nonempty
      {
        sizePart += part(s)(1) - part(s)(0) + 1;
        if (part(s) == e)
        {
#ifdef MC_DEBUG
          std::cout << "part(s) == e, s: " << s << std::endl;
#endif
          eProba = Real(part(s)(1) - part(s)(0) + 1);
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

Real computeProba(const Vector<int, 2>& initSeg,
                  const Vector<BOSNode>& c,
                  const Vector<int, 2>& endCond,
                  int mu,
                  Real pi)
{
#ifdef MC_DEBUG
  std::cout << "OrdinalProba::computeProba" << std::endl;
#endif
  Real proba = 1.; // The initial probability of being in any of the member of the input interval is 1

  int nbSegment = initSeg(1) - initSeg(0); // number of segments in the path

  if (endCond(0) <= c(nbSegment - 1).e_(0) && c(nbSegment - 1).e_(0) <= endCond(1)) // is the path compatible with the provided condition ?
  {
    for (int i = 0; i < c.size(); ++i) // loop over triplets of path variables
    {
      int y = c(i).y_; // breakpoint
      int z = c(i).z_; // accuracy
      Vector<int, 2> ePr; // previous iteration segment
      if (i == 0)
      {
        ePr = initSeg;
      }
      else
      {
        ePr = c(i - 1).e_; // last iteration segment
      }
      const Vector<Vector<int, 2> >& part = c(i).part_; // current iteration partition
      const Vector<int, 2>& e = c(i).e_; // current iteration segment segment

  #ifdef MC_DEBUG
      std::cout << "i: " << i
                << ", y: " << y
                << ", z: " << z
                << ", ePr(0): " << ePr(0) << ", ePr(1): " << ePr(1)
                << ", e(0): " << e(0) << ", e(1): " << e(1) << std::endl;
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
  }
  else // conditional probability of verifying condition given the path is null
      proba = 0.;

  return proba;
}

void nodeMultinomial(const Vector<int, 2>& eInit,
                     const Vector<int, 2>& endCond,
                     int mu,
                     Real pi,
                     std::list<Vector<BOSNode, 2> >& pathList,
                     std::list<Real>& probaList)
{
  for (int y0 = eInit(0); y0 < eInit(1) + 1; ++y0) // outer node
  {
    Vector<Vector<int, 2>, 3> part0; // partition corresponding to y0 value
    partition(eInit,
              y0,
              part0);
    Real y0proba = yProba(eInit, y0);
#ifdef MC_DEBUG
    std::cout << "y0: " << y0 << std::endl;
    std::cout << "\ty0proba: " << y0proba << std::endl;
#endif
    for (int z0 = 0; z0 < 2; ++z0)
    {
      Real z0proba = y0proba * zProba(z0, pi);
#ifdef MC_DEBUG
      std::cout << "y0: " << y0 << ", z0: " << z0 << std::endl;
      std::cout << "\tz0proba: " << z0proba << std::endl;
#endif
      for(int e0 = 0; e0 < 3; ++e0)
      {
        Real e0proba = z0proba * eProba(z0,
                                        part0,
                                        part0(e0),
                                        mu,
                                        pi);
#ifdef MC_DEBUG
        std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
        std::cout << "\te0proba: " << e0proba << std::endl;
#endif
        if (e0proba > epsilon) // null probability segments are not computed further
        {
          for(int y1 = part0(e0)(0); y1 < part0(e0)(0) + 1; ++y1) // inner node
          {
            Vector<Vector<int, 2>, 3> part1; // partition corresponding to y1 value
            partition(part0(e0),
                      y1,
                      part1);
            Real y1proba = e0proba * yProba(part0(e0), y1);
#ifdef MC_DEBUG
            std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
            std::cout << "y1: " << y1 << std::endl;
            std::cout << "\ty1proba: " << y1proba << std::endl;
#endif
            for (int z1 = 0; z1 < 2; ++z1)
            {
              Real z1proba = y1proba * zProba(z1, pi);
#ifdef MC_DEBUG
              std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
              std::cout << "y1: " << y1 << ", z1: " << z1 << std::endl;
              std::cout << "\tz1proba: " << z1proba << std::endl;
#endif
              for(int e1 = 0; e1 < 3; ++e1)
              {
                Real e1proba = z1proba * eProba(z1,
                                                part1,
                                                part1(e1),
                                                mu,
                                                pi);
#ifdef MC_DEBUG
                std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
                std::cout << "y1: " << y1 << ", z1: " << z1 << ", e1: " << e1 << ", part1(e1)(0): " << part1(e1)(0) << ", part1(e1)(1): " << part1(e1)(1) << std::endl;
                std::cout << "\te1proba: " << e1proba << std::endl;
#endif
                if (e1proba > epsilon && part1(e1)(0) <= endCond(0) && part1(e1)(1) <= endCond(1)) // is the final condition verified ?
                {
                    pathList.push_back(Vector<BOSNode, 2>());
                    pathList.back()(0).y_ = y0;
                    pathList.back()(0).part_ = part0;
                    pathList.back()(0).z_ = z0;
                    pathList.back()(0).e_ = part0(e0);
                    pathList.back()(1).y_ = y1;
                    pathList.back()(1).part_ = part1;
                    pathList.back()(1).z_ = z1;
                    pathList.back()(1).e_ = part1(e1);
                    probaList.push_back(e1proba); // proba of current path is saved
                }
                else
                {
#ifdef MC_DEBUG
                    std::cout << "e1, null proba case or final condition not verified" << std::endl;
#endif
                }
              }
            }
          }
        }
        else
        {
#ifdef MC_DEBUG
          std::cout << "e0, null proba case detected" << std::endl;
#endif
        }
      }
    }
  }
}

void samplePath(const Vector<int, 2>& eInit,
                Vector<BOSNode>& c,
                const Vector<int, 2>& endCond,
                int mu,
                Real pi,
                MultinomialStatistic& multi)
{
//  for (int i = 0; i < c.size(); ++i)
//  {
//    ySample(eInit,
//            c,
//            mu,
//            pi,
//            i,
//            multi);
//    zSample(eInit,
//            c,
//            mu,
//            pi,
//            i,
//            multi);
//    eSample(eInit,
//            c,
//            endCond,
//            mu,
//            pi,
//            i,
//            multi);
//  }
//#ifdef MC_DEBUG
// displayPath(eInit,
//             c);
//#endif
}

} // namespace OrdinalProba

} // namespace mixt
