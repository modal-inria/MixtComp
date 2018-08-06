/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 3, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"
#include "jsonIO.h"

using namespace mixt;

TEST(SGraphVisJSON, basicJSON) {
	SGraph g;
	g.add_payload("paramStr", "A parameter");

	SGraph child;
	child.add_payload("A nested real", 12.0);
	child.add_payload("A nested string", "Hello World !");

	g.add_child("varA", child);

	nlohmann::json res;

	res = SGraphToJSON(g);

	std::string computedRes = res.dump();
	std::string expectedRes =
			"{\"paramStr\":\"A parameter\",\"varA\":{\"A nested real\":12.0,\"A nested string\":\"Hello World !\"}}";

	ASSERT_EQ(computedRes, expectedRes);
}

TEST(SGraphVisJSON, integerInJSON) {
	SGraph g;
	g.add_payload("An integer", Index(12)); // 12 can not be used directly, because it would be an int, and there will be an ambiguity as to whether it should be stored as a long or as a double

	nlohmann::json res;

	res = SGraphToJSON(g);

	std::string computedRes = res.dump();
	std::string expectedRes =
			"{\"An integer\":12}";

	ASSERT_EQ(computedRes, expectedRes);
}

TEST(SGraphVisJSON, vectorInJSON) {
	Vector<Real> val(3);
	val << 1.0, 2.0, 3.0;

	std::vector<std::string> colNames = {"riri", "fifi", "loulou"};

	NamedVector<Real> ducks = {colNames, val};

	SGraph g;
	g.add_payload("A named vector", ducks); // 12 can not be used directly, because it would be an int, and there will be an ambiguity as to whether it should be stored as a long or as a double

	nlohmann::json res;

	res = SGraphToJSON(g);

	std::string computedRes = res.dump();
	std::string expectedRes =
			"{\"A named vector\":{\"data\":[1.0,2.0,3.0],\"rowNames\":[\"riri\",\"fifi\",\"loulou\"]}}";

	ASSERT_EQ(computedRes, expectedRes);
}
