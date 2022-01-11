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
 *  Created on: January 11, 2022
 *  Author:     Quentin GRIMONPREZ
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(MultinomialStatistic, sample) {
	MultinomialStatistic multi;

	Vector<Real> param(4);
    param << 0.2, 0.3, 0.4, 0.1;

    Vector<Real> proba(4, 0);
    for (int i = 0; i < 10000; ++i) {
		proba(multi.sample(param)) += 1.;
	}
	proba /= 10000.;

    for (Index i = 0; i < param.size(); ++i) {
        EXPECT_NEAR(proba(i), param(i), 0.02);
    }
}

TEST(MultinomialStatistic, sampleCallBlock) {
	MultinomialStatistic multi;

	Vector<Real> param(8);
    param << 0., 0.5, 0.3, 0.2, 0.2, 0.3, 0.4, 0.1;

	int nbModalities = 4;
	int z_i = 1;

    Vector<Real> proba(4, 0);
    for (int i = 0; i < 10000; ++i) {
		proba(multi.sample(param.block(z_i * nbModalities, 0,  // position of first element
                                       nbModalities      , 1))) += 1.;
	}
	proba /= 10000.;

    for (Index i = 0; i < proba.size(); ++i) {
        EXPECT_NEAR(proba(i), param(4 + i), 0.02);
    }
}

TEST(MultinomialStatistic, sampleBinomial) {
	MultinomialStatistic multi;

    Vector<Real> proba(2, 0);
    for (int i = 0; i < 10000; ++i) {
		proba(multi.sampleBinomial(0.7)) += 1.;
	}
	proba /= 10000.;

	EXPECT_NEAR(proba(0), 0.3, 0.02);
	EXPECT_NEAR(proba(1), 0.7, 0.02);
}

TEST(MultinomialStatistic, sampleInt) {
	MultinomialStatistic multi;

    Vector<Real> proba(6, 0);
    for (int i = 0; i < 10000; ++i) {
		proba(multi.sampleInt(2, 5)) += 1.;
	}
	proba /= 10000.;

	EXPECT_FLOAT_EQ(proba(0), 0.);
	EXPECT_FLOAT_EQ(proba(1), 0);
	EXPECT_NEAR(proba(2), 0.25, 0.02);
	EXPECT_NEAR(proba(3), 0.25, 0.02);
	EXPECT_NEAR(proba(4), 0.25, 0.02);
	EXPECT_NEAR(proba(5), 0.25, 0.02);
}
