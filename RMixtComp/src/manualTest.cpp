/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sep 21, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Rcpp.h>

#include "mixt_MixtComp.h"
#include "RGraph.h"

using namespace mixt;

/**
 * https://stackoverflow.com/questions/52006424/modifying-a-subsection-of-an-rcpplist-in-a-separate-function-by-reference
 */
// [[Rcpp::export]]
bool manualTest1() {
	RGraph rg;
	rg.add_payload( { "a" }, "a1", "riri");

	return rg.exist_payload({ "a" }, "a1");
}

// [[Rcpp::export]]
std::string manualTest2() {
	RGraph rg;

	rg.add_payload( { "a" }, "a1", "riri");

	return rg.get_payload<std::string>({ "a" }, "a1");
}
