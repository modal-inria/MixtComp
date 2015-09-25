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

#include "../../src/Mixture/Rank/mixt_RankVal.h"
#include "../../src/LinAlg/mixt_Math.h"

#include "../../src/Mixture/Rank/mixt_RankClass.h"
#include "../../src/Mixture/Rank/mixt_RankIndividual.h"

using namespace mixt;

/** Test RankVal::switchRepresentation using the property that it is an involution */
TEST(RankIndividual, switchRepresentation)
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

/** Test xGen, using the fact that for pi = 0.5 the distribution should be uniform. Kullback-Leibler divergence of uniform distribution from empirical distribution */
TEST(RankIndividual, xGenP05)
{
  int nbPos = 4;
  int nbSample = 1000000;
  Real tolerance = 1.e-4;
  int nbE = fac(nbPos);
  Real logProba = - std::log(nbE);

  RankIndividual rank(nbPos); // rank which will be completed multiple time
  Vector<RankVal> RankOut(nbPos); // whill store the result of xGen

  Vector<int> muVec(nbPos); // position -> modality representation
  muVec << 0, 3, 1, 2;
  RankVal mu(nbPos);
  mu.setO(muVec);
  Real pi = 0.5; // pi high enough to get mu, no matter the y obtained in removeMissing

  std::map<Vector<int>, Real> empDist; // empirical distribution
  Vector<int> tempRank(nbPos);

  for (int i = 0; i < nbSample; ++i)
  {
    rank.removeMissing(); // shuffle the presentation order
    rank.xGen(mu, pi);
    tempRank = rank.x().o();

    if (empDist.find(tempRank) == empDist.end())
    {
      empDist[tempRank] = 1.; // first occurrence
    } else
    {
      empDist[tempRank] += 1.;
    }
  }

  for (std::map<Vector<int>, Real>::iterator it = empDist.begin();
       it != empDist.end();
       ++it) // renormalization of the empirical distribution
  {
    it->second /= nbSample;
  }

#ifdef MC_DEBUG
  for (std::map<Vector<int>, Real>::iterator it = empDist.begin();
       it != empDist.end();
       ++it) // renormalization of the empirical distribution
  {
    std::cout << "vec: " << it->first.transpose() << ", proba: " << it->second << std::endl;
  }
#endif

  Real KLDivergence = 0.; // computation of the Kullback–Leibler_divergence
  for (std::map<Vector<int>, Real>::iterator it = empDist.begin();
       it != empDist.end();
       ++it)
  {
#ifdef MC_DEBUG
    std::cout << "vec: " << it->first.transpose() << ", empDist: " << it->second << ", calcDist: " << calcDist[it->first] << std::endl;
#endif

    KLDivergence += it->second * (std::log(it->second) - logProba);
  }

#ifdef MC_DEBUG
  std::cout << "KLDivergence: " << KLDivergence << std::endl;
#endif

  ASSERT_LT(KLDivergence, tolerance);
  ASSERT_EQ(empDist.size(), nbE);
}

/** Test xGen, using the fact that for pi = 1, mu will be sampled no matter the presentation order */
TEST(RankIndividual, xGenP1)
{
  int nbPos = 4;
  int nbSample = 100;

  Vector<bool> sorted(nbSample);
  RankIndividual rank(nbPos); // rank which will be completed multiple time
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
    RankOut = rank.x();

    (RankOut == mu) ? (sorted(i) = true) : (sorted(i) = false);
  }

  ASSERT_EQ(sorted, Vector<bool>(nbSample, true));
}

/** Computation of the joint probability p(x, y). Compare the probability obtained through direct computation with
 * the proba obtained during the xGen sampling of x. */
TEST(RankIndividual, lnCompletedProbability)
{
  int nbPos = 6;
  int nbSample = 100;

  Real tolerance = 1e-8;
  Vector<bool> sameProba(nbSample);

  Matrix<Real> proba(nbSample, 2);

  Vector<int> muVec (nbPos);
  muVec << 4, 0, 3, 5, 1, 2;
  RankVal mu(nbPos);
  mu.setO(muVec);
  Real pi = 0.3;

  RankIndividual rank;
  rank.setNbPos(nbPos);

  int a, g; // dummy variables

  for (int i = 0; i < nbSample; ++i)
  {
    rank.removeMissing();
    proba(i, 0) = rank.xGen(mu, pi);
    proba(i, 1) = rank.lnCompletedProbability(mu, pi, a, g);

    (std::abs(proba(i, 0) - proba(i, 1)) < tolerance) ? (sameProba(i) = true) : (sameProba(i) = false);
  }

#ifdef MC_DEBUG
  std::cout << "proba: " << std::endl;
  std::cout << proba << std::endl;
#endif

  ASSERT_EQ(sameProba, Vector<bool>(nbSample, true));
}

/** Estimate the observed probability distribution using independent samples. Compare the mode to the real
 * parameter. */
TEST(RankIndividual, observedProba)
{
  int nbPos = 6;
  RankVal mu = {5, 1, 0, 4, 2, 3};
  Real pi = 0.87;

  RankIndividual rv(nbPos);
  std::map<RankVal, Real> proba;

  rv.observedProba(mu, pi, proba);

  RankVal muEst = proba.begin()->first;
  Real probaEst = proba.begin()->second;

  for (std::map<RankVal, Real>::const_iterator it = proba.begin(), itEnd = proba.end();
       it != itEnd;
       ++it)
  {
#ifdef MC_DEBUG
    std::cout << "RankVal: " << it->first << ", proba: " << it->second << std::endl;
#endif
    if (probaEst < it->second)
    {
      muEst = it->first;
      probaEst = it->second;
    }
  }

#ifdef MC_DEBUG
    std::cout << "muEst: " << muEst << ", probaEst: " << probaEst << std::endl;
#endif

    ASSERT_EQ(mu, muEst);
}
