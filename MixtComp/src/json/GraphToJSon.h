/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JMixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef GRAPHTOJSON_H
#define GRAPHTOJSON_H

#include "json.hpp"
#include "mixt_MixtComp.h"

namespace mixt {

nlohmann::json GraphToJson(const mixt::SGraph& graph);

}

#endif
