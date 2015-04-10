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
  int x = 1; // value x obtained at the end of the binary search algorithm, proba 1. as upper segment was selected

  Real proba = OrdinalProba::computeProba(eInit,
                                          c,
                                          x,
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
  int x = 0; // the mode was not picked !

  Real proba = OrdinalProba::computeProba(eInit,
                                          c,
                                          x,
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
  int x = 1; // the mode was not picked, but x is compatible with last e, therefore conditional probability is 1.

  int index = 1; // iteration where y conditional probability is to be computed
  Vector<Real> computedProba; // conditional probability distribution actually computed by multinomialY
  Vector<Real> expectedProba(3); // conditional probability expected
  expectedProba(0) = 0.5; // since comparison is perfect, the splitting point does not matter
  expectedProba(1) = 0.5; // since comparison is perfect, the splitting point does not matter
  expectedProba(2) = 0.; // point is unreachable at iteration 2

  multinomialY(eInit,
               c,
               x,
               mu,
               pi,
               computedProba,
               index);

#ifdef MC_DEBUG_NEW
  std::cout << "computedProba" << std::endl;
  std::cout << computedProba << std::endl;
#endif
  EXPECT_TRUE(computedProba.isApprox(expectedProba));
}

/**
 * Test the probability vector obtained by multinomialY
 */
//TEST(Ordinal, multinomialZ0)
//{
//  int mu = 3; // mode
//  Real pi = 0.34; // precision
//
//  std::pair<int, int> eInit; // vector describing initial segment
//  eInit.first = 3;
//  eInit.second = 5;
//
//  Vector<OrdinalProba::ItBOS> c(1); // vector describing the search process
//
//  c(0).y_ = 4; // y picked, proba 1./3.
//  c(0).z_ = 1; // conditional probability will be computed over this variable
//  c(0).e_ = std::pair<int, int> (5, 5); // e, right segment selected, proba 2./3. (sizes of segments are not equal)
//  int x = 5; // the mode was not picked, but x is compatible with last e, therefore conditional probability is 1.
//
//  int index = 0; // iteration where y conditional probability is to be computed
//  Vector<Real> computedProba; // conditional probability distribution actually computed by multinomialY
//  Vector<Real> expectedProba(2); // conditional probability expected
//  expectedProba(0) = 1.; // only an imprecise comparison will yield 5 while the mode is at 3
//  expectedProba(1) = 0.; // perfect comparison is only possible for x = 3
//
//  multinomialZ(eInit,
//               c,
//               x,
//               mu,
//               pi,
//               computedProba,
//               index);
//
//#ifdef MC_DEBUG_NEW
//  std::cout << "computedProba" << std::endl;
//  std::cout << computedProba << std::endl;
//#endif
//  EXPECT_TRUE(computedProba.isApprox(expectedProba));
//}
