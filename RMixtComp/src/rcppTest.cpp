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

Rcpp::List modifyList(Rcpp::List l) {
  l["x"] = "x";
  return l;
}

/**
 * https://stackoverflow.com/questions/52006424/modifying-a-subsection-of-an-rcpplist-in-a-separate-function-by-reference
 */
// [[Rcpp::export]]
Rcpp::List rcppTest() {
  Rcpp::List res;
  res["a"] = Rcpp::List::create();
  
  res["a"] = modifyList(res["a"]);
  
  return res;
}

// [[Rcpp::export]]
Rcpp::List rcppTest2(Rcpp::List l) {
  
  RGraph g(l);
  g.add_payload({"a", "b"}, "payload", 23.0);
  Rcpp::List res = g.getL();
  
  std::cout << "end of computation" << std::endl;
  
  return res;
}