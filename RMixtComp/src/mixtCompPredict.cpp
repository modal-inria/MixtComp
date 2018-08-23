/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// #include <omp.h>

#include <Rcpp.h>

#include<Run/Learn.h>

#include "mixt_MixtComp.h"

// [[Rcpp::export]]
Rcpp::List mixtCompPredict(Rcpp::List dataList, Rcpp::List paramList, Rcpp::List mcStrategy, int nbClass, double confidenceLevel) {
	Rcpp::List res;
	return res;
}
