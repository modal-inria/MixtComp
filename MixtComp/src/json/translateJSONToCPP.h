/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_TRANSLATEJSONTOCPP_H
#define JSON_TRANSLATEJSONTOCPP_H

#include <IO/NamedAlgebra.h>
#include <LinAlg/LinAlg.h>
#include "json.hpp"

#include <iostream>
#include <LinAlg/names.h>

namespace mixt {

template<typename OutType>
void translateJSONToCPP(const nlohmann::json& in, OutType& out) {
	out = in.get<OutType>();
}

template<typename T>
void translateJSONToCPP(const nlohmann::json& in, NamedVector<T>& out) {
	Index nrow = in.at("nrow").get<Index>();
	out.vec_.resize(nrow);

	out.rowNames_ = in.at("rowNames").get<std::vector<std::string>>();

	std::vector<T> dataRaw = in.at("data").get<std::vector<T>>();
	for (Index i = 0; i < nrow; ++i) {
		out.vec_(i) = dataRaw[i];
	}
}

template<typename T>
void translateJSONToCPP(const nlohmann::json& in, NamedMatrix<T>& out) {
	Index nrow = in.at("nrow").get<Index>();
	Index ncol = in.at("ncol").get<Index>();
	out.mat_.resize(nrow, ncol);

	out.rowNames_ = in.at("rowNames").get<std::vector<std::string>>();
	out.colNames_ = in.at("colNames").get<std::vector<std::string>>();

	std::vector<std::vector<T>> dataRaw = in.at("data").get<std::vector<std::vector<T>>>();
	for (Index j = 0; j < ncol; ++j) {
		for (Index i = 0; i < nrow; ++i) {
			out.mat_(i, j) = dataRaw[i][j];
		}
	}
}

}

#endif
