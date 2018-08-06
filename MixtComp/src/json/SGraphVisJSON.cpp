/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "SGraphVisJSON.h"

#include "mixt_MixtComp.h"

namespace mixt {

SGraphVisJSON::SGraphVisJSON(nlohmann::json& j): j_(j) {};

void SGraphVisJSON::operator()(Index i) const {

}

void SGraphVisJSON::operator()(const Real r) const {

}

void SGraphVisJSON::operator()(const std::string& str) const {

}

void SGraphVisJSON::operator()(const Vector<Real>& vec) const {

}
void SGraphVisJSON::operator()(const Matrix<Real>& mat) const {

}

}
