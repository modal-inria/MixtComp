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
TEST(SGraph, BasicIOVisitor) {
	SGraph test;

	std::string nameStr = "Quote";
	std::string testStr = "They shall not pass.";

	test.add_payload(nameStr, testStr);

	AlgType res = test.get_payload(nameStr);
	std::string resStr = boost::apply_visitor(SGraphPrintVis(), res);

	ASSERT_EQ(testStr, resStr);
}

/** Coefficient-wise substract a vector to a scalar */
TEST(SGraph, BasicIOGet) {
	SGraph test;

	std::string name = "Quote";
	Real val = 12.0;

	test.add_payload(name, val);

	AlgType resAlg = test.get_payload(name);
	Real res = boost::get<Real>(resAlg);

	ASSERT_NEAR(val, res, epsilon);
}
