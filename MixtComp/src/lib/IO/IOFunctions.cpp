/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
		colNames[0] = "value";
	} else { // learn
		colNames[0] = "median";
		colNames[1] = std::string("q ") + std::to_string((alpha * 100.)) + "%";
		colNames[2] = std::string("q ") + std::to_string(((1. - alpha) * 100.)) + "%";
	}
}

}
