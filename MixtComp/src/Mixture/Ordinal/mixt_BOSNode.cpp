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
 *  Created on: April 29, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_BOSNode.h"

namespace mixt
{

void BOSNode::partition(const Vector<int, 2>& e)
{
#ifdef MC_DEBUG
  std::cout << "partition" << std::endl;
#endif
  if (y_ < e(0) || y_ > e(1)) // if y is not in the interval, the partition is empty
  {
    partSize_ = 0;
    part_.resize(partSize_);
  }
  else
  {
    partSize_ = 3; // maximum size of the partition
    int yPos; // position of the "breaking point" segment
    if (e(0) == y_) // is the left segment empty ?
    {
      partSize_ -= 1;
    }
    if (e(1) == y_) // is the right segment empty ?
    {
      partSize_ -= 1;
    }
    part_.resize(partSize_);

    if (e(0) != y_) // is the left interval non-empty ? If not, partition element will be an empty vector
    {
      part_(0)(0) = e(0);
      part_(0)(1) = y_ - 1;
      yPos = 1;
    }
    else
      yPos = 0;
    if (e(1) != y_) // is the right interval non-empty ? If not, partition element will be an empty vector
    {
      part_(partSize_ - 1)(0) = y_ + 1;
      part_(partSize_ - 1)(1) = e(1);
      yPos = partSize_ - 2;
    }
    else
      yPos = partSize_ - 1;
    part_(yPos)(0) = y_; // center interval always contains the center element
    part_(yPos)(1) = y_;
  }
#ifdef MC_DEBUG
  std::cout << "end partition" << std::endl;
#endif
}

Real BOSNode::yLogProba(const Vector<int, 2>& e) const
{
#ifdef MC_DEBUG
  std::cout << "BOSNode::yProba" << std::endl;
#endif
  Real yProba;
  if (e(0) <= y_ && y_ <= e(1)) // y is among the last segment values
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
  return std::log(yProba); // conditional probability of y, which only depends of the size of the interval
}

Real BOSNode::zLogProba(Real pi) const
{
#ifdef MC_DEBUG
  std::cout << "BOSNode::zProba" << std::endl;
#endif
  Real zProba;
  if (z_ == 1) // comparison is perfect
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
  return std::log(zProba);
}

Real BOSNode::eLogProba(int mu,
                        Real pi) const
{
#ifdef MC_DEBUG
  std::cout << "BOSNode::eProba" << std::endl;
  std::cout << "e_(0): " << e_(0) << ", e_(1): " << e_(1) << std::endl;
#endif
  Real eProba;

  if (z_ == 1) // comparison is perfect, and only the best segment has a nonzero probability
  {
#ifdef MC_DEBUG
    std::cout << "z == 1" << std::endl;
#endif
    int closestSegment = -1; // index in partition of the closest segment
    Real disClosestSegment; // distance between mu and closest segment
    for (int s = 0; s < partSize_; ++s) // loop on all segments of the partition
    {
#ifdef MC_DEBUG
      std::cout << "\ts: " << s << ", part(s)(0): " << part(s)(0) << ", part(s)(1): " << part(s)(1) << std::endl;
#endif
      Real disCurrSegment = std::min(std::abs(mu - part_(s)(0)), // distance of current segment to the mode
                                     std::abs(mu - part_(s)(1)));
      if (s == 0 || disCurrSegment < disClosestSegment) // for the first segment, or if a new closest segment has been detected
      {
        closestSegment = s;
        disClosestSegment = disCurrSegment;
#ifdef MC_DEBUG
        std::cout << "\t\tdisCurrSegment: " << disCurrSegment << std::endl;
        std::cout << "\t\tdisClosestSegment: " << disClosestSegment << std::endl;
#endif
      }
    }

    if (closestSegment > -1 && e_ == closestSegment) // a closest segment exists, and e points to it
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
    for (int s = 0; s < partSize_; ++s) // loop on all segments of the partition
    {
#ifdef MC_DEBUG
      std::cout << "s: " << s << ", part(s)(0): " << part(s)(0) << ", part(s)(1): " << part(s)(1) << std::endl;
#endif
      sizePart += part_(s)(1) - part_(s)(0) + 1;
      if (s == e_) // computation of probability for current e_
      {
#ifdef MC_DEBUG
        std::cout << "part(s) == e, s: " << s << std::endl;
#endif
        eProba = Real(part_(s)(1) - part_(s)(0) + 1);
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
  return std::log(eProba);
}

} // namespace mixt
