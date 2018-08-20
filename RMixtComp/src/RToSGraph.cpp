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

SGraph RToSGraph(const Rcpp::List& ls) {
	SGraph res;
	std::vector < std::string > names;

	SEXP dm = ls.attr("names");
	if (!Rf_isNull(dm)) {
		names = Rcpp::as < std::vector < std::string >> (ls.names());
	} else {
		throw(std::string("All list in input should be named lists (have named elements)."));
	}

	for (Index i = 0; i < ls.size(); ++i) {
		SEXP currElem = ls[i];
		if (TYPEOF(currElem) == VECSXP) { // recursive call
			res.add_child(names[i], RToSGraph(currElem));
		} else if (TYPEOF(currElem) == STRSXP) { // contains a vector of strings
			addCharacterVector(names[i], currElem, res);
		}
	}

	return res;
}

void addCharacterVector(const std::string& name, SEXP s, SGraph& g) {
	Rcpp::CharacterVector cv(s);

	SEXP currElemNames = cv.attr("names");
	if (Rf_isNull(currElemNames) && cv.size() == 1) { // condition for the Vector to be considered a scalar
		g.add_payload(name, Rcpp::as <std::string>(cv[0]));
	} else {
		g.add_payload(name, Rcpp::as < std::vector < std::string >> (cv));
	}
}
