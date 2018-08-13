/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 13, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "IOFunctions.h"

namespace mixt {

SGraph dummySGraph(const SGraph& desc) {
	SGraph res;

	for (std::map<std::string, SGraph>::const_iterator it = desc.get_children().begin(), itEnd =desc.get_children().end(); it != itEnd; ++it) {
		res.add_child(it->first, SGraph());
	}

	return res;
}

}
