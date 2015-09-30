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

#include "gtest/gtest.h"

#include <iostream>
#include "boost/regex.hpp"
#include "../src/Data/mixt_MisValParser.h"
#include "../src/LinAlg/mixt_LinAlg.h"
#include "../src/Mixture/Rank/mixt_RankParser.h"

using namespace mixt;

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
