/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "SGraph.h"

namespace mixt {

void SGraph::add_payload(const std::string& name, const AlgType& data) {
	std::pair<std::map<std::string, AlgType>::iterator, bool> res = payload_.insert(std::pair<std::string, AlgType>(name, data));
	if (!res.second) throw std::string("Try to add duplicate key ") + name;
}

void SGraph::add_child(const std::string& name, const SGraph& data) {
	std::pair<std::map<std::string, SGraph>::iterator, bool> res = children_.insert(std::pair<std::string, SGraph>(name, data));
	if (!res.second) throw std::string("Try to add duplicate key ") + name;
}

bool SGraph::exist_payload(const std::string& name) const {
	std::map<std::string, AlgType>::const_iterator it = payload_.find(name);
	return it != payload_.end();
}

bool SGraph::exist_child(const std::string& name) const {
	std::map<std::string, SGraph>::const_iterator it = children_.find(name);
	return it != children_.end();
}

void SGraph::listElements() const {
	std::cout << "payload:" << std::endl;
	for (std::map<std::string, AlgType>::const_iterator it = payload_.begin(), itEnd = payload_.end(); it != itEnd; ++it) {
		std::cout << "\t" << it->first << std::endl;
	}

	std::cout << "children:" << std::endl;
	for (std::map<std::string, SGraph>::const_iterator it = children_.begin(), itEnd = children_.end(); it != itEnd; ++it) {
		std::cout << "\t" << it->first << std::endl;
	}
}

}
