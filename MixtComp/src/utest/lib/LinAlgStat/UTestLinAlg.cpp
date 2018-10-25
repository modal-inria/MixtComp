/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Feb 02, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

/** Coefficient-wise substract a vector to a scalar */
TEST(Vector, SubScalar) {
	Index nbInd = 10;

	Real bound = 1.e4;

	UniformStatistic uni;

	Real scalar = uni.sample(-bound, bound);
	Vector<Real> sampled(nbInd);
	Vector<Real> expectedSub(nbInd);

	for (int i = 0; i < nbInd; ++i) {
		sampled(i) = uni.sample(-bound, bound);
		expectedSub(i) = scalar - sampled(i);
	}

	Vector<Real> computedSub = scalar - sampled;

	EXPECT_TRUE(computedSub.isApprox(expectedSub));
}

/** Coefficient-wise inverse of a vector */
TEST(Vector, CoeffInv) {
	Vector<Real> vecA(3);
	vecA << 1., 3., 7.;
	Vector<Real> vecB(3);
	vecB << 1. / 1., 1. / 3., 1. / 7.;

	EXPECT_TRUE(vecA.cInvE().isApprox(vecB));
}

TEST(Vector, QuotientScalar) {
	Vector<Real> vecA(3);
	vecA << 1., 3., 7.;
	Vector<Real> vecB(3);
	vecB << 0.5, 1.5, 3.5;

	EXPECT_TRUE((vecA / 2.).isApprox(vecB));
}

TEST(Vector, ScalarQuotient) {
	Vector<Real> vecA(3);
	vecA << 1., 3., 7.;
	Vector<Real> vecB(3);
	vecB << 2. / 1., 2. / 3., 2. / 7.;

	EXPECT_TRUE((2. / vecA).isApprox(vecB));
}

/** Coefficient-wise add a scalar to a vector */
TEST(Vector, AddScalar) {
	Vector<Real> vecA(3);
	vecA << 1., 3., 7.;
	Vector<Real> vecB(3);
	vecB << 3., 5., 9.;

	EXPECT_TRUE((2. + vecA).isApprox(vecB));
}

// Test double inversion of a matrix
TEST(Matrix, Inversion) {
	Matrix<Real> m(3, 3);
	m << 1, 3, 7, 13, 15, 5, 1, 3, 0;
	EXPECT_TRUE(m.inverse().inverse().isApprox(m));
}

// Test the two types of Matrix iterators on complete matrices
TEST(Matrix, iterator) {
	Matrix<int> a(3, 3);
	a << 1, 3, 7, 13, 15, 5, 1, 3, 0;

#ifdef MC_DEBUG
	std::cout << "a" << std::endl;
	std::cout << a << std::endl;
#endif

	Matrix<int> b(3, 3);

	Matrix<int>::const_iterator itA = a.begin();
	Matrix<int>::iterator itB = b.begin();

	for (; itA != a.end(); ++itA, ++itB) {
#ifdef MC_DEBUG
		std::cout << "*itA: " << *itA << std::endl;
#endif

		*itB = *itA;
	}

	ASSERT_EQ(a, b);
}

// Test the matrix sort
TEST(Matrix, sort) {
	Matrix<int> a(3, 3);
	a << 1, 3, 7, 13, 15, 5, -41, 3, -9;
	Matrix<int> b(3, 3);
	b << -41, 3, 7, -9, 3, 13, 1, 5, 15;

	a.sort();

	ASSERT_EQ(a, b);
}

// Test the matrix sortIndex
TEST(Matrix, sortIndex) {
	Matrix<int> a(3, 3);
	a << 1, 3, 7, 13, 15, 5, -41, -7, -9;
	Vector<int> indComputed;
	Vector<int> indExpected(3);
	indExpected << 2, 0, 1;

	a.col(1).sortIndex(indComputed);

	ASSERT_EQ(indComputed, indExpected);
}

/** Test if the operator < between matrices of same size is correctly implemented */
TEST(Matrix, comparison) {
	Vector<bool> res(3);
	Matrix<int> a(2, 3);
	Matrix<int> b(2, 3);

	// case where one element in b is smaller than one element in a
	a << 1, 3, 7, 13, 15, 21;
	b << 1, 3, 5, 13, 15, 21;
	res(0) = b < a;

	// case where one element in a is smaller than one element in b
	a << 1, 2, 7, 13, 15, 21;
	b << 1, 3, 5, 13, 15, 21;
	res(1) = a < b;

	// case when the two matrices are equal
	a << 1, 2, 7, 13, 15, 21;
	b << 1, 2, 5, 13, 15, 21;
	res(2) = !(a < b);

	ASSERT_EQ(res, Vector<bool>(3, true));
}

TEST(LinAlg, InitializerList) {
	Vector<int> expectedVec(3);
	expectedVec << 1, 2, 3;

	Vector<int> computedVec;
	computedVec = {1, 2, 3};

	ASSERT_EQ(computedVec, expectedVec);
}

TEST (LinAlg, VectorDataAccess) {
	Index sizeVec = 4;

	Vector<Real> alphaVec(sizeVec, 0.);

	double* alphaDouble = alphaVec.data();

	for (Index i = 0; i < sizeVec; ++i) {
		alphaDouble[i] = i;
	}

	ASSERT_NEAR((sizeVec - 1.) / 2., alphaVec.mean(), epsilon);
}

TEST(Vector, importIterable) {
	std::list<Real> list;
	list.push_back(3.);
	list.push_back(12.);
	list.push_back(5.);

	Vector<Real> vec;
	vec.copyIterable(list);

	Vector<Real> expectedVec(3);
	expectedVec << 3., 12., 5.;

	ASSERT_EQ(vec, expectedVec);
}

/**
 * Test if using resize on an already filled matrix preserved the data
 */
TEST(Matrix, resizePreservesData) {
	Matrix<Real> initial(3, 3);
	initial << 1., 2., 3., 4., 5., 6., 7., 8., 9.;
	initial = initial.block(0, 0, 3, 2);

	Matrix<Real> expected(3, 2);
	expected << 1., 2., 4., 5., 7., 8.;

	EXPECT_TRUE(initial.isApprox(expected, epsilon));
}

TEST(Vector, resizePreservesData) {
	Vector<Real> initial(3);
	initial << 1., 2., 3.;
	initial = initial.block(0, 0, 2, 1);

	Vector<Real> expected(2);
	expected << 1., 2.;

	EXPECT_TRUE(initial.isApprox(expected, epsilon));
}
