/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 22, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Rcpp.h>
#include <IO/SGraph.h>
#include "RToSGraph.h"
#include "SGraphToR.h"

using namespace mixt;

// [[Rcpp::export]]
Rcpp::List SGraphTest(Rcpp::List l) {
	Rcpp::List res;

	try {
		res = SGraphToR(RToSGraph(l));
	} catch (const std::string& s) {
		res["warnLog"] = s;
	}

	return res;
}
