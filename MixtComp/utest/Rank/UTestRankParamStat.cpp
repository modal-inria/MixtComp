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

#include "../../src/Mixture/Rank/mixt_RankClass.h"
#include "../Mixture/Rank/mixt_RankIndividual.h"
#include "../Mixture/Rank/mixt_RankParamStat.h"
#include "../Statistic/mixt_Statistic.h"

using namespace mixt;

/** Use RankParamStat to compute statistics on mu and pi. Use the setExpectationParam to make a comparison
 * with the real parameters */
TEST(RankParamStat, computeStat)
{
  int nbPos = 5;
  int nbInd = 500;
  int nbIterburnIn = 500;
  int nbIterRun = 500;
  Real confidenceLevel = 0.95;
  Real tolerance = 0.05;

  MultinomialStatistic multi;
  UniformStatistic uni;

  RankIndividual rankIndividual(nbPos); // rank which will be completed multiple time
  Vector<RankIndividual> data(nbInd); // will store the result of xGen
  std::set<int> setInd;

  RankVal mu = {0, 3, 1, 2, 4}; // ordering (position -> modality) representation
  Real pi = 0.75;

  for (int i = 0; i < nbInd; ++i)
  {
    rankIndividual.removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
    rankIndividual.xGen(mu, pi);
    data(i) = rankIndividual;
    setInd.insert(i);
  }

  Vector<int> muVec(nbPos);
  std::iota(muVec.begin(), muVec.end(), 0);
  multi.shuffle(muVec); // randomly initialized
  RankVal muEst(nbPos); // estimated mu
  muEst.setO(muVec);
  Real piEst = uni.sample(0.5, 1.); // estimated pi randomly initialized too

  RankParamStat paramStat(muEst, confidenceLevel); // stat computer on muEst and piEst

#ifdef MC_DEBUG
    std::cout << "Initialisation: mu: " << muVec.transpose() << ", pi: " << piEst << std::endl;
#endif

  RankClass rank(data,
                 setInd,
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

  for (int i = 0; i < nbIterRun; ++i)
  {
    for (int ind = 0; ind < nbInd; ++ind)
    {
      data(ind).sampleY(muEst, piEst);
    }
    rank.mStep();
    paramStat.sampleParam(i, nbIterRun - 1);
  }

  paramStat.setExpectationParam();

#ifdef MC_DEBUG
  std::cout << "Estimation:     mu: " << muEst.o().transpose() << ", pi : " << piEst << std::endl;
#endif

  ASSERT_EQ(mu, muEst);
}
