/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <LinAlg/Typedef.h>

namespace mixt {

Real toReal(const std::string& s) {
	return std::stod(s);
}

Index toIndex(const std::string& s) {
	return std::stol(s);
}

}
