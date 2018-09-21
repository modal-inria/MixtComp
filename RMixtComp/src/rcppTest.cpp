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
 *
 *   input <- list(
    a = list(
      a1 = "riri",
      a2 = "fifi",
      a3 = "loulou"),
    b = list(
      b1 = list(
        b11 = "dingo"
      )
    )
  )
 */
// [[Rcpp::export]]
Rcpp::List rcppTest() {
  RGraph rg;
  
  rg.add_payload({"a"}, "a1", "riri");
  rg.add_payload({"a"}, "a2", "fifi");
  rg.add_payload({"a"}, "a3", "loulou");
  rg.add_payload({"b", "b1"}, "b11", "dingo");
  
  return rg.getL();
}

// [[Rcpp::export]]
Rcpp::List rcppTest2(Rcpp::List l) {
  
  RGraph g(l);
  g.add_payload({"a", "b"}, "payload", 23.0);
  Rcpp::List res = g.getL();
  
  std::cout << "end of computation" << std::endl;
  
  return res;
}

// [[Rcpp::export]]
Rcpp::List rcppTest3() {
  RGraph rg;

  rg.add_payload({"path", "to", "enlightenment"}, "valueName" , "toto");

  return rg.getL();
}
