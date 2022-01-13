/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

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
