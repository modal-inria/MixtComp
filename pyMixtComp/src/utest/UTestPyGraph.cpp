/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: June 21, 2019
 *  Author:     Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"
#include "pythonIO.h"
#include <map>
#include <boost/python/numpy.hpp>

using namespace mixt;

TEST(PyGraph, constructor) {
	Py_Initialize();
	PyGraph g;

	boost::python::dict d = g.getD();
	boost::python::dict d0;

	EXPECT_EQ(d, d0);

	d0["toto"] = 3;
	PyGraph g2(d0);
	d = g2.getD();

	EXPECT_EQ(d, d0);
}

TEST(PyGraph, basicAdd0) {
	Py_Initialize();
	PyGraph g;
	g.add_payload( { }, "toto", 12);
	boost::python::dict comp = g.getD();
	boost::python::dict exp;
	exp["toto"] = 12;
	ASSERT_EQ(comp, exp);
}

TEST(PyGraph, basicAdd1) {
	PyGraph g;
	g.add_payload( { "complex", "path" }, "toto", 12);
	boost::python::dict comp = g.getD();

	boost::python::dict exp;
	exp["complex"] = boost::python::dict();
	exp["complex"]["path"] = boost::python::dict();
	exp["complex"]["path"]["toto"] = 12;

	ASSERT_EQ(exp, comp);
}


TEST(PyGraph, basicGet) {
	Index exp = 12;
	PyGraph g;
	g.add_payload( { "complex", "path" }, "toto", exp);
	Index comp;
	g.get_payload( { "complex", "path" }, "toto", comp);
	ASSERT_EQ(comp, exp);
}

TEST(PyGraph, combined) {
	boost::python::dict exp;
	exp["paramStr"] = "A parameter";
	exp["varA"] = boost::python::dict();
	exp["varA"]["A nested real"] = 12.0;
	exp["varA"]["A nested string"] = "Hello World !";

	PyGraph g;
	g.add_payload( { }, "paramStr", "A parameter");
	g.add_payload( { "varA" }, "A nested real", 12.0);
	g.add_payload( { "varA" }, "A nested string", "Hello World !");

	boost::python::dict comp = g.getD();

	ASSERT_EQ(exp, comp);
}

TEST(PyGraph, NamedVectorReal) {
	boost::python::numpy::initialize();

	boost::python::dict exp;
	exp["A named vector"] = boost::python::dict();
	exp["A named vector"]["ctype"] = "Vector";
	std::vector<double> vec2 = {1.0,2.0,3.0};
	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<double>();
	boost::python::tuple shape = boost::python::make_tuple(vec2.size());
	boost::python::tuple stride = boost::python::make_tuple(sizeof(double));
	boost::python::object own;
	boost::python::numpy::ndarray data = boost::python::numpy::from_data(&vec2[0], dt, shape, stride, own);
	exp["A named vector"]["data"] = data;
	exp["A named vector"]["dtype"] = "Real";
	exp["A named vector"]["nrow"] = 3;
	std::vector<std::string> vec3 = {"riri", "fifi", "loulou"};
	boost::python::list rowNames;
	for(auto& s: vec3){
		rowNames.append(s);
	}
	exp["A named vector"]["rowNames"] = rowNames;

	PyGraph gIn;
	gIn.set(exp);

	Vector<Real> vec;
	std::vector<std::string> rowNames2;

	NamedVector<Real> nv = { rowNames2, vec };
	gIn.get_payload( { }, "A named vector", nv);

	PyGraph gOut;
	gOut.add_payload( { }, "A named vector", nv);

	boost::python::dict comp = gOut.getD();

	// it seems that ASSERT_EQ does not work with ndarray,
	ASSERT_TRUE(comp.has_key("A named vector"));
	boost::python::dict comp0 = boost::python::extract<boost::python::dict>(comp["A named vector"]);
	ASSERT_TRUE(comp0.has_key("data"));
	ASSERT_TRUE(comp0.has_key("dtype"));
	ASSERT_TRUE(comp0.has_key("nrow"));
	ASSERT_TRUE(comp0.has_key("ctype"));
	ASSERT_TRUE(comp0.has_key("rowNames"));
	EXPECT_EQ(comp0["nrow"], 3);
	EXPECT_EQ(comp0["dtype"], "Real");
	EXPECT_EQ(comp0["ctype"], "Vector");

	std::string rowNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["rowNames"]));
	EXPECT_EQ(rowNamesOut, "['riri', 'fifi', 'loulou']");
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp0["data"]));
	EXPECT_EQ(dataOut, "[1. 2. 3.]");
}


