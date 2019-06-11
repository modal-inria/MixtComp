/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/* This is not really a file to test the standard library, but rather to provide example of its use in MixtComp, on
 * simple, isolated cases. */

TEST(set, intersection)
{
  std::set<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);
  s.insert(12);

  std::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  v.push_back(4);

  std::list<int> intersection;

  std::set_intersection(s.begin(),
                        s.end(),
                        v.begin(),
                        v.end(),
                        std::back_inserter(intersection));

  ASSERT_EQ(itString(intersection), "1 3");
}
