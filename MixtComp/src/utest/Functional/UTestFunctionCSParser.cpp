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
 *  Created on: June 22, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(FuncCSParser, validData) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2; // useless as well
	Index nObs = 2;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<FunctionCS> vecInd;

	vecStr[0] = "12.:24.,13.:-3.,15.:90.3";
	vecStr[1] = "-5.4:2.3,-4.21:-3.";

	warnLog = parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	ASSERT_EQ(0, warnLog.size());
}

TEST(FuncCSParser, timeMissing) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2; // useless as well
	Index nObs = 2;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<FunctionCS> vecInd;

	vecStr[0] = ":24.,13.:-3.,15.:90.3";
	vecStr[1] = "-5.4:2.3,-4.21:-3.";

	warnLog = parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	ASSERT_LT(0, warnLog.size());
}

TEST(FuncCSParser, unsupportedMissing) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2;
	Index nObs = 2;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<FunctionCS> vecInd;

	vecStr[0] = "-8.:24.,13.:-3.,15.:[90.3:104.]";
	vecStr[1] = "-5.4:2.3,-4.21:-3.";

	warnLog = parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	ASSERT_LT(0, warnLog.size());
}

TEST(FuncCSParser, verifyValues) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2;
	Index nObs = 1;
	Index nTime = 3;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<FunctionCS> vecInd;

	vecStr[0] = "-8.:24.,13.:-3.,15.:90.3";

	parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	Vector<Real> tExpected(nTime);
	tExpected << -8., 13., 15.;
	Vector<Real> xExpected(nTime);
	xExpected << 24., -3., 90.3;

	ASSERT_EQ(tExpected, vecInd(0).t());
	ASSERT_EQ(xExpected, vecInd(0).x());
}
