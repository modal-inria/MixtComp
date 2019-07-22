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
