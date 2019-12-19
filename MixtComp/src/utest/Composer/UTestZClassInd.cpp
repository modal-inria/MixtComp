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
	int nClass = 2;
	int nInd = 10;
	zclass.setIndClass(nInd, nClass);

	std::string warnLog;
	warnLog += zclass.setZi(zi);


	ASSERT_EQ(warnLog.size(), 0);

	// check created object
	AugmentedData<Vector<Index> > outZi = zclass.zi();
	EXPECT_FALSE(outZi.dataRange_.hasRange_);
	EXPECT_EQ(outZi.dataRange_.min_, 0);
	EXPECT_EQ(outZi.dataRange_.max_, 1);
	EXPECT_EQ(outZi.dataRange_.range_, nClass);
	ASSERT_EQ(outZi.data_.size(), nInd);
	for(int i = 0; i < 5; ++i)
		EXPECT_EQ(outZi.data_(i), 0);
	for(int i = 5; i < 10; ++i)
		EXPECT_EQ(outZi.data_(i), 1);
	EXPECT_EQ(outZi.misCount_(0), 10);
	for(int i = 1; i < outZi.misCount_.size(); ++i)
		EXPECT_EQ(outZi.misCount_(i), 0);


	Vector<std::set<Index> > classInd = zclass.classInd();
	ASSERT_EQ(classInd.size(), nClass);
	Index i = 0;
	for(auto f : classInd[0]) {
		EXPECT_EQ(f, i);
		i++;
	}
	for(auto f : classInd[1]) {
		EXPECT_EQ(f, i);
		i++;
	}


	// check setters
	zclass.setZAndClassInd(0, 1);
	EXPECT_EQ(zclass.zi().data_[0], 1);

	classInd = zclass.classInd();
	ASSERT_EQ(classInd.size(), nClass);
	i = 1;
	for(auto f : classInd[0]) {
		EXPECT_EQ(f, i);
		i++;
	}
	i = 0;
	for(auto f : classInd[1]) {
		EXPECT_EQ(f, i);
		if(i == 0)
			i = 5;
		else
			i++;
	}

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



TEST(ZClassInd, setAllMissing) {
	ZClassInd zclass;
	int nClass = 2;
	int nInd = 10;
	zclass.setIndClass(nInd, nClass);
	zclass.setAllMissing();
	zclass.setRange(0, 1, 2);

	AugmentedData<Vector<Index> > outZi = zclass.zi();
	EXPECT_EQ(outZi.misData_.size(), 10);
	EXPECT_EQ(outZi.misCount_(0), 0);
	EXPECT_EQ(outZi.misCount_(1), 10);
	for(int i = 2; i < outZi.misCount_.size(); ++i)
		EXPECT_EQ(outZi.misCount_(i), 0);

	EXPECT_EQ(outZi.dataRange_.min_, 0);
	EXPECT_EQ(outZi.dataRange_.max_, 1);
	EXPECT_EQ(outZi.dataRange_.range_, 2);

}


TEST(ZClassInd, checkMissingType) {
	std::vector<std::string> zi (10, "2");
	zi[0] = "1";
	zi[1] = "{1, 2}";
	zi[2] = "?";

	ZClassInd zclass;
	int nClass = 2;
	int nInd = 10;
	zclass.setIndClass(nInd, nClass);

	std::string warnLog;
	warnLog += zclass.setZi(zi);
	ASSERT_EQ(warnLog.size(), 0);

	warnLog += zclass.checkMissingType();
	ASSERT_EQ(warnLog.size(), 0);

	// intevals [a:b]
	zi[3] = "[1:2]";
	std::string warnLog2;

	warnLog2 += zclass.setZi(zi);
	ASSERT_EQ(warnLog.size(), 0);

	warnLog2 += zclass.checkMissingType();
	ASSERT_GT(warnLog2.size(), 0);

	// intevals [-inf:b]
	zi[3] = "[-inf:2]";
	std::string warnLog3;

	warnLog3 += zclass.setZi(zi);
	ASSERT_EQ(warnLog.size(), 0);

	warnLog3 += zclass.checkMissingType();
	ASSERT_GT(warnLog3.size(), 0);

	// intevals [a:+inf]
	zi[3] = "[1:+inf]";
	std::string warnLog4;

	warnLog4 += zclass.setZi(zi);
	ASSERT_EQ(warnLog.size(), 0);

	warnLog4 += zclass.checkMissingType();
	ASSERT_GT(warnLog4.size(), 0);
}


