/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JMixtComp
 *  Created on: Aug 3, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include <map>

#include "json.hpp"

#include "mixt_MixtComp.h"

namespace mixt {
/**
 * nlohmann::json is recursive, and mixt::SGraph is recursive too. Translation should be easy.
 */
nlohmann::json GraphToJson(const mixt::SGraph& graph) {
	nlohmann::json res;
	const std::map<std::string, AlgType>& payload = graph.get_payload();
	for (std::map<std::string, AlgType>::const_iterator it = payload.begin(),
			itEnd = payload.end(); it != itEnd; ++it) {
		const std::pair<std::string, AlgType>& val = *it;

		// TODO: convert to json and add to res
	}

	const std::map<std::string, SGraph>& children = graph.get_children();
	for (std::map<std::string, SGraph>::const_iterator it = children.begin(),
			itEnd = children.end(); it != itEnd; ++it) {
		const std::pair<std::string, SGraph>& val = *it;
		const nlohmann::json& child = GraphToJson(val.second);
		res[val.first] = child;
	}

	return res;
}

}
