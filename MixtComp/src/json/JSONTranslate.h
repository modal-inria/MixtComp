/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_JSONTRANSLATE_H
#define JSON_JSONTRANSLATE_H

#include <IO/NamedAlgebra.h>
#include "json.hpp"

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename InType, typename OutType>
void JSONTranslate(const InType& in, OutType& out) {
	out = in;
}

template<>
void JSONTranslate(const NamedVector<Real>& in, nlohmann::json& out);

template<>
void JSONTranslate(const nlohmann::json& in, NamedVector<Real>& out);

template<>
void JSONTranslate(const NamedMatrix<Real>& in, nlohmann::json& out);

template<>
void JSONTranslate(const nlohmann::json& in, NamedMatrix<Real>& out);

/**
 * Specialization necessary to remove ambiguity on the = operator used for vector.
 */
template<>
void JSONTranslate(const nlohmann::json& in, std::vector<std::string>& out);

}

#endif
