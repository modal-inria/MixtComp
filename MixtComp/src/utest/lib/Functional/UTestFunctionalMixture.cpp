/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 5, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

/**
 * Test if the FunctionalMixture compiles. As it is a template class, it needs to be instancied to be compiled. So here we go.
 */
TEST(FunctionalMixture, compilation) {
  Vector<std::set<Index> > classInd;

//  FunctionalMixture<DataHandlerDummy,
//                    DataExtractorDummy,
//                    ParamSetterDummy,
//                    ParamExtractorDummy> dummyFunctionalMixture(classInd);
}
