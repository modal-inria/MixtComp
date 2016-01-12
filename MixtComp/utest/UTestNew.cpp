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

/** To test for sampleY and sampleX, a Gibbs sampling is performed on a RankIndividual with partially observed data.
 * The mode of the distribution of completed individual is known and is checked at the end of a Gibbs sampling
 * that calls both sampleY and sampleX. Initialization of the Gibbs is performed randomly using removeMissing.*/
TEST(RankIndividual, sampleX) {
  int nbPos = 5;
  int nbIterationBurnIn = 1000;
  int nbIteration = 1000;

  Vector<int> muVec(nbPos);
  muVec << 0, 1, 2, 3, 4;
  RankVal mu(nbPos);
  mu.setO(muVec);

  Real pi = 0.8;

  RankIndividual rankIndividual(nbPos);
  Vector<int> xVec(nbPos);
  xVec << 0, 1, 2, 3, 4;
  Vector<MisVal> obsData(nbPos);
  obsData(0) = MisVal(missingFiniteValues_, {0, 1});
  obsData(1) = MisVal(missingFiniteValues_, {0, 1, 2});
  obsData(2) = MisVal(missingFiniteValues_, {1, 2,});
  obsData(3) = MisVal(missingFiniteValues_, {3, 4});
  obsData(4) = MisVal(missingFiniteValues_, {3, 4});
  rankIndividual.setO(xVec);
  rankIndividual.setObsData(obsData);
  rankIndividual.removeMissing();

#ifdef MC_DEBUGNEW
  std::cout << "top" << std::endl;
#endif

  std::map<RankVal, int> rankCount;

  for (int i = 0; i < nbIterationBurnIn; ++i) {
    rankIndividual.sampleY(mu, pi);
    rankIndividual.sampleX(mu, pi);
  }

  for (int i = 0; i < nbIteration; ++i) {
    rankIndividual.sampleY(mu, pi);
    rankIndividual.sampleX(mu, pi);

    if (rankCount.find(rankIndividual.x()) == rankCount.end()) {
      rankCount[rankIndividual.x()] = 1;
    }
    else {
      rankCount[rankIndividual.x()] += 1;
    }
  }

  std::map<RankVal, int>::const_iterator it = rankCount.begin(), itE = rankCount.end();
  RankVal mode = it->first;
  int modeCount = it->second;
  for (; it != itE; ++it) { // find the mode
    if (it->second > modeCount) { // new mode found
      mode = it->first;
      modeCount = it->second;
    }
  }

#ifdef MC_DEBUG
  std::cout << "filteredProba: " << itString(filteredProba) << ", lnObservedDirectSampling: " << lnObservedDirectSampling << std::endl;
#endif

  ASSERT_EQ(mu, mode);
}

/** Do Gibbs with everything missing and check that log observed proba is 0. */
