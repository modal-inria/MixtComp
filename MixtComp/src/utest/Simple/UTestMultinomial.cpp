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
 *  Author:     Quentin GRIMONPREZ
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(Multinomial, EstimateParameters) {
	Index nObs = 10;

	Vector<Real> param(4);
    param << 0, 0, 0, 0;
	Vector<Real> paramExpected(4);
    paramExpected << 0.2, 0.3, 0.4, 0.1;
	std::string idName = "dummy";
	Index nClass = 1;

	AugmentedData<Vector<int>> data;
	data.resizeArrays(nObs);
	data.setPresent(0, 0);
	data.setPresent(1, 0);
	data.setPresent(2, 1);
	data.setPresent(3, 1);
    data.setPresent(4, 1);
	data.setPresent(5, 2);
    data.setPresent(6, 2);
	data.setPresent(7, 2);
    data.setPresent(8, 2);
	data.setPresent(9, 3);
    data.computeRange();

	Multinomial multiMixture(idName, nClass, param);
    std::string str = "";
    multiMixture.setData(str, data, learning_);

    EXPECT_TRUE(multiMixture.hasModalities());
    EXPECT_TRUE(multiMixture.acceptedType()(0));
    EXPECT_TRUE(multiMixture.acceptedType()(1));
    EXPECT_TRUE(multiMixture.acceptedType()(2));
    EXPECT_FALSE(multiMixture.acceptedType()(3));
    EXPECT_FALSE(multiMixture.acceptedType()(4));
    EXPECT_FALSE(multiMixture.acceptedType()(5));
	ASSERT_EQ(multiMixture.computeNbFreeParameters(), 3);

    Vector<std::set<Index>> classInd(1);
    classInd(0) = {0, 1 ,2, 3, 4, 5, 6, 7, 8, 9};
    multiMixture.mStep(classInd);

	EXPECT_FLOAT_EQ((param - paramExpected).norm(), 0);
}

TEST(MultinomialSampler, sample) {

    std::vector<int> empty;
    std::vector<int> mod(2);
    mod[0] = 1;
    mod[1] = 3;

    std::pair<MisType, std::vector<int> > miss (missing_, empty);
    std::pair<MisType, std::vector<int> > miss2 (missingFiniteValues_, mod);

	AugmentedData<Vector<int>> data;
	data.resizeArrays(3);
	data.setPresent(0, 0);
	data.setMissing(1, miss);
	data.setMissing(2, miss2);
    data.computeRange();

	Vector<Real> param(4);
    param << 0.2, 0.3, 0.4, 0.1;

    MultinomialSampler sampler(data, param, 1);

    // sampling present_
    sampler.samplingStepNoCheck(0, 0);
    EXPECT_EQ(data.data_(0), 0);

    // sampling missing_
    Vector<Real> proba(4, 0.);
    for (int i = 0; i < 10000; ++i) {
        sampler.samplingStepNoCheck(1, 0);
        proba(data.data_(1)) += 1.;
    }
    proba /= 10000.;

    for (Index i = 0; i < param.size(); ++i) {
        EXPECT_NEAR(proba(i), param(i), 0.02);
    }

    // sampling missingFiniteValues_
    Vector<Real> proba2(4, 0.);
    for (int i = 0; i < 10000; ++i) {
        sampler.samplingStepNoCheck(2, 0);
        proba2(data.data_(2)) += 1.;
    }
    proba2 /= 10000.;
    std::cout << proba2 << std::endl;
    EXPECT_FLOAT_EQ(proba2(0), 0.);
    EXPECT_NEAR(proba2(1), param(1) / (param(1) + param(3)), 0.02);
    EXPECT_FLOAT_EQ(proba2(2), 0.);
    EXPECT_NEAR(proba2(3), param(3) / (param(1) + param(3)), 0.02);
}


TEST(MultinomialSampler, sample2Class) {

    std::vector<int> empty;
    std::vector<int> mod(2);
    mod[0] = 1;
    mod[1] = 3;

    std::pair<MisType, std::vector<int> > miss (missing_, empty);
    std::pair<MisType, std::vector<int> > miss2 (missingFiniteValues_, mod);

	AugmentedData<Vector<int>> data;
	data.resizeArrays(3);
	data.setPresent(0, 0);
	data.setMissing(1, miss);
	data.setMissing(2, miss2);
    data.computeRange();

	Vector<Real> param(8);
    param << 0.2, 0.3, 0.4, 0.1, 0.7, 0.1, 0.1, 0.1;

    MultinomialSampler sampler(data, param, 2);

    // sampling present_
    sampler.samplingStepNoCheck(0, 1);
    EXPECT_EQ(data.data_(0), 0);

    // sampling missing_
    Vector<Real> proba(4, 0.);
    for (int i = 0; i < 10000; ++i) {
        sampler.samplingStepNoCheck(1, 1);
        proba(data.data_(1)) += 1.;
    }
    proba /= 10000.;

    for (Index i = 0; i < proba.size(); ++i) {
        EXPECT_NEAR(proba(i), param(4 + i), 0.02);
    }

    // sampling missingFiniteValues_
    Vector<Real> proba2(4, 0.);
    for (int i = 0; i < 10000; ++i) {
        sampler.samplingStepNoCheck(2, 1);
        proba2(data.data_(2)) += 1.;
    }
    proba2 /= 10000.;
    std::cout << proba2 << std::endl;
    EXPECT_FLOAT_EQ(proba2(0), 0.);
    EXPECT_NEAR(proba2(1), param(4 + 1) / (param(4 + 1) + param(4 + 3)), 0.02);
    EXPECT_FLOAT_EQ(proba2(2), 0.);
    EXPECT_NEAR(proba2(3), param(4 + 3) / (param(4 + 1) + param(4 + 3)), 0.02);
}
