/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 10, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RTOSGRAPH_H
#define RTOSGRAPH_H

#include "Rcpp.h"
#include <IO/SGraph.h>

using namespace mixt;

SGraph RToSGraph(const Rcpp::List& ls);

void addCharacterVector(const std::string& name, SEXP s, SGraph& g);

#endif
