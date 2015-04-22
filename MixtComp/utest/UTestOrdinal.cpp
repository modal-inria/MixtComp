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
#include "../src/Mixture/Ordinal/mixt_OrdinalProba.h"
#include "../src/Various/mixt_Constants.h"
#include "../src/Statistic/mixt_MultinomialStatistic.h"

using namespace mixt;

/**
 * Simple case with two modalities
 */
TEST(Ordinal, computeProba0)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 0;
  eInit.second = 1;

  Vector<OrdinalProba::ItBOS> c(1); // vector describing the search process

  c(0).y_ = 1; // second element y picked, proba 0.5
  c(0).z_ = 1; // comparison is perfect, proba 0.5
  OrdinalProba::partition(eInit, // computation of the partition
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (1, 1); // segment is {1}, proba 1.

  Real proba = OrdinalProba::computeProba(eInit,
                                          c,
                                          mu,
                                          pi);

  ASSERT_LT(std::abs(0.25 - proba), epsilon);
}

/**
 * Simple case with three modalities and imprecision
 */
TEST(Ordinal, computeProba1)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 0;
  eInit.second = 2;

  Vector<OrdinalProba::ItBOS> c(2); // vector describing the search process

  c(0).y_ = 1; // y, middle element y picked, proba 1./3.
  c(0).z_ = 0; // z, comparison is imperfect, proba 0.5
  OrdinalProba::partition(eInit, // computation of the partition
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (0, 0); // e, left segment selected, proba 0.33 (all have the same size)

  c(1).y_ = 0; // y, only one element to choose from, proba 1.
  c(1).z_ = 1; // z, comparison is perfect, proba 0.5
  OrdinalProba::partition(c(0).e_, // computation of the partition
                          c(1).y_,
                          c(1).part_);
  c(1).e_ = std::pair<int, int> (0, 0); // e, only one segment, in the middle, with proba 1.

  Real proba = OrdinalProba::computeProba(eInit,
                                          c,
                                          mu,
                                          pi);

  ASSERT_LT(std::abs(  1./3. * 0.5 * 1./3.
                     * 1. * 0.5 * 1.
                     - proba), epsilon);
}

/**
 * Test the probability vector obtained by multinomialY
 */
