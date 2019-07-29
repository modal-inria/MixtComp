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
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(RankVal, initializerConstructor)
{
  RankVal rv = {0, 1, 3, 2};
  Vector<int> vecExpected(4);
  vecExpected << 0, 1, 3, 2;

  ASSERT_EQ(rv.o(), vecExpected);
}

/** Test RankVal::permutation, by doing multiple permutations and checking that ordering and ranking
 * still coincide (using involution property, as in RankIndividual switchRepresentation test) */
TEST(RankVal, permutation)
{
  int nbPos = 10;
  int nbPer = 100; // number of permutations to be tested
  int nbSample = 1000;

  Vector<bool> res(nbSample);

  MultinomialStatistic multi;
  RankVal rv;
  rv.setNbPos(nbPos);

  Vector<int> rank(nbPos);
  for (int m = 0; m < nbPos; ++m)
  {
    rank(m) = m;
  }
  rv.setO(rank);

  for (int i = 0; i < nbSample; ++i)
  {
    for (int p = 0; p < nbPer; ++p)
    {
      rv.permutation(multi.sampleInt(0, nbPos - 2));
    }
    Vector<int> oP(nbPos);
    rv.switchRepresentation(rv.o(), oP);
    res(i) = (oP == rv.r());
  }

  ASSERT_EQ(res, Vector<bool>(nbSample, true));
}
