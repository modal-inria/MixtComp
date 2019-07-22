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
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

//TEST(Categorical_pjk, OKSample) {
//	Index nbClass = 2;
//	Index nbInd = 4;
//	Index nbModality = 2;
//
//	Vector<Real> param(nbModality * nbClass);
//	param << 0.2, 0.8, 0.7, 0.3;
//
//	Vector<Index> z(nbInd);
//	z << 0, 1, 0, 1;
//
//	Vector<std::set<Index> > classInd(nbClass);
//	classInd(0) = {0, 2};
//	classInd(1) = {1, 3};
//
//	Vector<int> data(nbInd);
//	data << 0, 0, 1, 1;
//
//	std::string warnLog;
//
//	Poisson_k mixture("Poisson", nbClass, param, classInd);
//
//	std::string dummyStr;
//	mixture.setData(dummyStr, data);
//	int proba = mixture.checkSampleCondition(&warnLog);
//
//#ifdef MC_DEBUG
//	std::cout << "warnLog: " << warnLog << std::endl;
//#endif
//
//	ASSERT_EQ(proba, 1);
//}

//
//TEST(Categorical_pjk, MissingModality) {
//  int nbClass = 2;
//  int nbInd = 4;
//  int nbModality = 2;
//
//  Vector<Real> param(nbModality * nbClass);
//  param << 0.2, 0.8,
//           0.7, 0.3;
//
//  Vector<Index> z(nbInd);
//  z << 0, 1, 0, 1;
//
//  Vector<std::set<Index> > classInd(nbClass);
//  classInd(0) = {0, 2};
//  classInd(1) = {1, 3};
//
//  Vector<int> data(nbInd);
//  data << 0, 0, 0, 0;
//
//  std::string warnLog;
//
//  Poisson_k mixture("Poisson",
//                    nbClass,
//                    param,
//                    classInd);
//
//  mixture.setData(data);
//  int proba = mixture.checkSampleCondition(&warnLog);
//
//#ifdef MC_DEBUG
//  std::cout << "warnLog: " << warnLog << std::endl;
//#endif
//
//  ASSERT_EQ(proba, 0);
//}
