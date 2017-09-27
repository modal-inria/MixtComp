/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/**
 * Computation of a partition
 */
TEST(BOSNode, partition0)
{
  Vector<int, 2> e;
  e << 0, 5;
  BOSNode node;
  node.y_ = 2;
  Vector<Vector<int, 2> > partExpected(3);
  partExpected(0) << 0, 1;
  partExpected(1) << 2, 2;
  partExpected(2) << 3, 5;

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Computation of a partition
 */
TEST(BOSNode, partition1)
{
  Vector<int, 2> e;
  e << 6, 8;
  BOSNode node;
  node.y_ = 2;
  Vector<Vector<int, 2> > partExpected(0);

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Computation of a partition
 */
TEST(BOSNode, partition2)
{
  Vector<int, 2> e;
  e << 6, 8;
  BOSNode node;
  node.y_ = 6;
  Vector<Vector<int, 2> > partExpected(2);
  partExpected(0) << 6, 6;
  partExpected(1) << 7, 8;

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}

/**
 * Computation of a partition
 */
TEST(BOSNode, partition3)
{
  Vector<int, 2> e;
  e << 6, 8;
  BOSNode node;
  node.y_ = 8;
  Vector<Vector<int, 2> > partExpected(2);
  partExpected(0) << 6, 7;
  partExpected(1) << 8, 8;

  node.partition(e);
  ASSERT_EQ(node.part_, partExpected); // has the real mode been estimated correctly ?
}
