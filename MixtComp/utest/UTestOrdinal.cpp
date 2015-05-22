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
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

#include "../src/Various/mixt_Constants.h"
#include "../src/Mixture/Ordinal/mixt_BOSNode.h"
#include "../src/Mixture/Ordinal/mixt_BOSPath.h"
#include "../src/Statistic/mixt_MultinomialStatistic.h"
#include "../src/Statistic/mixt_UniformStatistic.h"

using namespace mixt;

/**
 * Computation of a partition
 */
TEST(Ordinal, partition0)
{
  Vector<int, 2> e;
  e << 0, 5;
  BOSNode node;
  node.y_ = 2;
  Vector<Vector<int, 2> > partExpected(3);
  partExpected(0) << 0, 1;
  partExpected(1) << 2, 2;
  partExpected(2) << 3, 5;

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Computation of a partition
 */
TEST(Ordinal, partition1)
{
  Vector<int, 2> e;
  e << 6, 8;
  BOSNode node;
  node.y_ = 2;
  Vector<Vector<int, 2> > partExpected(0);

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Computation of a partition
 */
TEST(Ordinal, partition2)
{
  Vector<int, 2> e;
  e << 6, 8;
  BOSNode node;
  node.y_ = 6;
  Vector<Vector<int, 2> > partExpected(2);
  partExpected(0) << 6, 6;
  partExpected(1) << 7, 8;

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Computation of a partition
 */
TEST(Ordinal, partition3)
{
  Vector<int, 2> e;
  e << 6, 8;
  BOSNode node;
  node.y_ = 8;
  Vector<Vector<int, 2> > partExpected(2);
  partExpected(0) << 6, 7;
  partExpected(1) << 8, 8;

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Simple case with two modalities
 */
TEST(Ordinal, computeLogProba0)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision

  BOSPath path;
  path.setInit(0, 1);
  path.setEnd(0, 1);

  path.c_(0).y_ = 1; // second element y picked, proba 0.5
  path.c_(0).z_ = 1; // comparison is perfect, proba 0.5
  path.c_(0).partition(path.eInit_); // computation of the partition
  path.c_(0).e_ << 1, 1; // segment is {1}, proba 1.

  Real expectedProba = std::log(0.5 * 0.5 * 1.);
  Real computedProba = path.computeLogProba(mu,
                                 pi);

  ASSERT_LT(std::abs(expectedProba - computedProba), epsilon);
}

/**
 * Simple case with three modalities and imprecision
 */
TEST(Ordinal, computeLogProba1)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision

  BOSPath path;
  path.setInit(0, 2);
  path.setEnd(0, 0);

  path.c_(0).y_ = 1; // y, middle element y picked, proba 1./3.
  path.c_(0).z_ = 0; // z, comparison is imperfect, proba 0.5
  path.c_(0).partition(path.eInit_); // computation of the partition
  path.c_(0).e_ << 0, 0; // e, left segment {0, 0} selected, proba 0.33 (all have the same size)

  path.c_(1).y_ = 0; // y, only one element to choose from, proba 1.
  path.c_(1).z_ = 1; // z, comparison is perfect, proba 0.5
  path.c_(1).partition(path.c_(0).e_); // computation of the partition
  path.c_(1).e_ << 0, 0; // e, only one segment in partition, with proba 1.

  Real expectedProba = std::log(1./3. * 0.5 * 1./3. *
                                1.    * 0.5 * 1.);
  Real computedProba = path.computeLogProba(mu, pi);

#ifdef MC_DEBUG
  std::cout << "expectedProba: " << expectedProba << std::endl;
  std::cout << "proba: " << computedProba << std::endl;
#endif
  ASSERT_LT(std::abs(expectedProba - computedProba), epsilon);
}

//TEST(Ordinal, tupleMultinomial)
//{
//  int mu = 0;
//  Real pi = 0.5;
//
//  BOSPath path;
//  path.setInit(0, 2);
//  path.setEnd(0, 2);
//
//  int index = 0; // index of the first node of the pair used in the computation
//
//  std::list<Vector<BOSNode> > pathList;
//  Vector<Real> probaVec;
//  path.tupleMultinomial(mu,
//                        pi,
//                        index,
//                        sizeTupleConst,
//                        pathList,
//                        probaVec);
//#ifdef MC_DEBUG
//  std::cout << probaVec << std::endl;
//#endif
//  ASSERT_EQ(pathList.size(), 32); // is the size of pathList correct ?
//}

/**
 * Test checking the initialization of the Gibbs sampling
 */
TEST(Ordinal, GibbsInit)
{
  BOSPath path;
  path.setInit(5, 12);
  path.setEnd(8, 10);

  path.initPath();
#ifdef MC_DEBUG
  std::cout << "path.c_.size(): " << path.c_.size() << std::endl;
  std::cout << "path.nbNode_: " << path.nbNode_ << std::endl;
#endif
  bool validPath;
  if (path.endCond_(0) <= path.c_(path.nbNode_ - 1).e_(0) && path.c_(path.nbNode_ - 1).e_(0) <= path.endCond_(1))
    validPath = true;
  else
    validPath = false;

#ifdef MC_DEBUG
  displayPath(path);
#endif

  int mu = 8; // arbitrary mode, just to verify the validity of the path
  int pi = 0.5;

  Real logProba = path.computeLogProba(mu, pi);

#ifdef MC_DEBUG
  std::cout << "logProba: " << logProba << std::endl;
#endif

  ASSERT_EQ(validPath, true); // is the path generated valid ?
  ASSERT_GT(logProba,
            minInf); // is his proba non zero ?
}

/**
 * test the probability distribution generated by arbitrary BOS parameters.
 */
TEST(Ordinal, ArbitraryGibbs)
{
  MultinomialStatistic multi;
  UniformStatistic uni;
  int nbIter = 1000;

  int iniMin = 0;
  int iniMax = 9;

//  int minCond = multi.sampleInt(0      , initSeg(1));
//  int maxCond = multi.sampleInt(minCond, initSeg(1));
  int endMin = iniMin;
  int endMax = iniMax;

  BOSPath path;
  path.setInit(iniMin, iniMax);
  path.setEnd (endMin, endMax);

  path.initPath();
#ifdef MC_DEBUG
  std::cout << "initialization, displayPath(path)" << std::endl;
  displayPath(path);
#endif

  Vector<Real> computedProba(iniMax - iniMin + 1);
  computedProba = 0.;
  int computedMode;

//  int mu = multi.sampleInt(iniMin, iniMax);
//  Real pi = uni.sample(0., 1.);
  int mu = 2;
  Real pi = 0.5;
  int sizeTuple = 4;

  int expectedMode = std::max(mu, endMin);
  expectedMode = std::min(expectedMode, endMax);

#ifdef MC_DEBUG
  std::cout << "iniMin: " << iniMin << ", iniMax: " << iniMax << std::endl;
  std::cout << "endMin: " << endMin << ", endMax: " << endMax << std::endl;
  std::cout << "mu: " << mu << ", pi: " << pi << std::endl;
  std::cout << "expectedMode: " << expectedMode << std::endl;
#endif

#ifdef MC_DEBUG
  std::cout << "Initial path, displayPath: " << std::endl;
  displayPath(path);
#endif

  for (int iter = 0; iter < nbIter; ++iter)
  {
#ifdef MC_DEBUG
    std::cout << "ArbitraryGibbs, iter: " << iter << std::endl;
#endif
    path.samplePath(mu,
                    pi,
                    sizeTuple);
    int x = path.c_(path.nbNode_-1).e_(0); // x is sampled here
    computedProba(x) += 1.; // the new occurrence of x is stored
  }
  computedProba /= computedProba.sum();
  computedProba.maxCoeff(&computedMode);
#ifdef MC_DEBUG
  std::cout << "Final path, displayPath: " << std::endl;
  displayPath(path);
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif

  ASSERT_EQ(expectedMode, computedMode); // has the real mode been estimated correctly ?
}

///**
// * Test if a null precision implies an equipartition of the sampled x value
// */
//TEST(Ordinal, GibbsSamplingNullPrecision)
//{
//  int mu = 1; // mode
//  Real pi = 0.; // precision
//  int nbIter = 10000; // number of calls to samplePath
//  Vector<Real, 2> computedProba; // computed probability distribution of x
//  Vector<Real, 2> expectedProba; // expected probability distribution of x
//  computedProba = 0.;
//  expectedProba = 0.5;
//  int x; // x value to be sampled at each iteration
//
//  MultinomialStatistic multi;
//
//  Vector<int, 2> eInit; // vector describing initial segment
//  eInit << 0, 1;
//
//  Vector<int, 2> endCond; // vector describing the constraint on the value
//  endCond << 0, 1;
//
//  Vector<OrdinalProba::BOSNode> c(1); // initial search process of the Gibbs sampler
//  c(0).y_ = 0;
//  c(0).z_ = 0;
//  OrdinalProba::partition(eInit,
//                          c(0).y_,
//                          c(0).part_);
//  c(0).e_ << 0, 0;
//
//  for (int i = 0; i < nbIter; ++i)
//  {
//#ifdef MC_DEBUG
//    std::cout << "i: " << i << std::endl;
//#endif
//    samplePath(eInit,
//               c,
//               endCond,
//               mu,
//               pi,
//               multi);
//    x = c(0).e_(0); // x is sampled here
//    computedProba(x) += 1.; // the new occurrence of x is stored
//  }
//  computedProba /= computedProba.sum();
//
//#ifdef MC_DEBUG
//  std::cout << "computedProba" << std::endl;
//  std::cout << computedProba << std::endl;
//#endif
//
//  ASSERT_LT((expectedProba - computedProba).norm(), 0.01);
//}
//
///**
// * Test if a non-null precision implies a corrected estimation of the mode, from sampled values
// */
//TEST(Ordinal, GibbsSamplingMode)
//{
//  int mu = 1; // mode
//  Real pi = 0.5; // precision
//  int nbIter = 10000; // number of calls to samplePath
//  Vector<Real, 2> computedProba; // computed probability distribution of x
//  computedProba = 0.;
//  int computedMode; // computed mode
//  int x; // x value to be sampled at each iteration
//
//  MultinomialStatistic multi;
//
//  Vector<int, 2> eInit; // vector describing initial segment
//  eInit << 0, 1;
//
//  Vector<int, 2> endCond; // vector describing the constraint on the value
//  endCond << 0, 1;
//
//  Vector<OrdinalProba::BOSNode> c(1); // initial search process of the Gibbs sampler
//
//  c(0).y_ = 1;
//  c(0).z_ = 1;
//  OrdinalProba::partition(eInit,
//                          c(0).y_,
//                          c(0).part_);
//  c(0).e_ << 1, 1;
//
//  for (int i = 0; i < nbIter; ++i)
//  {
//#ifdef MC_DEBUG
//    std::cout << "i: " << i << std::endl;
//#endif
//    OrdinalProba::samplePath(eInit,
//                             c,
//                             endCond,
//                             mu,
//                             pi,
//                             multi);
//    x = c(0).e_(0); // x is sampled here
//    computedProba(x) += 1.; // the new occurrence of x is stored
//  }
//  computedProba /= computedProba.sum();
//  computedProba.maxCoeff(&computedMode);
//#ifdef MC_DEBUG
//  std::cout << "computedProba" << std::endl;
//  std::cout << computedProba << std::endl;
//#endif
//
//  ASSERT_EQ(computedMode, mu); // has the real mode been estimated correctly ?
//}
//
///**
// * Test if a non-null precision implies a corrected estimation of the mode, from sampled values
// */
//TEST(Ordinal, GibbsSamplingRangeConstraint)
//{
//  int mu = 0; // mode
//  Real pi = 0.5; // precision
//  int nbIter = 100; // number of calls to samplePath
//  Vector<Real, 3> computedProba; // computed probability distribution of x
//  computedProba = 0.;
//  int computedMode; // computed mode
//  int expectedMode = 1; // expected mode not equal to "parameter" mode, as custom constraints are provided on x values
//  int x; // x value to be sampled at each iteration
//
//  MultinomialStatistic multi;
//
//  Vector<int, 2> initSeg; // vector describing initial segment
//  initSeg << 0, 2;
//
//  Vector<int, 2> endCond; // vector describing the constraint on the value
//  endCond << 1, 2;
//
//  Vector<OrdinalProba::BOSNode> c(2); // initial search process of the Gibbs sampler
//  c(0).y_ = 0;
//  c(0).z_ = 0;
//  OrdinalProba::partition(initSeg,
//                          c(0).y_,
//                          c(0).part_);
//  c(0).e_ << 1, 2;
//  c(1).y_ = 2;
//  c(1).z_ = 0;
//  OrdinalProba::partition(c(1).e_,
//                          c(1).y_,
//                          c(1).part_);
//  c(1).e_ << 2, 2;
//
////  OrdinalProba::initPath(initSeg,
////                         endCond,
////                         multi,
////                         c);
//
//#ifdef MC_DEBUG
//  std::cout << "Initial path" << std::endl;
//  OrdinalProba::displayPath(initSeg,
//                            c);
//#endif
//
//  for (int iter = 0; iter < nbIter; ++iter)
//  {
//#ifdef MC_DEBUG
//    std::cout << "iter: " << iter << std::endl;
//#endif
//    OrdinalProba::samplePath(initSeg,
//                             c,
//                             endCond,
//                             mu,
//                             pi,
//                             multi);
//    x = c(1).e_(0); // x is sampled here
//    computedProba(x) += 1.; // the new occurrence of x is stored
//  }
//  computedProba /= computedProba.sum();
//  computedProba.maxCoeff(&computedMode);
//#ifdef MC_DEBUG
//  std::cout << "computedProba" << std::endl;
//  std::cout << computedProba << std::endl;
//#endif
//
//  ASSERT_EQ(computedMode, expectedMode); // has the real mode been estimated correctly ?
//}
//
///**
// * Test allowing a single value in the Gibbs sampling (typical usage as data value is known)
// */
//TEST(Ordinal, GibbsSamplingValConstraint)
//{
//  int mu = 1; // mode
//  Real pi = 0.5; // precision
//  int nbIter = 10000; // number of calls to samplePath
//  Vector<Real> computedProba(3); // computed probability distribution of x
//  computedProba = 0.;
//  int computedMode; // computed mode
//  int expectedMode = 2; // expected mode not equal to "parameter" mode, as custom constraints are provided on x values
//  int x; // x value to be sampled at each iteration
//
//  MultinomialStatistic multi;
//
//  Vector<int, 2> eInit; // vector describing initial segment
//  eInit << 0, 2;
//
//  Vector<int, 2> endCond; // vector describing the constraint on the value
//  endCond << 2, 2;
//
//  Vector<OrdinalProba::BOSNode> c(2); // initial search process of the Gibbs sampler
//
//  c(0).y_ = 0;
//  c(0).z_ = 0;
//  OrdinalProba::partition(eInit,
//                          c(0).y_,
//                          c(0).part_);
//  c(0).e_ << 1, 2;
//  c(1).y_ = 2;
//  c(1).z_ = 0;
//  OrdinalProba::partition(c(1).e_,
//                          c(1).y_,
//                          c(1).part_);
//  c(1).e_ << 2, 2;
//
//  for (int i = 0; i < nbIter; ++i)
//  {
//#ifdef MC_DEBUG
//    std::cout << "i: " << i << std::endl;
//#endif
//    samplePath(eInit,
//               c,
//               endCond,
//               mu,
//               pi,
//               multi);
//    x = c(1).e_(0); // x is sampled here
//#ifdef MC_DEBUG
//    std::cout << "x: " << x << std::endl;
//#endif
//    computedProba(x) += 1.; // the new occurrence of x is stored
//#ifdef MC_DEBUG
//   std::cout << "x: " << x << std::endl;
//   std::cout << "computedProba" << std::endl;
//   std::cout << computedProba << std::endl;
//#endif
//  }
//  computedProba /= computedProba.sum();
//  computedProba.maxCoeff(&computedMode);
//#ifdef MC_DEBUG
//  std::cout << "computedProba" << std::endl;
//  std::cout << computedProba << std::endl;
//#endif
//
//  ASSERT_EQ(computedMode, expectedMode); // has the real mode been estimated correctly ?
//}
