/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 10, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Rcpp.h"
#include <IO/SGraph.h>
#include <LinAlg/mixt_LinAlg.h>
#include "RToSGraph.h"

mixt::SGraph RToSGraph(const Rcpp::List& ls) {
	mixt::SGraph res;
	std::vector < std::string > names;

	SEXP dm = ls.attr("names");
	if (!Rf_isNull(dm)) { // list must have named elements
		std::cout << "input has names" << std::endl;
		names = Rcpp::as < std::vector < std::string >> (ls.names());
	} else {
		throw(std::string("All list in input should be named lists (have named elements)."));
	}

	for (Index i = 0; i < ls.size(); ++i) {
		SEXP currElem = ls[i];
		if (TYPEOF(currElem) == VECSXP) { // recursive call
			std::cout << "Rcpp::List detected." << std::endl;
			res.add_child(names[i], RToSGraph(currElem));
		} else if (TYPEOF(currElem) == STRSXP) { // contains a vector of strings
			std::cout << "vector of strings detected." << std::endl;
			res.add_payload(names[i], Rcpp::as < std::vector < std::string >> (currElem));
	}
}

return res;
}
