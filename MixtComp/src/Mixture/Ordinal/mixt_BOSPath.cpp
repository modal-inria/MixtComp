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
  nbNode_ = eInit_(1) - eInit_(0); // number of segments in the path;
  c_.resize(nbNode_);
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

  int lastE = c_(nbNode_ - 1).e_;
  int lastSeg = c_(nbNode_ - 1).part_(lastE)(0); // last segment only contains one element
  if (endCond_(0) <= lastSeg && lastSeg <= endCond_(1)) // is the path compatible with the provided condition ?
  {
    for (int node = 0; node < nbNode_; ++node) // loop over each BOSNode in c
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
                              Vector<Real>& probaVec) const
{
#ifdef MC_DBUG_NEW
  std::cout << "BOSPath::nodeMultinomial" << std::endl;
#endif
  std::list<Real> probaList;
  Vector<BOSNode, 2> path; // a specific path is used for this computation, instead of c
  Vector<int, 2> firstSeg;
  Vector<int, 2> lastSeg;
  if (index == 0)
    firstSeg = eInit_;
  else
    firstSeg = c_(index - 1).part_(c_(index - 1).e_);

  if (index == c_.size() - 1)
    lastSeg = endCond_;
  else
    lastSeg = c_(index + 1).part_(c_(index + 1).e_);

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
                if (e1LogProba > minInf && endSeg(0) <= lastSeg(1) && endSeg(1) <= lastSeg(0)) // is the final condition verified ?
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

  // conversion from list of logs to categorical density distribution, similar to eStep conversion
  int nbPath = probaList.size();
  probaVec.resize(nbPath);
  std::list<Real>::const_iterator it = probaList.begin();
  for (int i = 0; i < nbPath; ++i, ++it)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << " / " << nbPath << std::endl;
#endif
    probaVec(i) = *it;
  }

  Real max = probaVec.maxCoeff();
  probaVec -= max;
  probaVec = probaVec.exp();
  Real sum = probaVec.sum();
  probaVec /= sum;
}

void BOSPath::initPath()
{
#ifdef MC_DEBUG
  std::cout << "initPath" << std::endl;
  std::cout << "endCond_(0): " << endCond_(0) << ", endCond_(1): " << endCond_(1) << std::endl;
#endif
  Vector<int, 2> seg = eInit_;

  for (int i = 0; i < nbNode_; ++i) // loop to fill all the elements of the path
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
#endif
    c_(i).y_ = multi_.sampleInt(seg(0), seg(1));
    c_(i).partition(seg);
    c_(i).z_ = 0; // comparisons are all blind in initialization
    Vector<Real> segProba(c_(i).partSize_);
    for (int s = 0; s < c_(i).partSize_; ++s) // computation of the allowed segments
    {
      if (c_(i).part_(s)(0) <= endCond_(1) && // test if the current segment of the partition can reach any allowed point
          endCond_(0)       <= c_(i).part_(s)(1) )
        segProba(s) = c_(i).part_(s)(1) - c_(i).part_(s)(0) + 1; // proba to sample segment is proportional
      else
        segProba(s) = 0.;
    }
    segProba /= segProba.sum();
    int sampleSeg = multi_.sample(segProba);
#ifdef MC_DEBUG
    std::cout << "segProba" << std::endl;
    std::cout << segProba << std::endl;
    std::cout << "sampleSeg: " << sampleSeg << std::endl;
#endif
    c_(i).e_ = sampleSeg;
    seg = c_(i).part_(sampleSeg);
#ifdef MC_DEBUG
    std::cout << "currNode" << std::endl;
    displaySegNode(c_(i));
#endif
  }
}

const Vector<int, 2>& BOSPath::seg(int i) const
{
#ifdef MC_DEBUG
  std::cout << "BOSPath::seg" << std::endl;
  std::cout << "c_(i).e_: " << c_(i).e_ << ", c_(i).partSize_: " << c_(i).partSize_ << std::endl;
#endif
  return c_(i).part_(c_(i).e_);
}

void BOSPath::samplePath(int mu,
                         Real pi)
{
#ifdef MC_DEBUG
  std::cout << "BOSPath::samplePath" << std::endl;
  std::cout << "displayPath" << std::endl;
  displayPath(*this);
#endif
  for (int node = 0; node < nbNode_ - 1; ++node)
  {
#ifdef MC_DEBUG
    std::cout << "node: " << node << " / " << nbNode_ - 2 << std::endl;
#endif
    // computation of the possible node values and associated probabilities
    std::list<Vector<BOSNode, 2> > pathList;
    Vector<Real> probaVec;
    nodeMultinomial(mu,
                    pi,
                    node,
                    pathList,
                    probaVec);

    // sampling and replacement in the path
    int pathSampled = multi_.sample(probaVec);
    std::list<Vector<BOSNode, 2> >::iterator it = pathList.begin();
    for(int path = 0; path < pathSampled; ++path)
    {
      ++it; // fast-forward to the sampled sub-path
    }
    c_[node    ] = (*it)(0);
    c_[node + 1] = (*it)(1);
#ifdef MC_DEBUG
    std::cout << "displayPath" << std::endl;
    displayPath(*this);
#endif
  }
}

void displaySegNode(const BOSNode& node)
{
  std::cout << "\ty: " << node.y_ << std::endl;
  std::cout << "\tpart: " << std::endl;
  for (int s = 0; s < node.partSize_; ++s)
  {
    std::cout << "\t\tpart(" << s << ")(0): "  << node.part_(s)(0)
              << ", part(" << s << ")(1): "   << node.part_(s)(1) << std::endl;
  }
  std::cout << "\tz: " << node.z_ << std::endl;
  std::cout << "\tnode.part_(node.e_)(0): " << node.part_(node.e_)(0) << ", node.part_(node.e_)(1): " << node.part_(node.e_)(1) << std::endl;
}

void displayPath(const BOSPath& path)
{
  std::cout << "path.eInit_(0): " << path.eInit_(0) << ", path.eInit_(1): " << path.eInit_(1) << std::endl;
  for (int node = 0; node < path.c_.size(); ++node)
  {
    std::cout << "node: " << node << std::endl;
    displaySegNode(path.c_(node));
  }
}

} // namespace mixt
