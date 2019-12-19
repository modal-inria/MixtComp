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
 *  Created on: Dec 19, 2019
 *  Author:     Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(ZClassInd, setZi) {
	std::vector<std::string> zi (10, "2");
	for(int i = 0; i < 5; ++i)
		zi[i] = "1";

	ZClassInd zclass;
	zclass.setIndClass(10, 2);

	std::string warnLog;
	warnLog += zclass.setZi(zi);


	ASSERT_EQ(warnLog.size(), 0);
}


TEST(ZClassInd, setZiDetectsWrongNumberOfClasses) {
	std::vector<std::string> zi (10, "2");
	for(int i = 0; i < 5; ++i)
		zi[i] = "1";

	ZClassInd zclass;
	zclass.setIndClass(10, 1);

	std::string warnLog;
	warnLog += zclass.setZi(zi);

	ASSERT_GT(warnLog.size(), 0);
}


TEST(ZClassInd, setZiDetectsWrongClassIndex) {
	std::vector<std::string> zi (10, "2");
	for(int i = 0; i < 5; ++i)
		zi[i] = "-1";

	ZClassInd zclass;
	zclass.setIndClass(10, 2);

	std::string warnLog;
	warnLog += zclass.setZi(zi);

	ASSERT_GT(warnLog.size(), 0);
}


