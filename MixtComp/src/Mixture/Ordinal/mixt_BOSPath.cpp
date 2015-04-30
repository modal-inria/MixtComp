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
                              Real pi)
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
      logProba = std::log(0);

  return logProba;
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

void nodeMultinomial(const Vector<int, 2>& eInit,
                     const Vector<int, 2>& endCond,
                     int mu,
                     Real pi,
                     std::list<Vector<BOSNode, 2> >& pathList,
                     std::list<Real>& probaList)
{
//  for (int y0 = eInit(0); y0 < eInit(1) + 1; ++y0) // outer node
//  {
//    Vector<Vector<int, 2>, 3> part0; // partition corresponding to y0 value
//    partition(eInit,
//              y0,
//              part0);
//    Real y0proba = yProba(eInit, y0);
//#ifdef MC_DEBUG
//    std::cout << "y0: " << y0 << std::endl;
//    std::cout << "\ty0proba: " << y0proba << std::endl;
//#endif
//    for (int z0 = 0; z0 < 2; ++z0)
//    {
//      Real z0proba = y0proba * zProba(z0, pi);
//#ifdef MC_DEBUG
//      std::cout << "y0: " << y0 << ", z0: " << z0 << std::endl;
//      std::cout << "\tz0proba: " << z0proba << std::endl;
//#endif
//      for(int e0 = 0; e0 < 3; ++e0)
//      {
//        Real e0proba = z0proba * eProba(z0,
//                                        part0,
//                                        part0(e0),
//                                        mu,
//                                        pi);
//#ifdef MC_DEBUG
//        std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
//        std::cout << "\te0proba: " << e0proba << std::endl;
//#endif
//        if (e0proba > epsilon) // null probability segments are not computed further
//        {
//          for(int y1 = part0(e0)(0); y1 < part0(e0)(0) + 1; ++y1) // inner node
//          {
//            Vector<Vector<int, 2>, 3> part1; // partition corresponding to y1 value
//            partition(part0(e0),
//                      y1,
//                      part1);
//            Real y1proba = e0proba * yProba(part0(e0), y1);
//#ifdef MC_DEBUG
//            std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
//            std::cout << "y1: " << y1 << std::endl;
//            std::cout << "\ty1proba: " << y1proba << std::endl;
//#endif
//            for (int z1 = 0; z1 < 2; ++z1)
//            {
//              Real z1proba = y1proba * zProba(z1, pi);
//#ifdef MC_DEBUG
//              std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
//              std::cout << "y1: " << y1 << ", z1: " << z1 << std::endl;
//              std::cout << "\tz1proba: " << z1proba << std::endl;
//#endif
//              for(int e1 = 0; e1 < 3; ++e1)
//              {
//                Real e1proba = z1proba * eProba(z1,
//                                                part1,
//                                                part1(e1),
//                                                mu,
//                                                pi);
//#ifdef MC_DEBUG
//                std::cout << "y0: " << y0 << ", z0: " << z0 << ", e0: " << e0 << ", part0(e0)(0): " << part0(e0)(0) << ", part0(e0)(1): " << part0(e0)(1) << std::endl;
//                std::cout << "y1: " << y1 << ", z1: " << z1 << ", e1: " << e1 << ", part1(e1)(0): " << part1(e1)(0) << ", part1(e1)(1): " << part1(e1)(1) << std::endl;
//                std::cout << "\te1proba: " << e1proba << std::endl;
//#endif
//                if (e1proba > epsilon && part1(e1)(0) <= endCond(0) && part1(e1)(1) <= endCond(1)) // is the final condition verified ?
//                {
//                    pathList.push_back(Vector<BOSNode, 2>());
//                    pathList.back()(0).y_ = y0;
//                    pathList.back()(0).part_ = part0;
//                    pathList.back()(0).z_ = z0;
//                    pathList.back()(0).e_ = part0(e0);
//                    pathList.back()(1).y_ = y1;
//                    pathList.back()(1).part_ = part1;
//                    pathList.back()(1).z_ = z1;
//                    pathList.back()(1).e_ = part1(e1);
//                    probaList.push_back(e1proba); // proba of current path is saved
//                }
//                else
//                {
//#ifdef MC_DEBUG
//                    std::cout << "e1, null proba case or final condition not verified" << std::endl;
//#endif
//                }
//              }
//            }
//          }
//        }
//        else
//        {
//#ifdef MC_DEBUG
//          std::cout << "e0, null proba case detected" << std::endl;
//#endif
//        }
//      }
//    }
//  }
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
