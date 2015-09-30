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
 *  Created on: September 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include <map>

#include "../../src/Mixture/Rank/mixt_RankVal.h"
#include "../../src/LinAlg/mixt_Math.h"

#include "../../src/Mixture/Rank/mixt_RankClass.h"
#include "../../src/Mixture/Rank/mixt_RankIndividual.h"
#include "../../src/Mixture/Rank/mixt_RankParser.h"

using namespace mixt;

TEST(RankParser, basicTest)
{
  int nbMod = 4;

  RankParser rp;
  Vector<std::string> vecStr(3);
  vecStr << "1 , 3 , 2 , 0",
            "0 , 3 , 1 , 2",
            "0 , 1 , 2 , 3";

  Vector<RankIndividual> vecIndComputed;

  Vector<RankIndividual> vecIndExpected(3);

  vecIndExpected(0).setNbPos(4);
  vecIndExpected(0).setO(std::vector<int>({1 , 3 , 2 , 0}));

  vecIndExpected(1).setNbPos(4);
  vecIndExpected(1).setO(std::vector<int>({0 , 3 , 1 , 2}));

  vecIndExpected(2).setNbPos(4);
  vecIndExpected(2).setO(std::vector<int>({0 , 1 , 2 , 3}));

  rp.parseStr(vecStr,
              0, // minMod
              nbMod,
              vecIndComputed);

  ASSERT_EQ(vecIndComputed(0).x(), vecIndExpected(0).x());
  ASSERT_EQ(vecIndComputed(1).x(), vecIndExpected(1).x());
  ASSERT_EQ(vecIndComputed(2).x(), vecIndExpected(2).x());
}

TEST(RankParser, minMaxDetection)
{
  int nbPos;
  Vector<std::string> vecStr(1);
  vecStr << "{22 -8 1 2 3}, -1, 3, 38, 2";

  Vector<RankIndividual> vecInd;

  RankParser rp;

  std::pair<MisType, std::vector<int> > misVal;

  std::string resStr = rp.parseStr(vecStr,
                                   1, // min modality
                                   nbPos,
                                   vecInd);

#ifdef MC_DEBUG
  std::cout << resStr << std::endl;
#endif

  ASSERT_GT(resStr.size(), 0);
}
