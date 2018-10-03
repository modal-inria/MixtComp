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
	g.add_payload( { }, "toto", 12);
	std::string comp = g.get();
	std::string exp = R"-({"toto":12})-";
	ASSERT_EQ(comp, exp);
}

TEST(JSONGraph, basicAdd1) {
	JSONGraph g;
	g.add_payload( { "complex", "path" }, "toto", 12);
	std::string comp = g.get();
	std::string exp = R"-({"complex":{"path":{"toto":12}}})-";
	ASSERT_EQ(comp, exp);
}

TEST(JSONGraph, basicGet) {
	Index exp = 12;
	JSONGraph g;
	g.add_payload( { "complex", "path" }, "toto", exp);
	Index comp;
	g.get_payload( { "complex", "path" }, "toto", comp);
	ASSERT_EQ(comp, exp);
}

TEST(JSONSGraph, combined) {
	std::string exp = R"({"paramStr":"A parameter","varA":{"A nested real":12.0,"A nested string":"Hello World !"}})";

	JSONGraph g;
	g.add_payload( { }, "paramStr", "A parameter");
	g.add_payload( { "varA" }, "A nested real", 12.0);
	g.add_payload( { "varA" }, "A nested string", "Hello World !");

	std::string comp = g.get();

	ASSERT_EQ(exp, comp);
}

TEST(JSONSGraph, NamedVectorReal) {
	std::string exp = R"-({"A named vector":{"ctype":"Vector","data":[1.0,2.0,3.0],"dtype":"Real","nrow":3,"rowNames":["riri","fifi","loulou"]}})-";
	JSONGraph gIn;
	gIn.set(exp);

	Vector<Real> vec;
	std::vector<std::string> rowNames;
	NamedVector<Real> nv = { rowNames, vec };
	gIn.get_payload( { }, "A named vector", nv);

	JSONGraph gOut;
	gOut.add_payload( { }, "A named vector", nv);
	std::string comp = gOut.get();

	ASSERT_EQ(exp, comp);
}

TEST(JSONSGraph, NamedMatrixReal) {
	std::string exp =
			R"-({"A named matrix":{"colNames":["1","2","3"],"ctype":"Matrix","data":[[1.0,2.0,3.0],[4.0,5.0,6.0],[7.0,8.0,9.0]],"dtype":"Real","ncol":3,"nrow":3,"rowNames":["A","B","C"]}})-";
	JSONGraph gIn;
	gIn.set(exp);

	Matrix<Real> vec;
	std::vector<std::string> rowNames;
	std::vector<std::string> colNames;
	NamedMatrix<Real> nm = { rowNames, colNames, vec };
	gIn.get_payload( { }, "A named matrix", nm);

	JSONGraph gOut;
	gOut.add_payload( { }, "A named matrix", nm);
	std::string comp = gOut.get();

	ASSERT_EQ(exp, comp);
}

TEST(JSONSGraph, VectorOfString) {
	std::string exp = R"-({"var1":["12.0","-35.90","205.72"]})-";
	JSONGraph gIn;
	gIn.set(exp);

	std::vector<std::string> vec;
	gIn.get_payload<std::vector<std::string>>( { }, "var1", vec);

	JSONGraph gOut;
	gOut.add_payload( { }, "var1", vec);
	std::string comp = gOut.get();

	ASSERT_EQ(exp, comp);
}

TEST(JSONSGraph, name_payload) {
	std::string exp = R"-({"var1":"12.0","varZ":"test"})-";
	JSONGraph gIn;
	gIn.set(exp);

	std::list<std::string> l;
	gIn.name_payload( { }, l);

	ASSERT_EQ(itString(l), std::string("var1 varZ"));
}

/**
 * This is used in the data output of the Functional model
 */
TEST(JSONSGraph, VectorOfVectorOfReal) {
	std::string exp = R"-({"var1":[[1.],[1., 5., 12.],[0., -42.]]})-";
	JSONGraph gIn;
	gIn.set(exp);

	std::vector<std::vector<Real>> temp;
	gIn.get_payload<std::vector<std::vector<Real>>>( { }, "var1", temp);

//	JSONGraph gOut;
//	gOut.add_payload( { }, "var1", temp);
//	std::string comp = gOut.get();
//
//	ASSERT_EQ(exp, comp);
}
