/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "SGraphVisJSON.h"

namespace mixt {

SGraphVisJSON::SGraphVisJSON(nlohmann::json& j, const std::string& payloadName) :
		j_(j), payloadName_(payloadName) {
}
;

void SGraphVisJSON::operator()(Index i) const {
	j_[payloadName_] = i;
}

void SGraphVisJSON::operator()(const Real r) const {
	j_[payloadName_] = r;
}

void SGraphVisJSON::operator()(const std::string& str) const {
	j_[payloadName_] = str;
}

void SGraphVisJSON::operator()(const Vector<Real>& vec) const {
}

void SGraphVisJSON::operator()(const Matrix<Real>& mat) const {
}

}
