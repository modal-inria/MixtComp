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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

using namespace mixt;

typedef typename std::pair<MisType, std::vector<int> > MisVal;

TEST(Vector, importIterable) {
  std::list<Real> list;
  list.push_back(3.);
  list.push_back(12.);
  list.push_back(5.);

  Vector<Real> vec;
  vec.copyIterable(list);

  Vector<Real> expectedVec(3);
  expectedVec << 3., 12., 5.;

  ASSERT_EQ(vec, expectedVec);
}

/** This test performs forward sampling using computeObservedProba, then Gibbs sampling using sampleY and sampleX.
 * The density probability on X for a partially observed individual are then compared via Kullback–Leibler divergence. */
TEST(RankIndividual, sampleX) {
  int nbPos = 5;
  int nbIterationBurnIn = 1000;
  int nbIteration = 1000;
  Real errorTolerance = 1e-5;

  Vector<int> muVec(nbPos);
  muVec << 0, 1, 2, 3, 4;
  RankVal mu(nbPos);
  mu.setO(muVec);

  Real pi = 0.8;

  RankIndividual rankIndividual(nbPos);
  Vector<int> xVec(nbPos);
  xVec << 3, 4, 2, 0, 1;
  Vector<MisVal> obsData(nbPos);
  obsData(0) = MisVal(present_, {});
  obsData(1) = MisVal(missingFiniteValues_, {2, 4});
  obsData(2) = MisVal(missingFiniteValues_, {2, 4});
  obsData(3) = MisVal(missing_, {});
  obsData(4) = MisVal(present_, {});
  rankIndividual.removeMissing();

  rankIndividual.setO(xVec);
  rankIndividual.setObsData(obsData);

  std::map<RankVal, Real> observedProba;

  /* This computation does not take xVec nor obsData information into account,
   * it is the observed proba of a completely missing individual. */
  RankIndividual rankIndSampler(nbPos);
  rankIndSampler.observedProba(mu,
                               pi,
                               observedProba);

  std::list<RankVal> listCompleted = rankIndividual.enumCompleted();
  Vector<Real> filteredProba(listCompleted.size());

  int currProba = 0;
  for (std::list<RankVal>::const_iterator it = listCompleted.begin(), itE = listCompleted.end();
       it != itE;
       ++it, ++currProba) {
    filteredProba(currProba) = observedProba[*it];
  }

  Vector<Real> dummyVec;
  Real lnObservedDirectSampling = std::log(dummyVec.logToMulti(filteredProba));

  for (int i = 0; i < nbIterationBurnIn; ++i) {
    rankIndividual.sampleY(mu, pi);
    rankIndividual.sampleX(mu, pi);
  }

  for (int i = 0; i < nbIteration; ++i) {
    rankIndividual.sampleY(mu, pi);
    rankIndividual.sampleX(mu, pi);
  }

  Real lnObservedGibbsSampling = 12.;

#ifdef MC_DEBUG
  std::cout << "filteredProba: " << itString(filteredProba) << ", lnObservedDirectSampling: " << lnObservedDirectSampling << std::endl;
#endif

  ASSERT_LT(std::abs(lnObservedDirectSampling - lnObservedGibbsSampling), errorTolerance);
}

/** Do Gibbs with everything missing and check that log observed proba is 0. */
