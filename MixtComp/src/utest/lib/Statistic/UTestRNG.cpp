/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: January the 23rd, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/**
 * The deterministic mode must be enabled for the unit-test results to be meaningful.
 */
TEST(RNG, deterministicMode) {
	ASSERT_EQ(true, deterministicMode());
}
