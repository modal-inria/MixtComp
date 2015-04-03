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
 *  Created on: Feb 02, 2015
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
  c(2) = 2; // upper segment selected, proba 1.
  int x = 1; // value x obtained at the end of the binary search algorithm, proba 1. as upper segment was selected

  Real proba = OrdinalProba::computeProba(c,
                                          x,
                                          eVal,
                                          mu,
                                          pi);

  ASSERT_LT(std::abs(0.25 - proba), epsilon);
}

// Conditional probabilities sum to 1
// Mode is on the correct modality
