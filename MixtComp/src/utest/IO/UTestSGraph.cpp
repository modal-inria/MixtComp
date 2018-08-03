/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/** Coefficient-wise substract a vector to a scalar */
TEST(SGraph, BasicIO) {
	SGraph test;

	std::string nameStr = "Quote";
	std::string testStr = "They shall not pass.";

	test.add_payload(nameStr, testStr);

	AlgType res = test.get_payload(nameStr);
	std::string resStr = boost::apply_visitor(SGraphPrintVis(), res);

	ASSERT_EQ(testStr, resStr);
}
