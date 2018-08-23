/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 22, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Rcpp.h>

#include "mixt_MixtComp.h"
#include "RGraph.h"

using namespace mixt;

// [[Rcpp::export]]
Rcpp::List rcppTest() {
  RGraph g;
  g.add_payload({"bla", "bla"}, "nameOfPayload", 12.0);
  
	return g.getL();
}
