/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JMixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include "json.hpp"

#include "GraphToJSon.h"
#include "mixt_MixtComp.h"

int main(int argc, char* argv[]) {
    mixt::SGraph graph;
    nlohmann::json j = GraphToJson(graph);

    std::cout << "json mock learn executable OK." << std::endl;

    return 0;
}
