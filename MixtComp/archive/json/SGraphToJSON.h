/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SGRAPHTOJSON_H
#define SGRAPHTOJSON_H

#include "json.hpp"

#include <IO/SGraph.h>

namespace mixt {

/**
 * nlohmann::json is recursive, and mixt::SGraph is recursive too. Translation should be easy.
 */
nlohmann::json SGraphToJSON(const SGraph& graph);

}

#endif
