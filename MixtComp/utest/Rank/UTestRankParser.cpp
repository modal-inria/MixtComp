/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(RankParser, basicTest) {
  int nbMod = 4;

  Vector<std::string> vecStr(3);
  vecStr << "1 , 3 , 2 , 0",
            "0 , 3 , 1 , 2",
            "0 , 1 , 2 , 3";

  Vector<RankIndividual> vecIndComputed; // result of parsing
  Vector<RankIndividual> vecIndExpected(3); // expected result of parsing

  vecIndExpected(0).setNbPos(4);
  vecIndExpected(0).setO(std::vector<int>({1 , 3 , 2 , 0}));

  vecIndExpected(1).setNbPos(4);
  vecIndExpected(1).setO(std::vector<int>({0 , 3 , 1 , 2}));

  vecIndExpected(2).setNbPos(4);
  vecIndExpected(2).setO(std::vector<int>({0 , 1 , 2 , 3}));

  parseRankStr(vecStr,
               0, // minMod
               nbMod,
               vecIndComputed);

  ASSERT_EQ(vecIndComputed(0).x(),
            vecIndExpected(0).x());
  ASSERT_EQ(vecIndComputed(1).x(),
            vecIndExpected(1).x());
  ASSERT_EQ(vecIndComputed(2).x(),
            vecIndExpected(2).x());
}

/** This test checks that the minimal and maximal modalities are correct, that is in the interval [minModality, nbPos - minModality + 1] */
TEST(RankParser, minMaxDetection) {
  int nbPos;
  Vector<std::string> vecStr(1);
  vecStr << "{22 -8 1 2 3}, -1, 3, 38, 2";

  Vector<RankIndividual> vecInd;

  MisVal misVal;

  std::string resStr = parseRankStr(vecStr,
                                    1, // min modality
                                    nbPos,
                                    vecInd);

#ifdef MC_DEBUG
  std::cout << resStr << std::endl;
#endif

  ASSERT_GT(resStr.size(), 0);
}
