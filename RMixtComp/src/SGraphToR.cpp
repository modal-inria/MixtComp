/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 10, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Rcpp.h"
#include <IO/SGraph.h>
#include "AlgTypeVisR.h"
#include "SGraphToR.h"

Rcpp::List SGraphToR(const SGraph& graph) {
	Rcpp::List res;

	const std::map<std::string, AlgType>& payload = graph.get_payload();
	for (std::map<std::string, AlgType>::const_iterator it = payload.begin(),
			itEnd = payload.end(); it != itEnd; ++it) {
		const std::pair<std::string, AlgType>& val = *it;
		boost::apply_visitor(AlgTypeVisR(res, val.first), val.second);
	}

	const std::map<std::string, SGraph>& children = graph.get_children();
	for (std::map<std::string, SGraph>::const_iterator it = children.begin(),
			itEnd = children.end(); it != itEnd; ++it) {
		const std::pair<std::string, SGraph>& val = *it;
		res[val.first] = SGraphToR(val.second);
	}

	return res;
}
