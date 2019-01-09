/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
