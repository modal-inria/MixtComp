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
	AlgType obj;
	obj = Index(12);

	Real res;
	try {
		boost::apply_visitor(AlgTypeVisitor<Real>(res), obj);
	} catch (const std::string& s) {
		std::cout << s << std::endl;
	}
}
