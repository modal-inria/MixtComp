/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 22, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(FunctionalParser, validData) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2; // useless as well
	Index nObs = 2;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<Function> vecInd;

	vecStr[0] = "12.:24.,13.:-3.,15.:90.3";
	vecStr[1] = "-5.4:2.3,-4.21:-3.";

	warnLog = parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	ASSERT_EQ(0, warnLog.size());
}

TEST(FunctionalParser, timeMissing) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2; // useless as well
	Index nObs = 2;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<Function> vecInd;

	vecStr[0] = ":24.,13.:-3.,15.:90.3";
	vecStr[1] = "-5.4:2.3,-4.21:-3.";

	warnLog = parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	ASSERT_LT(0, warnLog.size());
}

TEST(FunctionalParser, unsupportedMissing) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2;
	Index nObs = 2;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<Function> vecInd;

	vecStr[0] = "-8.:24.,13.:-3.,15.:[90.3:104.]";
	vecStr[1] = "-5.4:2.3,-4.21:-3.";

	warnLog = parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	ASSERT_LT(0, warnLog.size());
}

TEST(FunctionalParser, verifyValues) {
	Index nSub = 2; // useless in this context, but used in observation's setVal
	Index nCoeff = 2;
	Index nObs = 1;
	Index nTime = 3;

	std::string warnLog;

	std::vector<std::string> vecStr(nObs);
	Vector<Function> vecInd;

	vecStr[0] = "-8.:24.,13.:-3.,15.:90.3";

	parseFunctionalStr(nSub, nCoeff, vecStr, vecInd);

	Vector<Real> tExpected(nTime);
	tExpected << -8., 13., 15.;
	Vector<Real> xExpected(nTime);
	xExpected << 24., -3., 90.3;

	ASSERT_EQ(tExpected, vecInd(0).t());
	ASSERT_EQ(xExpected, vecInd(0).x());
}
