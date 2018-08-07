/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"
#include "jsonIO.h"

using namespace mixt;

TEST(JSonToSGraph, basic) {
	nlohmann::json expectedJSON =
			R"({"paramStr":"A parameter","varA":{"A nested real":12.0,"A nested string":"Hello World !"}})"_json;

	nlohmann::json computedJSON = SGraphToJSON(JSONToSGraph(expectedJSON));

	ASSERT_EQ(expectedJSON, computedJSON);
}
