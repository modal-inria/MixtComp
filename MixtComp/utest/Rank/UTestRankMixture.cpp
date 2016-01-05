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
 *  Created on: January 5, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

typedef typename std::pair<MisType, std::vector<int> > MisVal;

TEST(RankMixture, lnObservedProbability) {
  int nbClass = 1;
  int nbPos = 4;

  Vector<std::set<int> > classInd(nbClass);
  classInd(0).insert(0);

  Vector<int> x0(nbPos);
  x0 << 0, 1, 2, 3;
  Vector<RankIndividual> data(1);
  data(0).setNbPos(nbPos);
  data(0).setO(x0);

  Vector<Vector<MisVal> > obsData(1);
  obsData(0) = Vector<MisVal>(nbPos, MisVal(missing_, {})); // all positions are missing

  Vector<int> mu0(nbPos);
  mu0 << 2, 3, 0, 1;
  Vector<Real> pi(1);
  pi(0) = 0.8;

  Vector<RankVal> mu(nbClass);
  mu(0).setNbPos(nbPos);
  mu(0).setO(mu0);

  RankMixture<DataHandlerDummy,
              DataExtractorDummy,
              ParamSetterDummy,
              ParamExtractorDummy> rm("toto",
                                      classInd,
                                      mu,
                                      pi,
                                      data,
                                      obsData);

  rm.computeObservedProba();

  ASSERT_EQ(rm.lnObservedProbability(0, 0), 0.);
}
