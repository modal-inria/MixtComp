/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSONTOSGRAPH_H
#define JSONTOSGRAPH_H

#include "json.hpp"

#include <IO/SGraph.h>

namespace mixt {

SGraph JSONToSGraph(const nlohmann::json& graph);

}

#endif
