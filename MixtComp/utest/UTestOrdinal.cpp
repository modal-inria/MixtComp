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

// Simple case with two modalities
TEST(Ordinal, simple1)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision

  std::pair<int, int> eVal; // vector describing initial segment
  eVal.first = 0;
  eVal.second = 1;

  Vector<int> c(3); // vector describing the search process
  c(0) = 1; // first element y picked, proba 0.5
  c(1) = 1; // comparison is perfect, proba 0.5
  c(2) = 1; // middle segment selected, proba 1.
  int x = 1; // value x obtained at the end of the binary search algorithm, proba 1. as upper segment was selected

  Real proba = OrdinalProba::computeProba(c,
                                          x,
                                          eVal,
                                          mu,
                                          pi);

  ASSERT_LT(std::abs(0.25 - proba), epsilon);
}

// Simple case with three modalities and imprecision
TEST(Ordinal, simple2)
{
  int mu = 1; // mode
  Real pi = 0.5; // precision

  std::pair<int, int> eVal; // vector describing initial segment
  eVal.first = 0;
  eVal.second = 2;

  Vector<int> c(6); // vector describing the search process
  c(0) = 1; // y, middle element y picked, proba 0.3
  c(1) = 0; // z, comparison is imperfect, proba 0.5
  c(2) = 0; // e, left segment selected, proba 0.33 (all have the same size)

  c(3) = 0; // y, only one element to choose from, proba 1.
  c(4) = 1; // z, comparison is perfect, proba 0.5
  c(5) = 1; // e, only one segment, in the middle, with proba 1.
  int x = 0; // the mode was not picked !

  Real proba = OrdinalProba::computeProba(c,
                                          x,
                                          eVal,
                                          mu,
                                          pi);

  ASSERT_LT(std::abs(  1./3. * 0.5 * 1./3.
                     * 1. * 0.5 * 1.
                     - proba), epsilon);
}

// Conditional probabilities sum to 1
// Mode is on the correct modality
