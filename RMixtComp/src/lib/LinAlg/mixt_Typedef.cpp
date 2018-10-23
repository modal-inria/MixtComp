/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Typedef.h"

namespace mixt {

Real toReal(const std::string& s) {
	return std::stod(s);
}

Index toIndex(const std::string& s) {
	return std::stol(s);
}

}
