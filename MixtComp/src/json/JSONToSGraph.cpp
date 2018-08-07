/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>

#include "JSONToSGraph.h"

namespace mixt {

SGraph JSONToSGraph(const nlohmann::json& json) {
	SGraph res;

	if (json.find("dtype") != json.end()) { // if there is a dtype string, that means that the current json represents a NamesAlgebra object, which requires a particular parsing
			// parse vector / matrix
	}

	for (nlohmann::json::const_iterator it = json.begin(); it != json.end();
			++it) { // if json is not a NamedAlgebra, simply loop over all elements

		if ((*it).is_object()) { // if object is a json, recursively call JSONToSGraph
			SGraph converted = JSONToSGraph(*it);
			res.add_child(it.key(), converted);
		} else if ((*it).is_number_unsigned()) { // Index
			Index val = (*it).get<Index>();
			res.add_payload(it.key(), val);
		} else if ((*it).is_number_float()) { // Real
			Real val = (*it).get<Real>();
			res.add_payload(it.key(), val);
		} else if ((*it).is_string()) { // std::string
			std::string val = (*it).get<std::string>();
			res.add_payload(it.key(), val);
		}

	}

	return res;
}

}
