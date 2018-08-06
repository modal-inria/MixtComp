/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JMixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "SGraphVisPrint.h"
#include "mixt_MixtComp.h"

namespace mixt {

std::string SGraphVisPrint::operator()(Index i) const {
	return std::to_string(i);
}

std::string SGraphVisPrint::operator()(const Real r) const {
	return std::to_string(r);
}

std::string SGraphVisPrint::operator()(const std::string& str) const {
	return str;
}

std::string SGraphVisPrint::operator()(const NamedVector<Real>& vec) const {
	//return itString(vec);
	return "to be implemented";
}

std::string SGraphVisPrint::operator()(const NamedMatrix<Real>& mat) const {
	//return std::to_string(mat);
	return "to be implemented";
}

}
