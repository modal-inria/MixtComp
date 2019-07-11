/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

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
	boost::python::list rowNamesTemp;
	for(auto& s: vec3){
		rowNamesTemp.append(s);
	}
	boost::python::numpy::ndarray rowNames = boost::python::numpy::array(rowNamesTemp);
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
	EXPECT_EQ(rowNamesOut, "['riri' 'fifi' 'loulou']");
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp0["data"]));
	EXPECT_EQ(dataOut, "[1. 2. 3.]");
}

TEST(PyGraph, NamedMatrixReal) {
	boost::python::dict exp;
	exp["A named matrix"] = boost::python::dict();
	exp["A named matrix"]["ctype"] = "Matrix";
	exp["A named matrix"]["dtype"] = "Real";
	exp["A named matrix"]["nrow"] = 4;
	exp["A named matrix"]["ncol"] = 3;

	std::vector<std::string> vec2 = {"1", "2", "3"};
	boost::python::list colNamesTemp;
	for(auto& s: vec2){
		colNamesTemp.append(s);
	}
	boost::python::numpy::ndarray colNames = boost::python::numpy::array(colNamesTemp);
	exp["A named matrix"]["colNames"] = colNames;

	std::vector<std::string> vec3 = {"A", "B", "C", "D"};
	boost::python::list rowNamesTemp;
	for(auto& s: vec3){
		rowNamesTemp.append(s);
	}
	boost::python::numpy::ndarray rowNames = boost::python::numpy::array(rowNamesTemp);
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
	EXPECT_EQ(rowNamesOut, "['A' 'B' 'C' 'D']");
	std::string colNamesOut = boost::python::extract<std::string>(boost::python::str(comp0["colNames"]));
	EXPECT_EQ(colNamesOut, "['1' '2' '3']");
	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp0["data"]));
	EXPECT_EQ(dataOut, "[[ 1.  2.  3.]\n [ 4.  5.  6.]\n [ 7.  8.  9.]\n [10. 11. 12.]]");
}

//TEST(PyGraph, VectorOfString) {
//	boost::python::dict exp;
//	std::vector<std::string> vec = {"12.0","-35.90","205.72"};
//	boost::python::list temp;
//	for(auto& s: vec){
//		temp.append(s);
//	}
//	boost::python::numpy::ndarray var1 = boost::python::numpy::array(temp);
//	exp["var1"] = var1;
//
//	PyGraph gIn;
//	gIn.set(exp);
//	std::cout <<"ici"<<std::endl;
//	std::vector<std::string> vec2;
//	gIn.get_payload<std::vector<std::string> >( { }, "var1", vec2);
//
//	std::cout <<"la"<<std::endl;
//
//	PyGraph gOut;
//	gOut.add_payload( { }, "var1", vec2);
//	std::cout <<"et la"<<std::endl;
//
//	boost::python::dict comp = gOut.getD();
//
//	ASSERT_TRUE(comp.has_key("var1"));
//	std::string dataOut = boost::python::extract<std::string>(boost::python::str(comp["var1"]));
//	EXPECT_EQ(dataOut, "['12.0' '-35.90' '205.72']");
//}

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

///**
// * This is used in the data output of the Functional model
// */
//TEST(PyGraph, VectorOfVectorOfReal) {
//	std::string exp = R"-({"var1":[[1.0],[1.0,5.0,12.0],[0.0,-42.0]]})-";
//	PyGraph gIn;
//	gIn.set(exp);
//
//	std::vector<std::vector<Real>> temp;
//	gIn.get_payload<std::vector<std::vector<Real>>>( { }, "var1", temp);
//
//	PyGraph gOut;
//	gOut.add_payload( { }, "var1", temp);
//	std::string comp = gOut.get();
//
//	ASSERT_EQ(exp, comp);
//}
//
//TEST(PyGraph, AddSubGraph) {
//	std::string in = R"-({"var": "toto"})-";
//	std::string sub = R"-({"var": "sub-toto"})-";
//	std::string expected = R"-({"subG":{"var":"sub-toto"},"var":"toto"})-";
//
//	PyGraph gIn;
//	gIn.set(in);
//
//	PyGraph subG;
//	subG.set(sub);
//
//	gIn.addSubGraph({}, "subG", subG);
//
//	ASSERT_EQ(gIn.get(), expected);
//}

