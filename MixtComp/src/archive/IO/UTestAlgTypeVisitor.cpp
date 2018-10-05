/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 17, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

TEST(AlgTypeVisitor, BasicTest) {
	Index in = 12;

	AlgType obj;
	obj = in;

	Real res;

	res = boost::apply_visitor(AlgTypeVisitor<Real>(), obj);

	ASSERT_NEAR(Real(in), res, epsilon);
}
