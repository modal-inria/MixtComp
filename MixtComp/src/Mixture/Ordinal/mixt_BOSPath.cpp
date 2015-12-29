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

  int lastSeg = c_(nbNode_ - 1).e_(0); // last segment only contains one element
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
        yLogProba = c_(node).yLogProba(c_(node - 1).e_); // yProba based on previous iteration segment
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

void BOSPath::tupleMultinomial(int mu,
                               Real pi,
                               int startIndex,
                               int sizeTuple,
                               std::list<Vector<BOSNode> >& pathList,
                               Vector<Real>& probaVec,
                               const Vector<bool, 2>& az) const
{
#ifdef MC_DEBUG
  std::cout << "BOSPath::tupleMultinomial" << std::endl;
#endif

  std::list<Real> probaList;
  Vector<BOSNode> tuple(sizeTuple); // a specific path is used for this computation, instead of c

  // tupleMultinomial is not recursive, but nodeMultinomial is, and allows easy handling of any size of tuple
  nodeMultinomial(mu,
                  pi,
                  startIndex,
                  startIndex, // currIndex = startIndex for the first recursive call
                  0., // log(1), because proba is 1 at the beginning of first node of the tuple
                  pathList,
                  probaList,
                  tuple,
                  az); // a specific path is used for this computation, instead of c) const

  int nbPath = probaList.size(); // conversion from list of logs to categorical density distribution, similar to eStep conversion

#ifdef MC_DEBUG
  std::cout << "nbPath: " << nbPath << std::endl;
#endif

  Vector<Real> logProba(nbPath);
  std::list<Real>::const_iterator it = probaList.begin();
  for (int i = 0; i < nbPath; ++i, ++it)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << " / " << nbPath << std::endl;
#endif

    logProba(i) = *it;
  }

#ifdef MC_DEBUG
  std::cout << "logProba.size(): " << logProba.size() << std::endl;
#endif

  probaVec.logToMulti(logProba);
}

void BOSPath::nodeMultinomial(int mu,
                              Real pi,
                              int startIndex,
                              int currIndex,
                              Real logProba,
                              std::list<Vector<BOSNode> >& pathList,
                              std::list<Real>& probaList,
                              Vector<BOSNode>& tuple,
                              const Vector<bool, 2>& az) const
{
  int tupleIndex = currIndex - startIndex; // relative index inside the tuple
#ifdef MC_DEBUG
  std::cout << "BOSPath::nodeMultinomial, startIndex: " << startIndex
            << ", currIndex: " << currIndex
            << ", tupleIndex: " << tupleIndex
            << ", tuple.size(): " << tuple.size()
            << ", logProba: " << logProba << std::endl;
#endif

  Vector<int, 2> firstSeg;
  if (tupleIndex == 0) // is this the beginning of the tuple ?
  {
    if (currIndex == 0) // is the tuple at the beginning of the path ?
    {
      firstSeg = eInit_;
    }
    else
    {
      firstSeg = c_(currIndex - 1).e_;
    }
  }
  else
  {
    firstSeg = tuple(tupleIndex - 1).e_;
  }

#ifdef MC_DEBUG
  std::cout << "firstSeg" << std::endl;
  std::cout << firstSeg << std::endl;
#endif

  for (tuple(tupleIndex).y_ = firstSeg(0)    ;
       tuple(tupleIndex).y_ < firstSeg(1) + 1;
       ++tuple(tupleIndex).y_)
  {
    tuple(tupleIndex).partition(firstSeg); // computation of path(0).part_
    Real yLogProba = tuple(tupleIndex).yLogProba(firstSeg);
    for (tuple(tupleIndex).z_ = 0;
         tuple(tupleIndex).z_ < 2;
         ++tuple(tupleIndex).z_)
    {
      Real zLogProba = tuple(tupleIndex).zLogProba(pi);
      for(int e = 0                      ;
          e < tuple(tupleIndex).partSize_;
          ++e)
      {
        tuple(tupleIndex).e_ = tuple(tupleIndex).part_(e); // segment is taken from the partition
        Real eLogProba = tuple(tupleIndex).eLogProba(mu, pi);
        if (eLogProba > minInf) // null probability segments are not computed further
        {
          if (tupleIndex + 1 < tuple.size()) // is the next element a node or an end condition ?
          {
            nodeMultinomial(mu,
                            pi,
                            startIndex,
                            currIndex + 1,
                            logProba + yLogProba + zLogProba + eLogProba,
                            pathList,
                            probaList,
                            tuple,
                            az); // a specific path is used for this computation, instead of c) const
          }
          else // next element is the end condition. pathList and probaList must eventually be filled.
          {
            endMultinomial(mu,
                           pi,
                           startIndex,
                           currIndex + 1,
                           logProba + yLogProba + zLogProba + eLogProba,
                           tuple,
                           pathList,
                           probaList,
                           az);
          }
        }
        else // null probability result in no action
        {
        }
      }
    }
  }
}

