/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"
#include "jsonIO.h"

using namespace mixt;

TEST(JSONGraph, basicAdd0) {
	JSONGraph g;
	g.add_payload({}, "toto", 12);
	std::string comp = g.get();
	std::string exp = R"-({"toto":12})-";
	ASSERT_EQ(comp, exp);
}

TEST(JSONGraph, basicAdd1) {
	JSONGraph g;
	g.add_payload({"complex", "path"}, "toto", 12);
	std::string comp = g.get();
	std::string exp = R"-({"complex":{"path":{"toto":12}}})-";
	ASSERT_EQ(comp, exp);
}

TEST(JSONGraph, basicGet) {
	Index exp = 12;
	JSONGraph g;
	g.add_payload({"complex", "path"}, "toto", exp);
	Index comp;
	g.get_payload({"complex", "path"}, "toto", comp);
	ASSERT_EQ(comp, exp);
}

TEST(JSONSGraph, combined) {
	std::string exp = R"({"paramStr":"A parameter","varA":{"A nested real":12.0,"A nested string":"Hello World !"}})";

	JSONGraph g;
	g.add_payload({}, "paramStr", "A parameter");
	g.add_payload({"varA"}, "A nested real", 12.0);
	g.add_payload({"varA"}, "A nested string", "Hello World !");

	std::string comp = g.get();

	ASSERT_EQ(exp, comp);
}
