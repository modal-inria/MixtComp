/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

/**
 * Check that classes which do not contain 0 only values are validated.
 */
TEST(Poisson_k, OKSample) {
	Index nbClass = 2;
	Index nbInd = 4;

	Vector<Real> param(nbClass);
	param << 3, 1;

	Vector<std::set<Index> > classInd(nbClass);
	classInd(0) = {0, 2};
	classInd(1) = {1, 3};

	AugmentedData<Vector<int>> data;
	data.resizeArrays(nbInd);
	data.setPresent(0, 0);
	data.setPresent(1, 1);
	data.setPresent(2, 5);
	data.setPresent(3, 6);

	Poisson mixture("Poisson", nbClass, param);

	mixture.setData("", data, learning_);
	std::string warnLog = mixture.checkSampleCondition(classInd);

	ASSERT_EQ(warnLog.size(), 0);
}

/**
 * Check that classes which do not contain 0 only values are validated.
 * This test is only meaningful if degeneracyAuthorizedForNonBoundedLikelihood = false
 */
//TEST(Poisson_k, 0OnlyClass) {
//	Index nbClass = 2;
//	Index nbInd = 4;
//
//	Vector<Real> param(nbClass);
//	param << 3, 1;
//
//	Vector<std::set<Index> > classInd(nbClass);
//	classInd(0) = {0, 2};
//	classInd(1) = {1, 3};
//
//	AugmentedData<Vector<int>> data;
//	data.resizeArrays(nbInd);
//	data.setPresent(0, 0);
//	data.setPresent(1, 1);
//	data.setPresent(2, 0);
//	data.setPresent(3, 6);
//
//	Poisson mixture("Poisson", nbClass, param);
//
//	mixture.setData("", data, learning_);
//	std::string warnLog = mixture.checkSampleCondition(classInd);
//
//	ASSERT_GT(warnLog.size(), 0);
//}