void BOSPath::endMultinomial(int mu,
                             Real pi,
                             int startIndex,
                             int currIndex,
                             Real logProba,
                             const Vector<BOSNode>& tuple,
                             std::list<Vector<BOSNode> >& pathList,
                             std::list<Real>& probaList,
                             const Vector<bool, 2>& az) const
{
  Vector<int, 2> lastSegment = tuple(tuple.size() - 1).e_;

  if (az != true) // Shall we check that "if tuple is copied into c_, this will result in a c_ where all z = 0 or z = 1" ?
  {
    int nbZOne = 0;
    for (int n = 0; n < nbNode_; ++n)
    {
      if (startIndex <= n && n < startIndex + tuple.size()) // would this node be overwritten by the tuple ?
      {
        nbZOne += tuple(n - startIndex).z_;
      }
      else
      {
        nbZOne += c_(n).z_;
      }
    }

#ifdef MC_DEBUG
    std::cout << "BOSPath::endMultinomial, az: " << az.transpose() << ", nbZOne: " << nbZOne << std::endl;
#endif

    if ((!az(0) && nbZOne == 0) || (!az(1) && nbZOne == nbNode_)) // all z = 1, therefore the tuple can not be considered a correct candidate
    {
      return; // this completion of the individual does not satisfy the constraints on the latent variable z
    }
  }

  if (currIndex < nbNode_) // is this end condition a node ? In this case, the conditional probabilities must be computed inside that node
  {
#ifdef MC_DEBUG
    std::cout << "not the last node" << std::endl;
#endif
    BOSNode endNode = c_(currIndex); // copy of the ending node
    endNode.partition(lastSegment); // computation of the partition in the ending node, using the resulting segment of the last node of the tuple

    if (endNode.isInPart(endNode.e_)) // is the last node final segment still compatible with its new partition ?
    {
      Real lastNodeLogProba =   endNode.yLogProba(lastSegment) // yLogProba in ending node, based on the last segment
                              + endNode.zLogProba(pi)
                              + endNode.eLogProba(mu, pi); // the proba of the segment with the new partition

      if (lastNodeLogProba > minInf) // has the next node a non zero probability, given the current value of path(1).e_ ?
      {
        pathList.push_back(tuple);
        probaList.push_back(logProba + lastNodeLogProba); // log proba of current path is saved
      }
    }
  }
  else // the second node corresponds to the last node in c, and the compatibility with the data constraint is checked
  {
    if (lastSegment(0) <= endCond_(1) &&
        endCond_(0)    <= lastSegment(1)) // is the final segment compatible with the data constraint ?
    {
#ifdef MC_DEBUG
      std::cout << "last node, data constraint satisfied" << std::endl;
#endif
      pathList.push_back(tuple);
      probaList.push_back(logProba); // proba of current path is saved
    }
  }
}

void BOSPath::initPath()
{
#ifdef MC_DEBUG
  std::cout << "initPath" << std::endl;
  std::cout << "endCond_(0): " << endCond_(0) << ", endCond_(1): " << endCond_(1) << std::endl;
#endif
  Vector<int, 2> currSeg = eInit_;

  for (int i = 0; i < nbNode_; ++i) // loop to fill all the elements of the path
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
#endif
    c_(i).y_ = multi_.sampleInt(currSeg(0), currSeg(1)); // uniform sampling of z
    c_(i).partition(currSeg); // compute the partition, where y cuts into currSeg
    c_(i).z_ = 0; // comparisons are all blind in initialization
    Vector<Real> segProba(c_(i).partSize_); // vector of proba of each segment in the partition
    for (int s = 0; s < c_(i).partSize_; ++s) // computation of the allowed segments
    {
      if (c_(i).part_(s)(0) <= endCond_(1) && // test if the current segment of the partition can reach any allowed point
          endCond_(0)       <= c_(i).part_(s)(1))
      {
        segProba(s) = c_(i).part_(s)(1) - c_(i).part_(s)(0) + 1; // proba to sample segment is proportional to its size
      }
      else
      {
        segProba(s) = 0.; // segments incompatible with the last segment have a zero probability
      }
    }
    segProba /= segProba.sum();
    int sampleSeg = multi_.sample(segProba);
#ifdef MC_DEBUG
    std::cout << "segProba" << std::endl;
    std::cout << segProba << std::endl;
    std::cout << "sampleSeg: " << sampleSeg << std::endl;
#endif
    c_(i).e_ = c_(i).part_(sampleSeg);
    currSeg = c_(i).e_; // initial segment used for next node
#ifdef MC_DEBUG
    std::cout << "currNode" << std::endl;
    BOSDisplaySegNode(c_(i));
#endif
  }
}

