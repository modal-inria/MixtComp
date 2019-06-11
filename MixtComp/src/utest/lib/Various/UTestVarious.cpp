/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 2, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(itString, simpleTest)
{
  Vector<Real> a(3);
  a << 0.2, 0.7, 0.3;

  ASSERT_EQ(itString(a), "0.2 0.7 0.3");
}