TEST(PyGraph, NamedVectorIntVoidNamesVoidData) {
	boost::python::dict exp;
	exp["A named vector"] = boost::python::dict();
	exp["A named vector"]["ctype"] = "Vector";
	std::vector<int> vec2 = {};
	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<int>();
	boost::python::tuple shape = boost::python::make_tuple(vec2.size());
	boost::python::tuple stride = boost::python::make_tuple(sizeof(int));
	boost::python::object own;
	boost::python::numpy::ndarray data = boost::python::numpy::from_data(&vec2[0], dt, shape, stride, own);
	exp["A named vector"]["data"] = data;
	exp["A named vector"]["dtype"] = "Integer";
	exp["A named vector"]["nrow"] = 0;
	std::vector<std::string> vec3 = {};
	boost::python::list rowNames;
	for(auto& s: vec3){
		rowNames.append(s);
	}
	exp["A named vector"]["rowNames"] = rowNames;

	PyGraph gIn;
	gIn.set(exp);

	Vector<int> vec;
	std::vector<std::string> rowNames2;

	NamedVector<int> nv = { rowNames2, vec };
	gIn.get_payload( { }, "A named vector", nv);

	PyGraph gOut;
	gOut.add_payload( { }, "A named vector", nv);

	boost::python::dict comp = gOut.getD();

	// it seems that ASSERT_EQ does not work with ndarray,
	ASSERT_TRUE(comp.has_key("A named vector"));
	boost::python::dict comp0 = boost::python::extract<boost::python::dict>(comp["A named vector"]);
	ASSERT_TRUE(comp0.has_key("data"));
	ASSERT_TRUE(comp0.has_key("dtype"));
	ASSERT_TRUE(comp0.has_key("nrow"));
	ASSERT_TRUE(comp0.has_key("ctype"));
	ASSERT_TRUE(comp0.has_key("rowNames"));
	EXPECT_EQ(comp0["nrow"], 0);
	EXPECT_EQ(comp0["dtype"], "Integer");
	EXPECT_EQ(comp0["ctype"], "Vector");

	std::string rowNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["rowNames"]));
	EXPECT_EQ(rowNamesOut, "[]");
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp0["data"]));
	EXPECT_EQ(dataOut, "[]");
}



TEST(PyGraph, NamedMatrixReal) {
	boost::python::dict exp;
	exp["A named matrix"] = boost::python::dict();
	exp["A named matrix"]["ctype"] = "Matrix";
	exp["A named matrix"]["dtype"] = "Real";
	exp["A named matrix"]["nrow"] = 4;
	exp["A named matrix"]["ncol"] = 3;

	std::vector<std::string> vec2 = {"1", "2", "3"};
	boost::python::list colNames;
	for(auto& s: vec2){
		colNames.append(s);
	}
	exp["A named matrix"]["colNames"] = colNames;

	std::vector<std::string> vec3 = {"A", "B", "C", "D"};
	boost::python::list rowNames;
	for(auto& s: vec3){
		rowNames.append(s);
	}
	exp["A named matrix"]["rowNames"] = rowNames;

	double mul_data[][3] = {{1.0,2.0,3.0}, {4.0,5.0,6.0}, {7.0,8.0,9.0}, {10.0,11.0,12.0}};

	boost::python::tuple shape = boost::python::make_tuple(4, 3);
	boost::python::tuple stride = boost::python::make_tuple(sizeof(double)*3, sizeof(double));
	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<double>();
	boost::python::numpy::ndarray data = boost::python::numpy::from_data(mul_data, dt, shape, stride, boost::python::object());
	exp["A named matrix"]["data"] = data;


	PyGraph gIn;
	gIn.set(exp);

	Matrix<Real> vec;
	std::vector<std::string> rowNames2;
	std::vector<std::string> colNames2;
	NamedMatrix<Real> nm = { rowNames2, colNames2, vec };
	gIn.get_payload( { }, "A named matrix", nm);

	PyGraph gOut;
	gOut.add_payload( { }, "A named matrix", nm);
	boost::python::dict comp = gOut.getD();

	ASSERT_TRUE(comp.has_key("A named matrix"));
	boost::python::dict comp0 = boost::python::extract<boost::python::dict>(comp["A named matrix"]);
	ASSERT_TRUE(comp0.has_key("data"));
	ASSERT_TRUE(comp0.has_key("dtype"));
	ASSERT_TRUE(comp0.has_key("nrow"));
	ASSERT_TRUE(comp0.has_key("ncol"));
	ASSERT_TRUE(comp0.has_key("ctype"));
	ASSERT_TRUE(comp0.has_key("rowNames"));
	ASSERT_TRUE(comp0.has_key("colNames"));
	EXPECT_EQ(comp0["nrow"], 4);
	EXPECT_EQ(comp0["ncol"], 3);
	EXPECT_EQ(comp0["dtype"], "Real");
	EXPECT_EQ(comp0["ctype"], "Matrix");

	std::string rowNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["rowNames"]));
	EXPECT_EQ(rowNamesOut, "['A', 'B', 'C', 'D']");
	std::string colNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["colNames"]));
	EXPECT_EQ(colNamesOut, "['1', '2', '3']");
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp0["data"]));
	EXPECT_EQ(dataOut, "[[ 1.  2.  3.]\n [ 4.  5.  6.]\n [ 7.  8.  9.]\n [10. 11. 12.]]");
}


