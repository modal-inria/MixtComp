/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

//TEST(Poisson_k, OKSample) {
//  int nbClass = 2;
//  int nbInd = 4;
//
//  Vector<Real> param(nbClass);
//  param << 3, 1;
//
//  Vector<std::set<Index> > classInd(nbClass);
//  classInd(0) = {0, 2};
//  classInd(1) = {1, 3};
//
//  Vector<int> data(nbInd);
//  data << 0, 1, 5, 6;
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
//  ASSERT_EQ(proba, 1);
//}
//
//TEST(Poisson_k, 0OnlyClass) {
//  int nbClass = 2;
//  int nbInd = 4;
//
//  Vector<Real> param(nbClass);
//  param << 3, 1;
//
//  Vector<std::set<Index> > classInd(nbClass);
//  classInd(0) = {0, 2};
//  classInd(1) = {1, 3};
//
//  Vector<int> data(nbInd);
//  data << 0, 1, 0, 6;
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
