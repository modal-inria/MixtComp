/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/**
 * Generate individuals that follows a distribution, and try to estimate it back. Note that the individual are generated
 * in the constructor of Ordinal, which is a special constructor used for debugging.
 */
TEST(Ordinal, mStep) {
	int nbInd = 100;
	int nbModalities = 4;
	int mu;
	Real pi;
	Real errorTolerance = 0.05;

	MultinomialStatistic multi;
	UniformStatistic uni;

	mu = multi.sampleInt(0, nbModalities - 1);
	pi = uni.sample(0., 1.);

	Vector<Index> z_i(nbInd, 0); // dummy class variable
	Vector<std::set<Index> > classInd(1);

	for (int i = 0; i < nbInd; ++i) {
		classInd(0).insert(i);
	}

	Ordinal<DataHandlerDummy, DataExtractorDummy, ParamSetterDummy,
			ParamExtractorDummy> ordinal(1, nbInd, nbModalities, mu, pi);

	ordinal.mStep(classInd);
	Vector<int> muEst = ordinal.mu(); // estimated mu
	Vector<Real> piEst = ordinal.pi(); // estimated pi

	ASSERT_EQ(mu, muEst(0));
	ASSERT_LT(std::abs(pi - piEst(0)), errorTolerance);
}

/**
 * Test if the number of combination
 */
//TEST(Ordinal, tupleMultinomial) {
//	Index mu = 0;
//	Real pi = 0.5;
//
//	BOSPath path;
//	path.setInit(0, 2);
//	path.setEnd(0, 2);
//
//	Index index = 0; // index of the first node of the pair used in the computation
//
//	std::list<Vector<BOSNode> > pathList;
//	Vector<Real> probaVec;
//	path.tupleMultinomial(mu, pi, index, sizeTupleConst, pathList, probaVec);
//
//	ASSERT_EQ(pathList.size(), 32); // is the size of pathList correct ?
//}

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
