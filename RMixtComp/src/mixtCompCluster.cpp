/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 22, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// #include <omp.h>

#include <Rcpp.h>

#include<Run/Learn.h>

#include "mixt_MixtComp.h"
#include "SGraphToR.h"
#include "RToSGraph.h"

using namespace mixt;

// [[Rcpp::export]]
Rcpp::List mixtCompCluster(Rcpp::List algoR, Rcpp::List dataR, Rcpp::List descR, Rcpp::List paramR) {
	SGraph algoG = RToSGraph(algoR);
	SGraph dataG = RToSGraph(dataR);
	SGraph descG = RToSGraph(descR);
	SGraph paramG = RToSGraph(paramR);

	SGraph resG = learn(algoG, dataG, descG);

	Rcpp::List resL = SGraphToR(resG);

	return resL;
}
