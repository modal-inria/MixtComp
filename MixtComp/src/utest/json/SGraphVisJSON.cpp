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
	std::string expectedRes = "{\"paramStr\":\"A parameter\",\"varA\":{\"A nested real\":12.0,\"A nested string\":\"Hello World !\"}}";

	ASSERT_EQ(computedRes, expectedRes);
}
