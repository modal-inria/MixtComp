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

TEST(JSONToSGraph, basic) {
	nlohmann::json expectedJSON = R"({"paramStr":"A parameter","varA":{"A nested real":12.0,"A nested string":"Hello World !"}})"_json;

	nlohmann::json computedJSON = SGraphToJSON(JSONToSGraph(expectedJSON));

	ASSERT_EQ(expectedJSON, computedJSON);
}

TEST(JSONToSGraph, VectorReal) {
	nlohmann::json expectedJSON = R"-({"A named vector":{"ctype":"Vector","data":[1.0,2.0,3.0],"dtype":"Real","nrow":3,"rowNames":["riri","fifi","loulou"]}})-"_json;

	nlohmann::json computedJSON = SGraphToJSON(JSONToSGraph(expectedJSON));

	ASSERT_EQ(expectedJSON, computedJSON);
}

TEST(JSONToSGraph, MatrixReal) {
	nlohmann::json expectedJSON =
			R"-({"A named matrix":{"colNames":["1","2","3"],"ctype":"Matrix","data":[[1.0,2.0,3.0],[4.0,5.0,6.0],[7.0,8.0,9.0]],"dtype":"Real","ncol":3,"nrow":3,"rowNames":["A","B","C"]}})-"_json;

	nlohmann::json computedJSON = SGraphToJSON(JSONToSGraph(expectedJSON));

	ASSERT_EQ(expectedJSON, computedJSON);
}

TEST(JSONToSGraph, VectorOfString) {
	nlohmann::json expectedJSON = R"-({"var1":["12.0","-35.90","205.72"]})-"_json;

	nlohmann::json computedJSON = SGraphToJSON(JSONToSGraph(expectedJSON));

	ASSERT_EQ(expectedJSON, computedJSON);
}
