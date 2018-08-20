/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 10, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SGRAPHTOR_H
#define SGRAPHTOR_H

#include "Rcpp.h"
#include <IO/SGraph.h>

using namespace mixt;

Rcpp::List SGraphToR(const SGraph& graph);

#endif
