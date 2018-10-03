/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_TRANSLATECPPTOJSON_H
#define JSON_TRANSLATECPPTOJSON_H

#include <IO/NamedAlgebra.h>
#include "json.hpp"

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>
#include <LinAlg/names.h>

namespace mixt {

template<typename InType>
void translateCPPToJSON(const InType& in, nlohmann::json& out) {
	out = in;
}

template<typename T>
void translateCPPToJSON(const NamedVector<T>& in, nlohmann::json& out) {
	Index nrow = in.vec_.size();
	const T* rawData = in.vec_.data();
	std::vector<T> data(rawData, rawData + in.vec_.size());

	out["ctype"] = "Vector";
	out["data"] = data;
	out["dtype"] = names<T>::name;
	out["rowNames"] = in.rowNames_;
	out["nrow"] = nrow;
}

template<typename T>
void translateCPPToJSON(const NamedMatrix<T>& in, nlohmann::json& out) {
	Index nrow = in.mat_.rows();
	Index ncol = in.mat_.cols();
	std::vector<std::vector<T>> data(nrow, std::vector<Real>(ncol));

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			data[i][j] = in.mat_(i, j);
		}
	}

	out["colNames"] = in.colNames_;
	out["ctype"] = "Matrix";
	out["data"] = data;
	out["dtype"] = names<T>::name;
	out["rowNames"] = in.rowNames_;
	out["ncol"] = ncol;
	out["nrow"] = nrow;
}

}

#endif
