/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/** Get to get value. */
TEST(SGraph, BasicIOGet) {
	SGraph test;

	std::string name = "Quote";
	Real val = 12.0;

	test.add_payload(name, val);

	Real res = test.get_payload<Real>(name);

	ASSERT_NEAR(val, res, epsilon);
}
