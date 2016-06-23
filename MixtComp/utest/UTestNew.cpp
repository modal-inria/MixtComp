/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

using namespace mixt;

/**
 * Simple case with three modalities and imprecision
 */
//TEST(BOSPath, computeLogProba1)
//{
//  int mu = 1; // mode
//  Real pi = 0.5; // precision
//
//  BOSPath path;
//  path.setInit(0, 2);
//  path.setEnd(0, 0);
//
//  Vector<int, 2> eInit;
//  eInit << 0, 2;
//
//  Vector<BOSNode> c(2);
//
//  c(0).y_ = 1; // y, middle element y picked, proba 1./3.
//  c(0).z_ = 0; // z, comparison is imperfect, proba 0.5
//  c(0).partition(eInit); // computation of the partition
//  c(0).e_ << 0, 0; // e, left segment {0, 0} selected, proba 0.33 (all have the same size)
//
//  c(1).y_ = 0; // y, only one element to choose from, proba 1.
//  c(1).z_ = 1; // z, comparison is perfect, proba 0.5
//  c(1).partition(path.c()(0).e_); // computation of the partition
//  c(1).e_ << 0, 0; // e, only one segment in partition, with proba 1.
//
//  path.setC(c);
//
//  Real expectedProba = std::log(1./3. * 0.5 * 1./3. *
//                                1.    * 0.5 * 1.);
//  Real computedProba = path.computeLogProba(mu, pi);
//
//#ifdef MC_DEBUG
//  std::cout << "expectedProba: " << expectedProba << std::endl;
//  std::cout << "proba: " << computedProba << std::endl;
//#endif
//  ASSERT_LT(std::abs(expectedProba - computedProba), epsilon);
//}
