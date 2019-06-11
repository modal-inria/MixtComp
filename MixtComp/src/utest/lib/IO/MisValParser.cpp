/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: October 12, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <regex>

#include "gtest/gtest.h"
#include "MixtComp.h"
#include "jsonIO.h"

using namespace mixt;

TEST(MisValParser, reNumber) {
	std::regex reNumber(strNumber);
	std::smatch matches;
	Real exp = -0.956;
	std::string str = type2str(exp);
	Real v = 0.0;

	if (std::regex_match(str, matches, reNumber)) { // value is present
		v = str2type<Real>(matches[1].str());
	}

	ASSERT_NEAR(v, exp, epsilon);
}

TEST(MisValParser, reValue) {
	std::regex reValue(strBlank + strNumber + strBlank);
	std::smatch matches;
	std::string str("    -3.5   ");
	Real v = 0.0;

	if (std::regex_match(str, matches, reValue)) { // value is present
		v = str2type<Real>(matches[1].str());
	}

	ASSERT_NEAR(v, -3.5, epsilon);
}

TEST(MisValParser, reMissing) {
	std::regex reMissing(strBlank + strQMark + strBlank);
	std::smatch matches;
	std::string str("  ?   ");
	bool v = false;

	if (std::regex_match(str, matches, reMissing)) { // value is present
		v = true;
	}

	ASSERT_TRUE(v);
}
