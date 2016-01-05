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
}

TEST(RankIndividual, enumCompletedAllMissing) {
  int nbPos = 5;

  Vector<int> o(nbPos);
  o << 0,4,3,1,2;

  Vector<MisVal> mv(nbPos);
  mv(0) = MisVal(missing_, {});
  mv(1) = MisVal(missing_, {});
  mv(2) = MisVal(missing_, {});
  mv(3) = MisVal(missing_, {});
  mv(4) = MisVal(missing_, {});

  RankIndividual rv(nbPos);
  rv.setO(o);
  rv.setObsData(mv);

  std::list<RankVal> rvList = rv.enumCompleted();

#ifdef MC_DEBUG
  std::cout << "rvList.size(): " << rvList.size() << std::endl;
#endif

  ASSERT_EQ(rvList.size(), fac(nbPos));
}

TEST(RankIndividual, enumCompletedAllPresent) {
  int nbPos = 5;

  Vector<int> o(nbPos);
  o << 0,4,3,1,2;

  Vector<MisVal> mv(nbPos);
  mv(0) = MisVal(present_, {});
  mv(1) = MisVal(present_, {});
  mv(2) = MisVal(present_, {});
  mv(3) = MisVal(present_, {});
  mv(4) = MisVal(present_, {});

  RankIndividual rv(nbPos);
  rv.setO(o);
  rv.setObsData(mv);

  std::list<RankVal> rvList = rv.enumCompleted();

#ifdef MC_DEBUG
  std::cout << "rvList.size(): " << rvList.size() << std::endl;
#endif

  ASSERT_EQ(rvList.size(), 1);
}

TEST(RankIndividual, invalidIndividual) {
  int nbPos = 5;

  Vector<int> o(nbPos);
  o << 0,4,1,1,2;

  Vector<MisVal> mv(nbPos);
  mv(0) = MisVal(present_, {});
  mv(1) = MisVal(present_, {});
  mv(2) = MisVal(present_, {});
  mv(3) = MisVal(present_, {});
  mv(4) = MisVal(present_, {});

  RankIndividual rv(nbPos);
  rv.setO(o);
  rv.setObsData(mv);

  std::list<RankVal> rvList = rv.enumCompleted();

#ifdef MC_DEBUG
  std::cout << "rvList.size(): " << rvList.size() << std::endl;
#endif

  ASSERT_EQ(rvList.size(), 0);
}

TEST(RankIndividual, partialyObserved) {
  int nbPos = 5;

  Vector<int> o(nbPos);
  o << 0,4,1,3,2;

  Vector<MisVal> mv(nbPos);
  mv(0) = MisVal(present_, {});
  mv(1) = MisVal(missingFiniteValues_, {1, 4}); // 2 completions will be possible by permuting 1 and 4 ordering
  mv(2) = MisVal(missingFiniteValues_, {1, 4, 2});
  mv(3) = MisVal(present_, {});
  mv(4) = MisVal(present_, {});

  RankIndividual rv(nbPos);
  rv.setO(o);
  rv.setObsData(mv);

  std::list<RankVal> rvList = rv.enumCompleted();

#ifdef MC_DEBUG
  std::cout << "rvList.size(): " << rvList.size() << std::endl;
#endif

  ASSERT_EQ(rvList.size(), 2);
}
