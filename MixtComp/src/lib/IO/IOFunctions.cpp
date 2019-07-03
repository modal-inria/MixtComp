/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: Aug 9, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "IOFunctions.h"

namespace mixt {

void quantileNames(Index ncol, Real confidenceLevel, std::vector<std::string>& colNames) {
	Real alpha = (1. - confidenceLevel) / 2.;

	if (ncol == 1) { // predict
		colNames.resize(1);
		colNames[0] = "value";
	} else { // learn
		colNames.resize(3);
		colNames[0] = "median";
		colNames[1] = std::string("q ") + std::to_string((alpha * 100.)) + "%";
		colNames[2] = std::string("q ") + std::to_string(((1. - alpha) * 100.)) + "%";
	}
}

void completePath(const std::vector<std::string>& path, const std::string& name, std::string& str) {
	str = "";
	for (std::vector<std::string>::const_iterator it = path.begin(), itEnd = path.end(); it != itEnd; ++it) {
		str += "/" + *it;
	}
	str += "/" + name;
}

}
