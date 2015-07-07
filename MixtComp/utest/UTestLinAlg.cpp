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
#include "../src/LinAlg/mixt_LinAlg.h"

using namespace mixt;

// Test double inversion of a matrix
TEST(Matrix, Inversion)
{
  Matrix<Real> m(3, 3);
  m << 1, 3, 7,
      13, 15, 5,
       1, 3, 0;
  EXPECT_TRUE(m.inverse().inverse().isApprox(m));
}

// Test the two types of Matrix iterators
TEST(Matrix, iterator)
{
  Matrix<int> a(3, 3);
  a << 1, 3, 7,
      13, 15, 5,
       1, 3, 0;
  Matrix<int> b(3, 3);

  Matrix<int>::const_iterator itA = a.begin();
  Matrix<int>::iterator itB = b.begin();

  for(;
      itA != a.end();
      ++itA,
      ++itB)
  {
    *itB = *itA;
  }

  ASSERT_EQ(a, b);
}

// Test the contiguous matrix sort
TEST(Matrix, sortContiguous)
{
  Matrix<int> a(3, 3);
  a << 1, 3, 7,
      13, 15, 5,
       -41, 3, -9;
  Matrix<int> b(3, 3);
  b << -41, 3, 7,
       -9, 3, 13,
       1, 5, 15;

  a.sortContiguous();

  ASSERT_EQ(a, b);
}

// Test the matrix sort
TEST(Matrix, sort)
{
  Matrix<int> a(3, 3);
  a << 1, 3, 7,
      13, 15, 5,
       -41, 3, -9;
  Matrix<int> b(3, 3);
  b << -41, 3, 7,
       -9, 3, 13,
       1, 5, 15;

  a.sort();

  ASSERT_EQ(a, b);
}
