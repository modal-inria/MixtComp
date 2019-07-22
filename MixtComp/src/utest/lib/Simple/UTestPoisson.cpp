/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

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
