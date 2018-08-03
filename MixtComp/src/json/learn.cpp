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
	nlohmann::json j = mixt::GraphToJson(graph);

	nlohmann::json k;
	k["toto"] = "pouet";

	j["test"] = 12.;
	j["test2"] = k;

//    json res_list;
    // build the list

    std::string output_str = j.dump();
    std::cout << output_str << std::endl;

//    std::ofstream out(json_file_output);
//    out << output_str;
//    out.close();

	return 0;
}