void BOSPath::samplePath(int mu,
                         Real pi,
                         int sizeTupleMax,
                         const Vector<bool, 2>& az)
{
  int sizeTuple = std::min(nbNode_, sizeTupleMax);

#ifdef MC_DEBUG
  std::cout << "BOSPath::samplePath, nbNode_: " << nbNode_ << ", sizeTupleMax: " << sizeTupleMax << ", sizeTuple: " << sizeTuple << std::endl;
#endif

  for (int startIndex = 0; startIndex < nbNode_ - sizeTuple + 1; ++startIndex)
  {
#ifdef MC_DEBUG
    std::cout << "node: " << startIndex << " / " << nbNode_ - sizeTuple + 1 << std::endl;
#endif

    // computation of the possible node values and associated probabilities
    std::list<Vector<BOSNode> > pathList;
    Vector<Real> probaVec;

    tupleMultinomial(mu,
                     pi,
                     startIndex,
                     sizeTuple,
                     pathList,
                     probaVec,
                     az);

    // sampling and replacement in the path
#ifdef MC_DEBUG
    std::cout << "probaVec.size(): " << probaVec.size() << std::endl;
    std::cout << "pathList.size(): " << pathList.size() << std::endl;
    std::cout << "probaVec: " << probaVec << std::endl;
    std::cout << "pathList" << std::endl;
    for(std::list<Vector<BOSNode, 2> >::iterator it = pathList.begin();
        it != pathList.end();
        ++it)
    {
      for (int currNode = 0; currNode < sizeTuple; ++currNode) // copy sampled tuple inside the path
      {
        std::cout << "displaySegNode: " << currNode << std::endl;
        BOSDisplaySegNode((*it)(currNode));
      }
    }
#endif

    int pathSampled = multi_.sample(probaVec); // sample one the path provided by tupleMultinomial
    std::list<Vector<BOSNode> >::const_iterator it = pathList.begin();
    for(int path = 0; path < pathSampled; ++path) // fast-forward to the sampled sub-path
    {
      ++it;
    }
    for (int currNode = 0; currNode < sizeTuple; ++currNode) // copy sampled tuple inside the path
    {
      c_[startIndex + currNode] = (*it)(currNode);
    }

#ifdef MC_DEBUG
    std::cout << "end of node iteration inside samplePath, displayPath:" << std::endl;
    BOSDisplayPath(*this);
#endif

  }
}

void BOSPath::forwardSamplePath(int mu,
                                Real pi,
                                const Vector<bool, 2>& az)
{
#ifdef MC_DEBUG
    std::cout << "BOSPath::forwardSamplePath, mu: " << mu << ", pi: " << pi
              << ", az: " << itString(az) << ", nbNode_: " << nbNode_ << std::endl;
#endif

  Vector<int, 2> seg = eInit_;
  Vector<Real> currProba;

  while (true) // pi = 1 results in an infinite loop, but this is not an admissible value for pi (and should have been rejected earlier in any code)
  {
#ifdef MC_DEBUG
    std::cout << "BOSPath::forwardSamplePath, nbSample: " << nbSample << std::endl;
#endif

    for (int n = 0; n < nbNode_; ++n)
    {
      BOSNode& currNode = c_(n);

      currNode.y_ = multi_.sampleInt(seg(0), seg(1));
      currNode.partition(seg);

      currNode.z_ = multi_.sampleBinomial(pi);

      currProba.resize(currNode.partSize_);
      for (int e = 0; e < currNode.partSize_; ++e)
      {
        currNode.e_ = currNode.part_(e);
        currProba(e) = std::exp(currNode.eLogProba(mu, pi));
      }
      currNode.e_ = currNode.part_(multi_.sample(currProba));

      seg = currNode.e_;
    }

    if (!((az(0) && nbZ() == 0) || (az(1) && nbZ() == nbNode_)))
    {
      return;
    }
  }
}

int BOSPath::nbZ() const
{
  int nz = 0;
  for (int node = 0; node < nbNode_; ++node)
  {
    nz += c_(node).z_;
  }
  return nz;
}

bool BOSPath::allZ0() const
{
  return nbZ() == 0;
}

bool BOSPath::allZ1() const
{
  return nbZ() == nbNode_;
}

void BOSDisplaySegNode(const BOSNode& node)
{
  std::cout << "\ty: " << node.y_ << std::endl;
  std::cout << "\tpart: " << std::endl;
  for (int s = 0; s < node.partSize_; ++s)
  {
    std::cout << "\t\tpart(" << s << ")(0): "  << node.part_(s)(0)
              << ", part(" << s << ")(1): "   << node.part_(s)(1) << std::endl;
  }
  std::cout << "\tz: " << node.z_ << std::endl;
  std::cout << "\tnode.e_(0): " << node.e_(0) << ", node.e_(1): " << node.e_(1) << std::endl;
}

void BOSDisplayPath(const BOSPath& path)
{
  std::cout << "path.eInit_(0): " << path.eInit_(0) << ", path.eInit_(1): " << path.eInit_(1) << std::endl;
  for (int node = 0; node < path.c_.size(); ++node)
  {
    std::cout << "node: " << node << std::endl;
    BOSDisplaySegNode(path.c_(node));
  }
}

} // namespace mixt
