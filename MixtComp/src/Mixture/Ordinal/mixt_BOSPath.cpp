/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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

void BOSPath::setC(const Vector<BOSNode>& c) {
  c_ = c;
}

Real BOSPath::computeLogProba(int mu,
                              Real pi) const {
  Real logProba = 0.; // The initial probability of being in any of the member of the input interval is 1

  int lastSeg = c_(nbNode_ - 1).e_(0); // last segment only contains one element
  if (endCond_(0) <= lastSeg && lastSeg <= endCond_(1)) { // is the path compatible with the provided condition ?
    for (int node = 0; node < nbNode_; ++node) { // loop over each BOSNode in c
      Real yLogProba;
      if (node == 0) {
        yLogProba = c_(node).yLogProba(eInit_); // yProba based on initial segment
      }
      else {
        yLogProba = c_(node).yLogProba(c_(node - 1).e_); // yProba based on previous iteration segment
      }
      Real zLogProba = c_(node).zLogProba(pi);
      Real eLogProba = c_(node).eLogProba(mu,
                                 pi);
      logProba += yLogProba + zLogProba + eLogProba;
    }
  }
  else { // conditional probability of verifying condition given the path is null
      logProba = minInf;
  }

  return logProba;
}

void BOSPath::tupleMultinomial(int mu,
                               Real pi,
                               int startIndex,
                               int sizeTuple,
                               std::list<Vector<BOSNode> >& pathList,
                               Vector<Real>& probaVec,
                               zCondition zCond) const
{
  std::list<Real> probaList;
  Vector<BOSNode> tuple(sizeTuple); // a specific path is used for this computation, instead of c

  // tupleMultinomial is not recursive, but nodeMultinomial is, and allows easy handling of any size of tuple
  nodeMultinomial(mu,
                  pi,
                  startIndex,
                  startIndex, // currIndex = startIndex for the first recursive call
                  0., // log(1), because proba is 1 at the beginning of first node of the tuple
                  pathList, // paths will be appended to pathList, so that the function calling tupleMultinomial will be able to sample one using the corresponding probaList
                  probaList,
                  tuple,
                  zCond); // a specific path is used for this computation, instead of c) const

  int nbPath = probaList.size(); // conversion from list of logs to categorical density distribution, similar to eStep conversion

  Vector<Real> logProba(nbPath);
  std::list<Real>::const_iterator it = probaList.begin();
  for (int i = 0; i < nbPath; ++i, ++it)
  {

    logProba(i) = *it;
  }

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
                              zCondition zCond) const
{
  int tupleIndex = currIndex - startIndex; // relative index inside the tuple

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
                            zCond); // a specific path is used for this computation, instead of c) const
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
                           zCond);
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
                             zCondition zCond) const
{
  Vector<int, 2> lastSegment = tuple(tuple.size() - 1).e_;

  if (zCond != allZAuthorized_) // Shall we check that "if tuple is copied into c_, this will result in a c_ where all z = 0 or z = 1" ?
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

    if ((zCond == allZ0Forbidden_ && nbZOne == 0) || (zCond == allZ1Forbidden_ && nbZOne == nbNode_)) // all z = 1, therefore the tuple can not be considered a correct candidate
    {
      return; // this completion of the individual does not satisfy the constraints on the latent variable z
    }
  }

  if (currIndex < nbNode_) // is this end condition a node ? In this case, the conditional probabilities must be computed inside that node
  {
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
  Vector<int, 2> currSeg = eInit_;

  for (int i = 0; i < nbNode_; ++i) // loop to fill all the elements of the path
  {
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

    c_(i).e_ = c_(i).part_(sampleSeg);
    currSeg = c_(i).e_; // initial segment used for next node
  }

  computeNbZ();
}

void BOSPath::samplePath(int mu,
                         Real pi,
                         int sizeTupleMax,
                         zCondition zCond)
{
  int sizeTuple = std::min(nbNode_, sizeTupleMax);

  for (int startIndex = 0; startIndex < nbNode_ - sizeTuple + 1; ++startIndex)
  {
    // computation of the possible node values and associated probabilities
    std::list<Vector<BOSNode> > pathList;
    Vector<Real> probaVec;

    tupleMultinomial(mu,
                     pi,
                     startIndex,
                     sizeTuple,
                     pathList,
                     probaVec,
                     zCond);

    // sampling and replacement in the path

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
  }

  computeNbZ();
}

void BOSPath::forwardSamplePath(int mu,
                                Real pi,
                                zCondition zCond) {
  Vector<int, 2> seg = eInit_;
  Vector<Real> currProba;

  while (true) { // pi = 1 results in an infinite loop, but this is not an admissible value for pi (and should have been rejected earlier in any code)
    for (int n = 0; n < nbNode_; ++n) {
      BOSNode& currNode = c_(n);

      currNode.y_ = multi_.sampleInt(seg(0), seg(1));
      currNode.partition(seg);

      currNode.z_ = multi_.sampleBinomial(pi);

      currProba.resize(currNode.partSize_);
      for (int e = 0; e < currNode.partSize_; ++e) {
        currNode.e_ = currNode.part_(e);
        currProba(e) = std::exp(currNode.eLogProba(mu, pi));
      }
      currNode.e_ = currNode.part_(multi_.sample(currProba));

      seg = currNode.e_;
    }

    computeNbZ();

    switch (zCond) {
      case allZAuthorized_: {
        return;
      }
      break;

      case allZ0Forbidden_: {
        if (nbZ() != 0) {
          return;
        }
      }
      break;

      case allZ1Forbidden_: {
        if (nbZ() != nbNode_) {
          return;
        }
      }
      break;
    }
  }
}

void BOSPath::computeNbZ() {
  nbZ_ = 0;
  for (int node = 0; node < nbNode_; ++node) {
    nbZ_ += c_(node).z_;
  }

  if (nbZ_ == 0) {
    allZ_ = allZ0_;
  }
  else if (nbZ_ == nbNode_) {
    allZ_ = allZ1_;
  }
  else {
    allZ_ = mixZ0Z1_;
  }
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
  std::cout << "path.eInit()(0): " << path.eInit()(0) << ", path.eInit()(1): " << path.eInit()(1) << std::endl;
  for (int node = 0; node < path.c().size(); ++node)
  {
    std::cout << "node: " << node << std::endl;
    BOSDisplaySegNode(path.c()(node));
  }
}

} // namespace mixt
