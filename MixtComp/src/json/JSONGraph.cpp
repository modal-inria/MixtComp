/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "JSONGraph.h"

namespace mixt {

void JSONGraph::set(const std::string& s) {
	j_ = nlohmann::json::parse(s);
}

std::string JSONGraph::get() const {
	return j_.dump();
}

bool JSONGraph::exist_payload(const std::vector<std::string>& path, const std::string& name) const {
	return exist_payload(path, 0, j_, name);
}

bool JSONGraph::exist_payload(const std::vector<std::string>& path, Index currDepth, const nlohmann::json& currLevel, const std::string& name) const {
	if (currDepth == path.size()) {
		if (currLevel[name].is_null()) {
			return false;
		}
		return true;
	} else {
		const nlohmann::json& nextLevel = currLevel[path[currDepth]];
		if (nextLevel.is_null()) { // if next level does not exist, create it
			return 0;
		}

		return exist_payload(path, currDepth + 1, nextLevel, name);
	}
}

}

}
