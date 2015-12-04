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
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "../../src/mixt_MixtComp.h"

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
