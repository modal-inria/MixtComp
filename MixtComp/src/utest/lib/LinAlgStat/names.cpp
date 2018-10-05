/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

TEST(names, real) {
	ASSERT_EQ(names<Real>::name, std::string("Real"));
}
