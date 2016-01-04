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

#include "../UTest.h"

using namespace mixt;

/** Coefficient-wise substract a vector to a scalar */
TEST(Vector, SubScalar)
{
  int nbTest = 10;
  int nbInd = 10;
  Vector<bool> testCorrect(nbTest, false);

  Real bound = 1.e8;

  UniformStatistic uni;

  for (int t = 0; t < nbTest; ++t)
  {
    Real scalar = uni.sample(- bound,
                               bound);
    Vector<Real> vector(nbInd);
    Vector<Real> expectedSub(nbInd);

    for (int i = 0; i < nbInd; ++i)
    {
      vector(i) = uni.sample(- bound,
                               bound);
      expectedSub(i) = scalar - vector(i);
    }

    testCorrect = ((scalar - vector) == expectedSub);
  }

ASSERT_EQ(testCorrect, true);
}

/** Coefficient-wise inverse of a vector */
TEST(Vector, CoeffInv)
{
  Vector<Real> vecA(3);
  vecA << 1., 3., 7.;
  Vector<Real> vecB(3);
  vecB << 1. / 1., 1. / 3., 1. / 7.;

  EXPECT_TRUE(vecA.cInv().isApprox(vecB));
}

TEST(Vector, QuotientScalar)
{
  Vector<Real> vecA(3);
  vecA << 1., 3., 7.;
  Vector<Real> vecB(3);
  vecB << 0.5, 1.5, 3.5;

  EXPECT_TRUE((vecA / 2).isApprox(vecB));
}

TEST(Vector, ScalarQuotient)
{
  Vector<Real> vecA(3);
  vecA << 1., 3., 7.;
  Vector<Real> vecB(3);
  vecB << 2. / 1., 2. / 3., 2. / 7.;

  EXPECT_TRUE((2 / vecA).isApprox(vecB));
}

/** Coefficient-wise add a scalar to a vector */
TEST(Vector, AddScalar)
{
  Vector<Real> vecA(3);
  vecA << 1., 3., 7.;
  Vector<Real> vecB(3);
  vecB << 3., 5., 9.;

  EXPECT_TRUE((2. + vecA).isApprox(vecB));
}

// Test double inversion of a matrix
TEST(Matrix, Inversion)
{
  Matrix<Real> m(3, 3);
  m << 1, 3, 7,
      13, 15, 5,
       1, 3, 0;
  EXPECT_TRUE(m.inverse().inverse().isApprox(m));
}

// Test the two types of Matrix iterators on complete matrices
TEST(Matrix, iterator)
{
  Matrix<int> a(3, 3);
  a << 1, 3, 7,
      13, 15, 5,
       1, 3, 0;

#ifdef MC_DEBUG
    std::cout << "a" << std::endl;
    std::cout << a << std::endl;
#endif

  Matrix<int> b(3, 3);

  Matrix<int>::const_iterator itA = a.begin();
  Matrix<int>::iterator itB = b.begin();

  for(;
      itA != a.end();
      ++itA,
      ++itB)
  {
#ifdef MC_DEBUG
    std::cout << "*itA: " << *itA << std::endl;
#endif

    *itB = *itA;
  }

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

// Test the matrix sortIndex
TEST(Matrix, sortIndex)
{
  Matrix<int> a(3, 3);
  a << 1, 3, 7,
      13, 15, 5,
       -41, -7, -9;
  Vector<int> indComputed;
  Vector<int> indExpected(3);
  indExpected << 2, 0, 1;

  a.col(1).sortIndex(indComputed);

  ASSERT_EQ(indComputed, indExpected);
}

/** Test if the operator < between matrices of same size is correctly implemented */
TEST(Matrix, comparison)
{
  Vector<bool> res(3);
  Matrix<int> a(2, 3);
  Matrix<int> b(2, 3);

  // case where one element in b is smaller than one element in a
  a << 1, 3, 7,
       13, 15, 21;
  b << 1, 3, 5,
       13, 15, 21;
  res(0) = b < a;

  // case where one element in a is smaller than one element in b
  a << 1, 2, 7,
       13, 15, 21;
  b << 1, 3, 5,
       13, 15, 21;
  res(1) = a < b;

  // case when the two matrices are equal
  a << 1, 2, 7,
       13, 15, 21;
  b << 1, 2, 5,
       13, 15, 21;
  res(2) = !(a < b);

  ASSERT_EQ(res, Vector<bool>(3, true));
}
