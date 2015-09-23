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
 *  Created on: September 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

#include "../Mixture/Rank/mixt_RankIndividual.h"
#include "../Mixture/Rank/mixt_Rank.h"
#include "../Mixture/Rank/mixt_RankParamStat.h"
#include "../Statistic/mixt_Statistic.h"

using namespace mixt;

/** Use RankParamStat to compute statistics on  */
TEST(RankParamStat, computeStat)
{
  int nbPos = 5;
  int nbSample = 500;
  int nbIterburnIn = 500;
  int nbIterRun = 500;
  Real confidenceLevel = 0.95;
  Real tolerance = 0.05;

  MultinomialStatistic multi;
  UniformStatistic uni;

  RankIndividual rankIndividual(nbPos); // rank which will be completed multiple time
  Vector<Vector<int> > data(nbSample); // will store the result of xGen

  Vector<int> muVec(nbPos); // ordering (position -> modality) representation
  muVec << 0, 3, 1, 2, 4;
  RankVal mu(nbPos);
  mu.setO(muVec);
  Real pi = 0.75;

  for (int i = 0; i < nbSample; ++i)
  {
    rankIndividual.removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
    rankIndividual.xGen(mu, pi);
    data(i) = rankIndividual.getX().o();
  }

  RankVal muEst(nbPos); // estimated mu
  multi.shuffle(muVec); // randomly initialized
  muEst.setO(muVec);
  Real piEst = uni.sample(0.5, 1.); // estimated pi randomly initialized too

  RankParamStat paramStat(muEst, piEst, confidenceLevel); // stat computer on muEst and piEst
#ifdef MC_DEBUG
    std::cout << "Initialisation: mu: " << muVec.transpose() << ", pi: " << piEst << std::endl;
#endif

  Rank rank(1,
            data,
            muEst,
            piEst);
  rank.removeMissing();

  for (int i = 0; i < nbIterburnIn; ++i)
  {
    for (int ind = 0; ind < nbSample; ++ind)
    {
      rank.samplingStep(ind);
    }
    rank.sampleMu();
  }

  for (int i = 0; i < nbIterRun; ++i)
  {
    for (int ind = 0; ind < nbSample; ++ind)
    {
      rank.samplingStep(ind);
    }
    rank.mStep();
    muEst = rank.getMu();
    piEst = rank.getPi();
    paramStat.sampleParam(i, nbIterRun - 1);
  }

  paramStat.setExpectationParam();

#ifdef MC_DEBUG
  std::cout << "Estimation:     mu: " << muEst.o().transpose() << ", pi : " << piEst << std::endl;
#endif

  ASSERT_EQ(mu, muEst);
  ASSERT_LT(std::abs(pi - piEst), tolerance);
}