TEST(PyGraph, NamedMatrixIntVoidNames) {
	boost::python::dict exp;
	exp["A named matrix"] = boost::python::dict();
	exp["A named matrix"]["ctype"] = "Matrix";
	exp["A named matrix"]["dtype"] = "Real";
	exp["A named matrix"]["nrow"] = 4;
	exp["A named matrix"]["ncol"] = 3;

	std::vector<std::string> vec2 = {};
	boost::python::list colNames;
	for(auto& s: vec2){
		colNames.append(s);
	}
	exp["A named matrix"]["colNames"] = colNames;

	std::vector<std::string> vec3 = {};
	boost::python::list rowNames;
	for(auto& s: vec3){
		rowNames.append(s);
	}
	exp["A named matrix"]["rowNames"] = rowNames;

	int mul_data[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

	boost::python::tuple shape = boost::python::make_tuple(4, 3);
	boost::python::tuple stride = boost::python::make_tuple(sizeof(int)*3, sizeof(int));
	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<int>();
	boost::python::numpy::ndarray data = boost::python::numpy::from_data(mul_data, dt, shape, stride, boost::python::object());
	exp["A named matrix"]["data"] = data;


	PyGraph gIn;
	gIn.set(exp);

	Matrix<int> vec;
	std::vector<std::string> rowNames2;
	std::vector<std::string> colNames2;
	NamedMatrix<int> nm = { rowNames2, colNames2, vec };
	gIn.get_payload( { }, "A named matrix", nm);

	PyGraph gOut;
	gOut.add_payload( { }, "A named matrix", nm);
	boost::python::dict comp = gOut.getD();

	ASSERT_TRUE(comp.has_key("A named matrix"));
	boost::python::dict comp0 = boost::python::extract<boost::python::dict>(comp["A named matrix"]);
	ASSERT_TRUE(comp0.has_key("data"));
	ASSERT_TRUE(comp0.has_key("dtype"));
	ASSERT_TRUE(comp0.has_key("nrow"));
	ASSERT_TRUE(comp0.has_key("ncol"));
	ASSERT_TRUE(comp0.has_key("ctype"));
	ASSERT_TRUE(comp0.has_key("rowNames"));
	ASSERT_TRUE(comp0.has_key("colNames"));
	EXPECT_EQ(comp0["nrow"], 4);
	EXPECT_EQ(comp0["ncol"], 3);
	EXPECT_EQ(comp0["dtype"], "Integer");
	EXPECT_EQ(comp0["ctype"], "Matrix");

	std::string rowNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["rowNames"]));
	EXPECT_EQ(rowNamesOut, "[]");
	std::string colNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["colNames"]));
	EXPECT_EQ(colNamesOut, "[]");
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp0["data"]));
	EXPECT_EQ(dataOut, "[[ 1  2  3]\n [ 4  5  6]\n [ 7  8  9]\n [10 11 12]]");
}


