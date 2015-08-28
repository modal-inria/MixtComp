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
#include <map>

#include "../src/Mixture/Rank/mixt_Rank.h"
#include "../src/Mixture/Rank/mixt_RankFunction.h"
#include "../src/Mixture/Rank/mixt_RankVal.h"
#include "../src/LinAlg/mixt_Math.h"

using namespace mixt;

/** Test RankVal::switchRepresentation using the property that it is an involution */
TEST(Rank, switchRepresentation)
{
  int nbPos = 4;
  int nbSample = 1000;

  RankVal mu;
  mu.setNbPos(nbPos);

  MultinomialStatistic multi;

  Vector<bool> involution(nbSample);

  Vector<int> vec(nbPos);
  for (int p = 0; p < nbPos; ++p)
  {
    vec(p) = p;
  }
  mu.setO(vec);

  Vector<int> vecOut(nbPos);

  for (int i = 0; i < nbSample; ++i)
  {
    multi.shuffle(vec); // sample a new individual and assign it as an ordering representation
    mu.setO(vec);

    mu.switchRepresentation(mu.r(), vecOut);

    involution(i) = ((vec == vecOut) ? true : false);

#ifdef MC_DEBUG
    std::cout << "mu  : " << mu  .transpose() << std::endl;
    std::cout << "muP : " << muP .transpose() << std::endl;
    std::cout << "muPP: " << muPP.transpose() << std::endl;
#endif
  }

  ASSERT_EQ(involution, Vector<bool>(nbSample, true));
}

/** Test xGen, using the fact that for pi = 1, mu will be sampled no matter the presentation order */
TEST(Rank, xGen)
{
  int nbPos = 4;
  int nbSample = 100;

  Vector<bool> sorted(nbSample);
  Rank rank(nbPos); // rank which will be completed multiple time
  RankVal RankOut(nbPos); // whill store the result of xGen

  Vector<int> muVec(nbPos); // position -> modality representation
  muVec << 0, 3, 1, 2;
  RankVal mu(nbPos);
  mu.setO(muVec);
  Real pi = 1.; // pi high enough to get mu, no matter the y obtained in removeMissing

  for (int i = 0; i < nbSample; ++i)
  {
    rank.removeMissing(); // shuffle the presentation order
    rank.xGen(mu, pi);
    RankOut = rank.getX();

    (RankOut == mu) ? (sorted(i) = true) : (sorted(i) = false);
  }

  ASSERT_EQ(sorted, Vector<bool>(nbSample, true));
}

TEST(Rank, lnCompletedProbability)
{
  int nbPos = 6;
  int nbSample = 100;

  Real tolerance = 1e-8;
  Vector<bool> sameProba(nbSample);

  Matrix<Real> proba(nbSample, 2);

  Vector<int> muVec (nbPos); // position -> modality representation
  muVec << 4, 0, 3, 5, 1, 2;
  RankVal mu(nbPos);
  mu.setO(muVec);
  Real pi = 0.3;

  Rank rank;
  rank.setNbPos(nbPos);

  for (int i = 0; i < nbSample; ++i)
  {
    rank.removeMissing();
    proba(i, 0) = rank.xGen(mu, pi);
    proba(i, 1) = rank.   lnCompletedProbability(mu, pi);

    (std::abs(proba(i, 0) - proba(i, 1)) < tolerance) ? (sameProba(i) = true) : (sameProba(i) = false);
  }

#ifdef MC_DEBUG
  std::cout << "proba: " << std::endl;
  std::cout << proba << std::endl;
#endif

  ASSERT_EQ(sameProba, Vector<bool>(nbSample, true));
}

/** Test RankVal::permutation, by doing multiple permutations and checking that ordering and ranking
 * still coincide (using involution property, as in switchRepresentation test) */
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

/** Test if the operator < between matrices of same size is correctly implemented */
TEST(Matrix, comparison)
{
  Vector<bool> res(3);
  Matrix<int> a(2, 3);
  Matrix<int> b(2, 3);

  // case where one element in b is smaller than one element in a
  a << 1, 3, 7,
       13, 15, 21;
  b << 1, 3, 5,
       13, 15, 21;
  res(0) = b < a;

  // case where one element in a is smaller than one element in b
  a << 1, 2, 7,
       13, 15, 21;
  b << 1, 3, 5,
       13, 15, 21;
  res(1) = a < b;

  // case when the two matrices are equal
  a << 1, 2, 7,
       13, 15, 21;
  b << 1, 2, 5,
       13, 15, 21;
  res(2) = !(a < b);

  ASSERT_EQ(res, Vector<bool>(3, true));
}

TEST(RankFunction, index2Vector)
{
  int nbMod = 4;
  int nbInd = fac(nbMod);

  std::set<int> remainingMod;
  for (int m = 0; m < nbMod; ++m)
  {
    remainingMod.insert(m);
  }

  Vector<std::pair<Vector<int>, Real> > res(nbInd);
  Vector<int> vec(nbMod);

  RankFunction::index2Vector(res,
                             vec,
                             remainingMod,
                             0,
                             nbInd,
                             0,
                             nbMod);

  bool orderOK = true;
  for (int i = 0; i < nbInd - 1; ++i)
  {
    if (!(res(i).first < res(i + 1).first))
    {
      orderOK = false;
      break;
    }
  }

  ASSERT_TRUE(orderOK);
}

///** Test if the distribution of Y obtained using Gibbs sampling is significantly different to the real distribution
// * obtained through direct computation. Kullback–Leibler_divergence is used to quantify the difference. */
//TEST(Rank, gibbsY)
//{
//  int nbPos = 5;
//  int nbIterBurnIn = 500;
//  int nbIterRun    = 50000;
//
//  Vector<int> dummyVec(nbPos);
//
//  Rank rank(nbPos);
//  dummyVec << 2, 4, 3, 1, 0;
//  rank.setO(dummyVec); // set observed value x
//  rank.removeMissing(); // initialize y_ randomly
//
//  RankVal mu(nbPos);
//  dummyVec << 0, 3, 1, 2, 4;
//  mu.setO(dummyVec);
//
//  Real pi = 0.3;
//
//  std::map<Vector<int>, int> nbOccur; // sparse representation of the number of occurences of each presentation order
//
//  for (int i = 0; i < nbIterBurnIn; ++i)
//  {
//    rank.samplingY(mu, pi);
//  }
//
//  for (int i = 0; i < nbIterRun; ++i)
//  {
//    rank.samplingY(mu, pi);
//    dummyVec = rank.getY();
//
//    if (nbOccur.find(dummyVec) == nbOccur.end())
//    {
//      nbOccur[dummyVec] = 1; // first occurence
//    } else
//    {
//      nbOccur[dummyVec] += 1;
//    }
//
//#ifdef MC_DEBUG
//    std::cout << "i: " << i << ", dummyVec: " << dummyVec.transpose() << std::endl;
//#endif
//  }
//
//  // computation of the
//  for (int a = 0;)
//
//#ifdef MC_DEBUG
//  for (std::map<Vector<int>, int>::const_iterator it = nbOccur.begin();
//       it != nbOccur.end();
//       ++it)
//  {
//    std::cout << "rank: " << it->first.transpose() << " , nb occurences: " << it->second << std::endl;
//  }
//#endif
//}
