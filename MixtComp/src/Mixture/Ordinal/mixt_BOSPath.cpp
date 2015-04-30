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

#include "mixt_BOSPath.h"

#include "../../Various/mixt_Constants.h"

namespace mixt
{

void BOSPath::setInit(int a, int b)
{
  eInit_(0) = a;
  eInit_(1) = b;
  nbSegment_ = eInit_(1) - eInit_(0); // number of segments in the path};
  c_.resize(nbSegment_);
}

void BOSPath::setEnd(int a, int b)
{
  endCond_(0) = a;
  endCond_(1) = b;
};

Real BOSPath::computeLogProba(int mu,
                              Real pi) const
{
#ifdef MC_DEBUG
  std::cout << "BOSPath::computeProba" << std::endl;
#endif
  Real logProba = 0.; // The initial probability of being in any of the member of the input interval is 1

  int lastE = c_(nbSegment_ - 1).e_;
  int lastSeg = c_(nbSegment_ - 1).part_(lastE)(0); // last segment only contains one element
  if (endCond_(0) <= lastSeg && lastSeg <= endCond_(1)) // is the path compatible with the provided condition ?
  {
    for (int node = 0; node < nbSegment_; ++node) // loop over each BOSNode in c
    {
#ifdef MC_DEBUG
      std::cout << "node: " << node << std::endl;
#endif
      Real yLogProba;
      if (node == 0)
      {
        yLogProba = c_(node).yLogProba(eInit_); // yProba based on initial segment
      }
      else
      {
        yLogProba = c_(node).yLogProba(c_(node - 1).part_(c_(node - 1).e_)); // yProba based on previous iteration segment
      }
      Real zLogProba = c_(node).zLogProba(pi);
      Real eLogProba = c_(node).eLogProba(mu,
                                 pi);
#ifdef MC_DEBUG
      std::cout << "yLogProba: " << yLogProba << ", zLogProba: " << zLogProba << ", eLogProba: " << eLogProba << std::endl;
#endif
      logProba += yLogProba + zLogProba + eLogProba;
    }
  }
  else // conditional probability of verifying condition given the path is null
      logProba = minInf;

  return logProba;
}

void BOSPath::nodeMultinomial(int mu,
                              Real pi,
                              int index,
                              std::list<Vector<BOSNode, 2> >& pathList,
                              std::list<Real>& probaList) const
{
  Vector<BOSNode, 2> path; // a specific path is used for this computation, instead of c
  Vector<int, 2> firstSeg;
  if (index == 0)
    firstSeg = eInit_;
  else
    firstSeg = c_(index - 1).part_(c_(index - 1).e_);

  for (path(0).y_ = firstSeg(0)    ;
       path(0).y_ < firstSeg(1) + 1;
       ++path(0).y_) // outer node
  {
    path(0).partition(firstSeg);
    Real y0LogProba = path(0).yLogProba(firstSeg);
#ifdef MC_DEBUG
    std::cout << "y0: " << y0 << std::endl;
    std::cout << "\ty0LogProba: " << y0LogProba << std::endl;
#endif
    for (path(0).z_ = 0;
         path(0).z_ < 2;
         ++path(0).z_)
    {
      Real z0LogProba = path(0).zLogProba(pi);
#ifdef MC_DEBUG
      std::cout << "y0: " << path(0).y_ << ", z0: " << path(0).z_ << std::endl;
      std::cout << "\tz0LogProba: " << z0LogProba << std::endl;
#endif
      for(path(0).e_ = 0                ;
          path(0).e_ < path(0).partSize_;
          ++path(0).e_)
      {
        Real e0LogProba = path(0).eLogProba(mu, pi);
#ifdef MC_DEBUG
        std::cout << "y0: " << path(0).y_ << ", z0: " << path(0).z_ << ", e0: " << path(0).e_ << ", part0(e0)(0): " << path(0).part_(path(0).e_)(0) << ", part0(e0)(1): " << path(0).part_(path(0).e_)(1) << std::endl;
        std::cout << "\te0LogProba: " << e0LogProba << std::endl;
#endif
        if (e0LogProba > minInf) // null probability segments are not computed further
        {
          Vector<int, 2> currSeg = path(0).part_(path(0).e_);
          for(path(1).y_ = currSeg(0)    ;
              path(1).y_ < currSeg(1) + 1;
              ++path(1).y_) // inner node
          {
            path(1).partition(currSeg);
            Real y1LogProba = path(1).yLogProba(currSeg);
#ifdef MC_DEBUG
            std::cout << "y0: " << path(0).y_ << ", z0: " << path(0).z_ << ", e0: " << path(0).e_ << ", part0(e0)(0): " << path(0).part_(path(0).e_)(0) << ", part0(e0)(1): " << path(0).part_(path(0).e_)(1) << std::endl;
            std::cout << "y1: " << path(1).y_ << std::endl;
            std::cout << "\ty1LogProba: " << y1LogProba << std::endl;
#endif
            for (path(1).z_ = 0;
                 path(1).z_ < 2;
                 ++path(1).z_)
            {
              Real z1LogProba = path(1).zLogProba(pi);
#ifdef MC_DEBUG
              std::cout << "y0: " << path(0).y_ << ", z0: " << path(0).z_ << ", e0: " << path(0).e_ << ", part0(e0)(0): " << path(0).part_(path(0).e_)(0) << ", part0(e0)(1): " << path(0).part_(path(0).e_)(1) << std::endl;
              std::cout << "y1: " << path(1).y_ << ", z0: " << path(1).z_  << std::endl;
              std::cout << "\tz1LogProba: " << z1LogProba << std::endl;
#endif
              for(path(1).e_ = 0;
                  path(1).e_ < path(1).partSize_;
                  ++path(1).e_)
              {
                Real e1LogProba = path(1).eLogProba(mu, pi);
                Vector<int, 2> endSeg = path(1).part_(path(1).e_);
#ifdef MC_DEBUG
                std::cout << "y0: " << path(0).y_ << ", z0: " << path(0).z_ << ", e0: " << path(0).e_ << ", part0(e0)(0): " << path(0).part_(path(0).e_)(0) << ", part0(e0)(1): " << path(0).part_(path(0).e_)(1) << std::endl;
                std::cout << "y1: " << path(1).y_ << ", z0: " << path(1).z_ << ", e0: " << path(1).e_ << ", part0(e0)(0): " << path(1).part_(path(1).e_)(0) << ", part0(e0)(1): " << path(1).part_(path(1).e_)(1) << std::endl;
                std::cout << "\te1LogProba: " << e1LogProba << std::endl;
#endif
                if (e1LogProba > minInf && endSeg(0) <= endCond_(1) && endSeg(1) <= endCond_(0)) // is the final condition verified ?
                {
                  Real logProba = y0LogProba + z0LogProba + e0LogProba + y1LogProba + z1LogProba + e1LogProba;
                  pathList.push_back(path);
                  probaList.push_back(logProba); // proba of current path is saved
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

// format of interval e is a vector with bounds included: [1, 3] corresponds to the set {1, 2, 3}

//void initPath(const Vector<int, 2>& initSeg,
//              const Vector<int, 2>& endCond,
//              MultinomialStatistic& multi,
//              Vector<BOSNode>& c)
//{
//#ifdef MC_DEBUG
//  std::cout << "initPath" << std::endl;
//#endif
//  int nbSegment = initSeg(1) - initSeg(0); // number of segments in the path
//
//  c.resize(nbSegment);
//
//#ifdef MC_DEBUG
//  std::cout << "endCond(0): " << endCond(0) << ", endCond(1): " << endCond(1) << std::endl;
//#endif
//  Vector<int, 2> seg = initSeg;
//
//  for (int i = 0; i < nbSegment; ++i) // loop to fill all the elements of the path
//  {
//#ifdef MC_DEBUG
//    std::cout << "i: " << i << std::endl;
//#endif
//    c(i).y_ = multi.sampleInt(seg(0), seg(1));
//    partition(seg,
//              c(i).y_,
//              c(i).part_);
//    c(i).z_ = 0; // comparisons are all blind in initialization
//    Vector<Real> segProba(3);
//    for (int s = 0; s < 3; ++s) // computation of the allowed segments
//    {
//      if ( c(i).part_(s)(0) > -1       && // only non-empty segments are considered
//           c(i).part_(s)(0) <= endCond(1) && // test if the current segment of the partition can reach any allowed point
//           endCond(0)       <= c(i).part_(s)(1) )
//        segProba(s) = c(i).part_(s)(1) - c(i).part_(s)(0) + 1; // proba to sample segment is proportional
//      else
//        segProba(s) = 0.;
//    }
//    segProba /= segProba.sum();
//    int sampleSeg = multi.sample(segProba);
//#ifdef MC_DEBUG
//    std::cout << "segProba" << std::endl;
//    std::cout << segProba << std::endl;
//    std::cout << "sampleSeg: " << sampleSeg << std::endl;
//#endif
//    c(i).e_ = c(i).part_(sampleSeg);
//    seg = c(i).e_;
//#ifdef MC_DEBUG
//    std::cout << "currNode" << std::endl;
//    displaySegNode(c(i));
//#endif
//  }
//}

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
  std::cout << "\tnode.part_(node.e_)(0): " << node.part_(node.e_)(0) << ", node.part_(node.e_)(1): " << node.part_(node.e_)(1) << std::endl;
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

} // namespace mixt
