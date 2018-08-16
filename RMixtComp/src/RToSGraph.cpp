/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 10, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Rcpp.h"
#include <IO/SGraph.h>
#include "RToSGraph.h"

mixt::SGraph RToSGraph(const Rcpp::List& ls) {
	mixt::SGraph res;
	std::vector<std::string> names = ls.names();
//	int test = ls.attr();

	for (Rcpp::List::const_iterator it = ls.begin(), itEnd = ls.end(); it != itEnd; ++it) {
		if (TYPEOF(*it) == VECSXP) { // is this always an Rcpp::List ?

		}
	}

	return res;
}
