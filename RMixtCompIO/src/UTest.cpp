/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

/*
 *  Project:    MixtComp
 *  Created on: Nov 22, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Rcpp.h>

#include "MixtComp.h"
#include "RGraph.h"

using namespace mixt;

/**
 * https://stackoverflow.com/questions/52006424/modifying-a-subsection-of-an-rcpplist-in-a-separate-function-by-reference
 */
// [[Rcpp::export]]
Rcpp::List UTest1() {
	RGraph rg;

	rg.add_payload( { "a" }, "a1", "riri");
	rg.add_payload( { "a" }, "a2", "fifi");
	rg.add_payload( { "a" }, "a3", "loulou");
	rg.add_payload( { "b", "b1" }, "b11", "dingo");

	return rg.getL();
}

/**
 * https://stackoverflow.com/questions/52006424/modifying-a-subsection-of-an-rcpplist-in-a-separate-function-by-reference
 */
// [[Rcpp::export]]
bool UTest2() {
	RGraph rg;
	rg.add_payload( { "a" }, "a1", "riri");

	return rg.exist_payload( { "a" }, "a1");
}

// [[Rcpp::export]]
std::string UTest3() {
	RGraph rg;

	rg.add_payload( { "a" }, "a1", "riri");

	std::string res;

	try {
		rg.get_payload<std::string>( { "a" }, "a1", res);
	} catch (const std::string& s) {
	  Rcpp::Rcout << "exception: " << s << std::endl;
	}

	return res;
}

// [[Rcpp::export]]
Rcpp::List UTest4(const Rcpp::List& l) {
	RGraph rgIn(l);
	NamedVector<Real> nv;
	RGraph rgOut;

	rgIn.get_payload( { "This", "is" }, "Sparta", nv);
	rgOut.add_payload( { "This", "is" }, "Sparta", nv);

	return rgOut.getL();
}

// [[Rcpp::export]]
Rcpp::List UTest5(const Rcpp::List& l) {
	RGraph rgIn(l);
	NamedMatrix<Real> nm;
	RGraph rgOut;

	rgIn.get_payload( { }, "mat", nm);
	rgOut.add_payload( { }, "mat", nm);

	return rgOut.getL();
}

// [[Rcpp::export]]
bool UTest6() {
	RGraph dummy;
	return dummy.exist_payload( { }, "z_class");
}

// [[Rcpp::export]]
SEXP dummyTest() {
	Index nrow = 2;
	Index ncol = 3;

	Rcpp::IntegerMatrix temp(nrow, ncol);

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			temp(i, j) = i * j;
		}
	}

	rownames(temp) = Rcpp::CharacterVector::create("row1", "row2");
	colnames(temp) = Rcpp::CharacterVector::create("col1", "col2", "col3");

	return temp;
}
