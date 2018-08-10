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
	payload_.insert(std::pair<std::string, AlgType>(name, data));
}

void SGraph::add_child(const std::string& name, const SGraph& data) {
	children_.insert(std::pair<std::string, SGraph>(name, data));
}

bool SGraph::exist_payload(const std::string& name) const {
	std::map<std::string, AlgType>::const_iterator it = payload_.find(name);
	return it != payload_.end();
}

bool SGraph::exist_child(const std::string& name) const {
	std::map<std::string, SGraph>::const_iterator it = children_.find(name);
	return it != children_.end();
}

}
