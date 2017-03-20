/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/** Use RankParamStat to compute statistics on mu and pi. Use the setExpectationParam to make a comparison
 * with the real parameters */
TEST(RankStat, computeStat)
{
  int nbPos = 5;
  int nbInd = 500;
  int nbIterburnIn = 500;
  int nbIterRun = 500;
  Real confidenceLevel = 0.95;

  MultinomialStatistic multi;
  UniformStatistic uni;

  RankIndividual rankIndividual(nbPos); // rank which will be completed multiple time
  Vector<MisVal> obsData(nbPos, MisVal(missing_, {}));
  rankIndividual.setObsData(obsData);

  Vector<RankIndividual> data(nbInd); // will store the result of xGen
  std::set<Index> setInd;

  RankVal mu = {0, 3, 1, 2, 4}; // ordering (position -> modality) representation
  Real pi = 0.75;

  for (int i = 0; i < nbInd; ++i) {
    data(i) = rankIndividual;

    data(i).removeMissing(); // shuffle the presentation order, to get the correct marginal distribution corresponding to (mu, pi)
    data(i).xGen(mu, pi);

    setInd.insert(i);
  }

  Vector<int> muVec(nbPos);
  std::iota(muVec.begin(), muVec.end(), 0);
  multi.shuffle(muVec); // randomly initialized
  RankVal muEst(nbPos); // estimated mu
  muEst.setO(muVec);
  Real piEst = uni.sample(0.5, 1.); // estimated pi randomly initialized too

  RankStat paramStat(muEst, confidenceLevel); // stat computer on muEst and piEst

  RankClass rank(data,
                 setInd,
                 muEst,
                 piEst);

  for (int i = 0; i < nbIterburnIn; ++i) {
    for (int ind = 0; ind < nbInd; ++ind) {
      data(ind).sampleY(muEst, piEst, allGAuthorized_);
    }
    rank.sampleMu();
  }

  for (int i = 0; i < nbIterRun; ++i) {
    for (int ind = 0; ind < nbInd; ++ind) {
      data(ind).sampleY(muEst, piEst, allGAuthorized_);
    }
    rank.mStep(unBiased_);
    paramStat.sampleValue(i, nbIterRun - 1);
  }

  paramStat.setExpectationParam();

  ASSERT_EQ(mu, muEst);
}
