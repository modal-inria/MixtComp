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

#include "../src/Mixture/Rank/mixt_Rank.h"
#include "../src/Mixture/Rank/mixt_RankVal.h"

using namespace mixt;

TEST(Rank, switchRepresentation)
{
  int nbPos = 4;
  int nbSample = 1000;

  Rank rank;
  rank.setNbPos(nbPos);
  Vector<int> mu  (nbPos);
  Vector<int> muP (nbPos);
  Vector<int> muPP(nbPos);

  MultinomialStatistic multi;

  bool allCommute = true;

  for (int p = 0; p < nbPos; ++p)
  {
    mu(p) = p;
  }

  for (int i = 0; i < nbSample; ++i)
  {
    multi.shuffle(mu);
    rank.switchRepresentation(mu , muP );
    rank.switchRepresentation(muP, muPP);

    allCommute = ((muPP == mu) ? true : false);

#ifdef MC_DEBUG
    std::cout << "mu  : " << mu  .transpose() << std::endl;
    std::cout << "muP : " << muP .transpose() << std::endl;
    std::cout << "muPP: " << muPP.transpose() << std::endl;
#endif
  }

  ASSERT_EQ(allCommute, true);
}

TEST(Rank, xGen)
{
  int nbPos = 4;
  int nbSample = 100;

  bool allSorted = true;

  Vector<int> mu (nbPos); // position -> modality representation
  mu  << 0, 3, 1, 2;
  Vector<int> muP(nbPos); // modality -> position representation
  muP << 0, 2, 3, 1;

  Real pi = 0.9999; // pi high enough to get mu, no matter the y obtained in removeMissing

  Rank rank;
  rank.setNbPos(nbPos);

  for (int i = 0; i < nbSample; ++i)
  {
    rank.removeMissing(); // reinitialize the presentation order
    rank.xGen(muP, pi);
    Vector<int> x;
    rank.getX(x);

    if (mu != x)
    {
      allSorted = false;
    }

#ifdef MC_DEBUG
    std::cout << "i: " << i << ", lp: " << lp << ", x: " << x.transpose() << std::endl;
#endif
  }

  ASSERT_EQ(allSorted, true);
}

TEST(Rank, lnCompletedProbability)
{
  int nbPos = 6;
  int nbSample = 100;

  Matrix<Real> proba(nbSample, 3);

  Vector<int> mu (nbPos); // position -> modality representation
  mu  << 4, 0, 3, 5, 1, 2;
  Vector<int> muP(nbPos); // modality -> position representation
  mu  << 1, 4, 5, 2, 0, 3;
  Real pi = 0.3;

  Rank rank;
  rank.setNbPos(nbPos);

  for (int i = 0; i < nbSample; ++i)
  {
    rank.removeMissing();
    proba(i, 0) = rank.xGen(muP, pi);
    rank.xSwitch(); // xP_ will be needed in lnCompletedProbability
    proba(i, 1) = rank.   lnCompletedProbability(mu, pi);
    proba(i, 2) = rank.oldLnCompletedProbability(mu, pi);
  }

#ifdef MC_DEBUGNEW
  std::cout << "proba: " << std::endl;
  std::cout << proba << std::endl;
#endif
}

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
