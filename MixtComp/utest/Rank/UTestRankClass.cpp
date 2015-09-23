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

/** Test if Rank::probaYgX (direct computation of distribution of Y / X) has produced a correct list of Y candidate using < comparator on consecutive values of result.
 * Actual value of the conditional probability is not tested, but the lnCompletedProbability used in its computation
 * already is tested elsewhere. */
TEST(RankClass, probaYgX)
{
  int nbPos = 4;
  int nbE = fac(nbPos);

  RankIndividual rank(nbPos);

  Vector<int> muVec(nbPos);
  muVec << 0, 3, 1, 2;
  RankVal mu(nbPos);

  Real pi = 0.3;

  Vector<Vector<int> > resVec(nbE);
  Vector<Real> resProba(nbE);

  rank.probaYgX(mu,
                pi,
                resVec,
                resProba);

  bool sorted = true;
  for (int i = 0; i < nbE - 1; ++i)
  {
    if (!(resVec(i) <  resVec(i + 1)))
    {
      sorted = false;
      break;
    }
  }

#ifdef MC_DEBUG
  for (int i = 0; i < nbE; ++i)
  {
    std::cout << "i: " << i << ", vec: " << resVec(i).transpose() << ", proba: " << resProba(i) << std::endl;
  }
#endif

  EXPECT_TRUE(sorted);
}

/** Test if the conditional distribution p(y / x) obtained using Gibbs sampling is significantly different from the distribution
 * obtained through direct computation. Kullback–Leibler_divergence quantifies this difference. */
TEST(RankClass, gibbsY)
{
  int nbPos = 5;
  int nbIterBurnIn = 500;
  int nbIterRun    = 500000;
  Real tolerance = 1.e-3;
  int nbE = fac(nbPos); // number of random events

  Vector<int> dummyVec(nbPos);

  RankIndividual rank(nbPos);
  dummyVec << 2, 4, 3, 1, 0;
  rank.setO(dummyVec); // set observed value x
  rank.removeMissing(); // initialize y_ randomly

  RankVal mu(nbPos);
  dummyVec << 0, 3, 1, 2, 4;
  mu.setO(dummyVec);

  Real pi = 0.3;

  std::map<Vector<int>, Real> empDist; // empirical distribution
  std::map<Vector<int>, Real> calcDist; // calculated distribution

  for (int i = 0; i < nbIterBurnIn; ++i)
  {
    rank.sampleY(mu, pi);
  }

  for (int i = 0; i < nbIterRun; ++i)
  {
    rank.sampleY(mu, pi);
    dummyVec = rank.getY();

    if (empDist.find(dummyVec) == empDist.end())
    {
      empDist[dummyVec] = 1.; // first occurrence
    } else
    {
      empDist[dummyVec] += 1.;
    }

#ifdef MC_DEBUG
    std::cout << "i: " << i << ", dummyVec: " << dummyVec.transpose() << std::endl;
#endif
  }

  for (std::map<Vector<int>, Real>::iterator it = empDist.begin();
       it != empDist.end();
       ++it) // renormalization of the empirical distribution
  {
    it->second /= nbIterRun;
  }

#ifdef MC_DEBUG
  for (std::map<Vector<int>, int>::const_iterator it = empDist.begin();
       it != empDist.end();
       ++it)
  {
    std::cout << "rank: " << it->first.transpose() << " , nb occurences: " << it->second << std::endl;
  }
#endif

  Vector<Vector<int> > resVec(nbE);
  Vector<Real> resProba(nbE);
  rank.probaYgX(mu,
                pi,
                resVec,
                resProba);
  for (int e = 0; e < nbE; ++e) // filling the map object used to compute the KL-divergence
  {
    calcDist[resVec(e)] = resProba(e);
  }

  Real KLDivergence = 0.; // computation of the Kullback–Leibler_divergence
  for (std::map<Vector<int>, Real>::iterator it = empDist.begin();
       it != empDist.end();
       ++it)
  {
#ifdef MC_DEBUG
    std::cout << "vec: " << it->first.transpose() << ", empDist: " << it->second << ", calcDist: " << calcDist[it->first] << std::endl;
#endif

    KLDivergence += it->second * (std::log(it->second) - std::log(calcDist[it->first]));
  }

#ifdef MC_DEBUG
  std::cout << "KLDivergence: " << KLDivergence << std::endl;
#endif

  ASSERT_LT(KLDivergence, tolerance);
}

