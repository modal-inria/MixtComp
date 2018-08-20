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

/**
 * To do:
 * - get names
 */
// [[Rcpp::export]]
void rcppTest(SEXP elem) {
	if (TYPEOF(elem) == VECSXP) {
		std::cout << "generic vectors" << std::endl;
		Rcpp::List list = Rcpp::as < Rcpp::List > (elem);
		std::vector < std::string > names;

		SEXP dm = list.attr("names");
		if (!Rf_isNull(dm)) {
			std::cout << "input has names" << std::endl;
			names = Rcpp::as < std::vector < std::string >> (list.names());
		}

		for (std::vector<std::string>::const_iterator it = names.begin(), itEnd = names.end(); it != itEnd; ++it) {
			std::cout << *it << std::endl;
		}

		int size = list.size();
		std::cout << size << std::endl;
		std::cout << Rcpp::as<int>(list[0]) << std::endl;
	}

// std::cout << elem->names() << std::endl;

//  std::cout << Rf_isPrimitive(elem) << std::endl;
//  std::cout << Rf_isNumeric(elem) << std::endl;
//  std::cout << Rf_isList(elem) << std::endl;
//  std::cout << Rf_isVector(elem) << std::endl;
//  std::cout << Rf_isMatrix(elem) << std::endl;
//  std::cout << TYPEOF(elem) << std::endl;
}

// [[Rcpp::export]]
Rcpp::List SGraphTest(Rcpp::List l) {
	Rcpp::List res;

	try {
		SGraph s = RToSGraph(l);
		res = SGraphToR(s);
	} catch (const std::string& s) {
		res["warnLog"] = s;
	}

	return res;
}