TEST(Ordinal, multinomialY0)
{
  int mu = 2; // mode
  Real pi = 0.57; // precision

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 0;
  eInit.second = 2;

  Vector<OrdinalProba::ItBOS> c(2); // vector describing the search process

  c(0).y_ = 2; // y picked, proba 1./3.
  c(0).z_ = 0; // z, comparison is imperfect, proba (1. - 0.57)
  OrdinalProba::partition(eInit, // computation of the partition
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (0, 1); // e, segment {0, 1} selected, proba 2./3. (sizes of segments are not equal)

  c(1).y_ = 0; // vector of proba will be computed on this value
  c(1).z_ = 1; // z, comparison is perfect, proba 0.57
  OrdinalProba::partition(c(0).e_, // computation of the partition
                          c(1).y_,
                          c(1).part_);
  c(1).e_ = std::pair<int, int> (1, 1); // best segment, {1}, considering that the comparison is perfect

  int index = 1; // iteration where y conditional probability is to be computed
  Vector<Real> computedProba; // conditional probability distribution actually computed by multinomialY
  Vector<Real> expectedProba(2); // conditional probability expected
  expectedProba(0) = 0.5; // since comparison is perfect, the splitting point does not matter
  expectedProba(1) = 0.5; // since comparison is perfect, the splitting point does not matter
  int minVal;

  yMultinomial(eInit,
               c,
               mu,
               pi,
               index,
               computedProba,
               minVal);

#ifdef MC_DEBUG
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif
  EXPECT_TRUE(computedProba.isApprox(expectedProba) && minVal == 0);
}

/**
 * Test the probability vector obtained by multinomialY
 */
TEST(Ordinal, multinomialZ0)
{
  int mu = 3; // mode
  Real pi = 0.34; // precision

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 3;
  eInit.second = 5;

  Vector<OrdinalProba::ItBOS> c(1); // vector describing the search process

  c(0).y_ = 4; // y picked, proba 1./3.
  c(0).z_ = 1; // conditional probability will be computed over this variable
  OrdinalProba::partition(eInit, // computation of the partition
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (5, 5); // e, right segment selected, proba 2./3. (sizes of segments are not equal)

  int index = 0; // iteration where y conditional probability is to be computed
  Vector<Real> computedProba; // conditional probability distribution actually computed by multinomialY
  Vector<Real> expectedProba(2); // conditional probability expected
  expectedProba(0) = 1.; // only an imprecise comparison will yield 5 while the mode is at 3
  expectedProba(1) = 0.; // perfect comparison is only possible for x = 3

  zMultinomial(c,
               mu,
               pi,
               index,
               computedProba);

#ifdef MC_DEBUG
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif
  EXPECT_TRUE(computedProba.isApprox(expectedProba));
}

/**
 * Test the probability vector obtained by multinomialE
 */
TEST(Ordinal, multinomialE0)
{
  int mu = 3; // mode
  Real pi = 0.21; // precision

  std::pair<int, int> eInit; // vector describing initial segment, mu is already outside the range
  eInit.first = 6;
  eInit.second = 8;

  Vector<int> endCond(0);

  Vector<OrdinalProba::ItBOS> c(2); // vector describing the search process

  c(0).y_ = 8; // y picked, proba 1./3.
  c(0).z_ = 1; // conditional probability will be computed over this variable
  OrdinalProba::partition(eInit, // computation of the partition
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (6, 7); // value which will be variable

  c(1).y_ = 6; // y picked, proba 1./2.
  c(1).z_ = 1;
  OrdinalProba::partition(c(0).e_, // computation of the partition
                          c(1).y_,
                          c(1).part_);
  c(1).e_ = std::pair<int, int> (6, 6); // e, right segment selected, proba 2./3. (sizes of segments are not equal)

  Vector<Real> computedProba; // conditional probability distribution actually computed by multinomialY
  Vector<Real> expectedProba(3); // conditional probability expected
  expectedProba(0) = 1.; // only an imprecise comparison will yield 5 while the mode is at 3
  expectedProba(1) = 0.; // perfect comparison is only possible for x = 3
  expectedProba(2) = 0.; // perfect comparison is only possible for x = 3

  eMultinomial(c,
               endCond,
               mu,
               pi,
               0, // iteration where e conditional probability is to be computed
               computedProba);

#ifdef MC_DEBUG
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif
  EXPECT_TRUE(computedProba.isApprox(expectedProba));
}

/**
 * Test if a null precision implies an equipartition of the sampled x value
 */
TEST(Ordinal, GibbsSamplingNullPrecision)
{
  int mu = 1; // mode
  Real pi = 0.; // precision
  int nbIter = 10000; // number of calls to samplePath
  Vector<Real> computedProba(2); // computed probability distribution of x
  Vector<Real> expectedProba(2); // expected probability distribution of x
  computedProba = 0.;
  expectedProba = 0.5;
  int x; // x value to be sampled at each iteration


  MultinomialStatistic multi;

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 0;
  eInit.second = 1;

  Vector<int> endCond(0); // vector describing the constraint on the value

  Vector<OrdinalProba::ItBOS> c(1); // initial search process of the Gibbs sampler

  c(0).y_ = 0;
  c(0).z_ = 0;
  OrdinalProba::partition(eInit,
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (0, 0);

  for (int i = 0; i < nbIter; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
#endif
    samplePath(eInit,
               c,
               endCond,
               mu,
               pi,
               multi);
    x = c(0).e_.first; // x is sampled here
    computedProba(x) += 1.; // the new occurrence of x is stored
#ifdef MC_DEBUG
   std::cout << "x: " << x << std::endl;
   std::cout << "computedProba" << std::endl;
   std::cout << computedProba << std::endl;
#endif
  }
  computedProba /= computedProba.sum();

#ifdef MC_DEBUG
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif

  ASSERT_LT((expectedProba - computedProba).norm(), 0.01);
}

/**
 * Test if a non-null precision implies a corrected estimation of the mode, from sampled values
 */
TEST(Ordinal, GibbsSamplingMode)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision
  int nbIter = 10000; // number of calls to samplePath
  Vector<Real> computedProba(2); // computed probability distribution of x
  computedProba = 0.;
  int computedMode; // computed mode
  int x; // x value to be sampled at each iteration

  MultinomialStatistic multi;

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 0;
  eInit.second = 1;

  Vector<int> endCond(0); // vector describing the constraint on the value

  Vector<OrdinalProba::ItBOS> c(1); // initial search process of the Gibbs sampler

  c(0).y_ = 1;
  c(0).z_ = 1;
  OrdinalProba::partition(eInit,
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (1, 1);

  for (int i = 0; i < nbIter; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
#endif
    samplePath(eInit,
               c,
               endCond,
               mu,
               pi,
               multi);
    x = c(0).e_.first; // x is sampled here
    computedProba(x) += 1.; // the new occurrence of x is stored
#ifdef MC_DEBUG
   std::cout << "x: " << x << std::endl;
   std::cout << "computedProba" << std::endl;
   std::cout << computedProba << std::endl;
#endif
  }
  computedProba /= computedProba.sum();
  computedProba.maxCoeff(&computedMode);
#ifdef MC_DEBUG
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif

  ASSERT_EQ(computedMode, mu); // has the real mode been estimated correctly ?
}

/**
 * Test if a non-null precision implies a corrected estimation of the mode, from sampled values
 */
TEST(Ordinal, GibbsSampling1RangeConstraint)
{
  int mu = 0; // mode
  Real pi = 0.5; // precision
  int nbIter = 10000; // number of calls to samplePath
  Vector<Real> computedProba(3); // computed probability distribution of x
  computedProba = 0.;
  int computedMode; // computed mode
  int expectedMode = 1; // expected mode not equal to "parameter" mode, as custom constraints are provided on x values
  int x; // x value to be sampled at each iteration

  MultinomialStatistic multi;

  std::pair<int, int> eInit; // vector describing initial segment
  eInit.first = 0;
  eInit.second = 2;

  Vector<int> endCond(2); // vector describing the constraint on the value
  endCond(0) = 1;
  endCond(1) = 2;

  Vector<OrdinalProba::ItBOS> c(2); // initial search process of the Gibbs sampler

  c(0).y_ = 0;
  c(0).z_ = 0;
  OrdinalProba::partition(eInit,
                          c(0).y_,
                          c(0).part_);
  c(0).e_ = std::pair<int, int> (1, 2);
  c(1).y_ = 2;
  c(1).z_ = 0;
  OrdinalProba::partition(c(1).e_,
                          c(1).y_,
                          c(1).part_);
  c(1).e_ = std::pair<int, int> (2, 2);

  for (int i = 0; i < nbIter; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
#endif
    samplePath(eInit,
               c,
               endCond,
               mu,
               pi,
               multi);
    x = c(1).e_.first; // x is sampled here
#ifdef MC_DEBUG
    std::cout << "x: " << x << std::endl;
#endif
    computedProba(x) += 1.; // the new occurrence of x is stored
#ifdef MC_DEBUG
   std::cout << "x: " << x << std::endl;
   std::cout << "computedProba" << std::endl;
   std::cout << computedProba << std::endl;
#endif
  }
  computedProba /= computedProba.sum();
  computedProba.maxCoeff(&computedMode);
#ifdef MC_DEBUG
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif

  ASSERT_EQ(computedMode, expectedMode); // has the real mode been estimated correctly ?
}
