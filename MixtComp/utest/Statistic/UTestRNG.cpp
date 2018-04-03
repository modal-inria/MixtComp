/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January the 23rd, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/**
 * The deterministic mode must be enabled for the unit-test results to be meaningful.
 */
TEST(RNG, deterministicMode) {
	ASSERT_EQ(true, deterministicMode());
}