/** Test sampleMu by first generating individuals, then performing sampleMu. The test checks if the real mu has been sampled at least once. */
TEST(RankClass, sampleMu)
{
  int nbPos = 6;
  int nbInd = 500;
  int nbIterburnIn = 500;
  int nbIterRun = 500;

  std::set<RankVal> sampledResult; // store the sampled ranks

  MultinomialStatistic multi;

  RankIndividual rankIndividual(nbPos); // rank which will be completed multiple time
  Vector<RankIndividual> data(nbInd); // will store the result of xGen

  RankVal mu = {0, 3, 1, 2, 5, 4}; // position -> modality representation
  Real pi = 0.3; // pi high enough to get mu, no matter the y obtained in removeMissing

  for (int i = 0; i < nbInd; ++i)
  {
    rankIndividual.removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
    rankIndividual.xGen(mu, pi);
    data(i) = rankIndividual;
  }

  Vector<int> muVec(nbPos);
  std::iota(muVec.begin(), muVec.end(), 0);
  multi.shuffle(muVec);
  RankVal muEst(nbPos); // estimated mu is randomly initialized
  muEst.setO(muVec);

#ifdef MC_DEBUG
    std::cout << "muVec: " << muVec.transpose() << std::endl;
#endif

  RankClass rank(data,
                 muEst,
                 pi);

  for (int i = 0; i < nbInd; ++i)
  {
    data(i).removeMissing();
  }

  for (int i = 0; i < nbIterburnIn; ++i)
  {
    rank.sampleMu();
  }

  for (int i = 0; i < nbIterRun; ++i)
  {
    rank.sampleMu();
    sampledResult.insert(muEst);
  }

  ASSERT_TRUE(sampledResult.find(mu) != sampledResult.end());
}

/** Test sampleMu by first generating individuals, then performing sampleMu. The test checks if the real mu has been sampled at least once. */
TEST(RankClass, mStep)
{
  int nbPos = 7;
  int nbInd = 500;
  int nbIterburnIn = 500;
  Real tolerance = 0.05;

  std::set<RankVal> sampledResult; // store the sampled ranks

  MultinomialStatistic multi;
  UniformStatistic uni;

  RankIndividual rankIndividual(nbPos); // rank which will be completed multiple time
  Vector<RankIndividual> data(nbInd); // will store the result of xGen



  RankVal mu = {0, 3, 1, 2, 6, 5, 4}; // position -> modality representation
  Real pi = 0.75;

  for (int i = 0; i < nbInd; ++i)
  {
    rankIndividual.removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
    rankIndividual.xGen(mu, pi);
    data(i) = rankIndividual;

#ifdef MC_DEBUG
    std::cout << "data(i): " << data(i).transpose() << std::endl;
#endif
  }

  Vector<int> muVec(nbPos);
  std::iota(muVec.begin(), muVec.end(), 0);
  multi.shuffle(muVec); // randomly initialized
  RankVal muEst(nbPos); // estimated mu
  muEst.setO(muVec);
  Real piEst = uni.sample(0.5, 1.); // estimated pi randomly initialized too

#ifdef MC_DEBUG
    std::cout << "Initialisation: mu: " << muVec.transpose() << ", pi: " << piEst << std::endl;
#endif

  RankClass rank(data,
                 muEst,
                 piEst);

  for (int i = 0; i < nbInd; ++i)
  {
    data(i).removeMissing();
  }

  for (int i = 0; i < nbIterburnIn; ++i)
  {
    for (int ind = 0; ind < nbInd; ++ind)
    {
      data(ind).sampleY(muEst, piEst);
    }
    rank.sampleMu();
  }

  rank.mStep();

#ifdef MC_DEBUG
  std::cout << "Estimation:     mu: " << muEst.o().transpose() << ", pi : " << piEst << std::endl;
#endif

  ASSERT_EQ(mu, muEst);
  ASSERT_LT(std::abs(pi - piEst), tolerance);
}
