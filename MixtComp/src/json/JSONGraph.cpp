/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "JSONGraph.h"

namespace mixt {

JSONGraph::JSONGraph(const nlohmann::json& j) :
		j_(j) {
}

void JSONGraph::set(const nlohmann::json& j) {
	j_ = j;
}

void JSONGraph::set(const std::string& s) {
	j_ = nlohmann::json::parse(s);
}

std::string JSONGraph::get() const {
	return j_.dump();
}

void JSONGraph::getSubGraph(const std::vector<std::string>& path, JSONGraph& j) const {
	nlohmann::json l;
	go_to(path, l);

	j.set(l);
}

bool JSONGraph::exist_payload(const std::vector<std::string>& path, const std::string& name) const {
	nlohmann::json l;
	go_to(path, l);

	if (l[name].is_null()) {
		return false;
	}

	return true;
}

void JSONGraph::go_to(const std::vector<std::string>& path, nlohmann::json& l) const {
	go_to(path, 0, j_, l);
}

void JSONGraph::go_to(const std::vector<std::string>& path, Index currDepth, const nlohmann::json& currLevel, nlohmann::json& l) const {
	if (currDepth == path.size()) {
		l = currLevel;
	} else {
		const nlohmann::json& nextLevel = currLevel[path[currDepth]];
		if (nextLevel.is_null()) { // if next level does not exist, create it
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i) {
				askedPath += +"/" + path[i];
			}
			throw(askedPath + " path does not exist.");
		}

		go_to(path, currDepth + 1, nextLevel, l);
	}
}

void JSONGraph::addSubGraph(const std::vector<std::string>& path, const std::string& name, const JSONGraph& p) {
	addSubGraph(path, 0, j_, name, p);
}

void JSONGraph::addSubGraph(const std::vector<std::string>& path, Index currDepth, nlohmann::json& currLevel, const std::string& name, const JSONGraph& p) {
	if (currDepth == path.size()) { // currLevel is the right element in path, add the sub graph
		currLevel[name] = p.getJ();
	} else {
		nlohmann::json& nextLevel = currLevel[path[currDepth]];

		if (nextLevel.is_null()) { // if next level does not exist, create it
			nextLevel = nlohmann::json();
		} else if (!nextLevel.is_object()) { // if it already exists but is not a json object, throw an exception
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i) {
				askedPath + "/" + path[i];
			}
			throw(askedPath + " already exists and is not a json object.");
		}

		addSubGraph(path, currDepth + 1, nextLevel, name, p);
	}
}

void JSONGraph::name_payload(const std::vector<std::string>& path, std::list<std::string>& l) const {
	nlohmann::json j;
	go_to(path, j);

	for (nlohmann::json::const_iterator it = j.begin(); it != j.end(); ++it) {
		l.push_back(it.key());
	}
}

}