TEST(PyGraph, AddVector) {
	std::vector<std::string> vec = {"riri", "fifi", "loulou"};
	PyGraph g;
	g.add_payload( { "complex", "path" }, "toto", vec);
	boost::python::dict comp = g.getD();

	boost::python::dict exp;
	boost::python::list pyList;
	pyList.append("riri");
	pyList.append("fifi");
	pyList.append("loulou");

	exp["complex"] = boost::python::dict();
	exp["complex"]["path"] = boost::python::dict();
	exp["complex"]["path"]["toto"] = pyList;

	ASSERT_EQ(exp, comp);
}

TEST(PyGraph, AddVector2) {
	std::vector<std::vector<int> > vec(2);
	std::vector<int> vec2 = {1, 2, 3};
	std::vector<int> vec3 = {4, 5, 6};

	vec[0] = vec2;
	vec[1] = vec3;

	PyGraph g;
	g.add_payload( { "complex", "path" }, "toto", vec);
	boost::python::dict comp = g.getD();

	boost::python::dict exp;
	boost::python::list pyList, pyList2, pyList3;
	for(int i = 1; i < 4; ++i)
		pyList2.append(i);
	pyList.append(pyList2);
	for(int i = 4; i < 7; ++i)
		pyList3.append(i);			;
	pyList.append(pyList3);

	exp["complex"] = boost::python::dict();
	exp["complex"]["path"] = boost::python::dict();
	exp["complex"]["path"]["toto"] = pyList;

	ASSERT_EQ(exp, comp);

}

TEST(PyGraph, VectorOfString) {
	boost::python::dict exp;
	std::vector<std::string> vec = {"12.0","-35.90","205.72"};
	boost::python::list temp;
	for(auto& s: vec){
		temp.append(s);
	}

	exp["var1"] = temp;

	PyGraph gIn;
	gIn.set(exp);
	std::vector<std::string> vec2;
	gIn.get_payload( { }, "var1", vec2);

	PyGraph gOut;
	gOut.add_payload( { }, "var1", vec2);

	boost::python::dict comp = gOut.getD();

	ASSERT_TRUE(comp.has_key("var1"));
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp["var1"]));
	EXPECT_EQ(dataOut, "['12.0', '-35.90', '205.72']");
}

TEST(PyGraph, name_payload) {
	boost::python::dict exp;
	exp["var1"] = "12.0";
	exp["varZ"] = "test";

	PyGraph gIn;
	gIn.set(exp);

	std::list<std::string> l;
	gIn.name_payload( { }, l);

	ASSERT_EQ(itString(l), std::string("var1 varZ"));
}

/**
 * This is used in the data output of the Functional model
 */
TEST(PyGraph, VectorOfVectorOfReal) {
	boost::python::dict exp;
	boost::python::list pyList, pyList1, pyList2, pyList3;
	pyList1.append(1.0);
	pyList2.append(1.0);
	pyList2.append(5.0);
	pyList2.append(12.0);
	pyList3.append(0.0);
	pyList3.append(-42.0);
	pyList.append(pyList1);
	pyList.append(pyList2);
	pyList.append(pyList3);
	exp["var1"] = pyList;
	PyGraph gIn;
	gIn.set(exp);

	std::vector<std::vector<Real> > temp;
	gIn.get_payload( { }, "var1", temp);

	PyGraph gOut;
	gOut.add_payload( { }, "var1", temp);
	boost::python::dict comp = gOut.getD();

	ASSERT_EQ(exp, comp);
}

TEST(PyGraph, AddSubGraph) {
	boost::python::dict in, sub, expected;
	in["var"] = "toto";
	sub["var"] = "sub-toto";
	expected["subG"] = boost::python::dict();
	expected["subG"]["var"] = "sub-toto";
	expected["var"] = "toto";

	PyGraph gIn;
	gIn.set(in);

	PyGraph subG;
	subG.set(sub);

	gIn.addSubGraph({}, "subG", subG);

	boost::python::dict out = gIn.getD();
	ASSERT_TRUE(out.has_key("var"));
	ASSERT_TRUE(out.has_key("subG"));
	boost::python::dict out0 = boost::python::extract<boost::python::dict>(out["subG"]);
	ASSERT_TRUE(out0.has_key("var"));
	ASSERT_EQ(out["var"], "toto");
	ASSERT_EQ(out0["var"], "sub-toto");

}
