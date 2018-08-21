/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "JSONGraph.h"

namespace mixt {

void JSONGraph::set(const std::string& s) {
	j_ = nlohmann::json::parse(s);
}

std::string JSONGraph::get() const {
	return j_.dump();
}

template<>
void translation2(const NamedVector2<Real>& in, nlohmann::json& out) {
	Index nrow = in.vec_.size();
	const Real* rawData = in.vec_.data();
	std::vector<Real> data(rawData, rawData + in.vec_.size());

	out["ctype"] = "Vector";
	out["data"] = data;
	out["dtype"] = "Real";
	out["rowNames"] = in.rowNames_;
	out["nrow"] = nrow;
}

template<>
void translation2(const nlohmann::json& in, NamedVector2<Real>& out) {
	Index nrow = in.at("nrow").get<Index>();
	out.vec_.resize(nrow);

	out.rowNames_ = in.at("rowNames").get<std::vector<std::string>>();

	std::vector<Real> dataRaw = in.at("data").get<std::vector<Real>>();
	for (Index i = 0; i < nrow; ++i) {
		out.vec_(i) = dataRaw[i];
	}
}

template<>
void translation2(const NamedMatrix2<Real>& in, nlohmann::json& out) {
	Index nrow = in.mat_.rows();
	Index ncol = in.mat_.cols();
	std::vector<std::vector<Real>> data(nrow, std::vector<Real>(ncol));

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			data[i][j] = in.mat_(i, j);
		}
	}

	out["colNames"] = in.colNames_;
	out["ctype"] = "Matrix";
	out["data"] = data;
	out["dtype"] = "Real";
	out["rowNames"] = in.rowNames_;
	out["ncol"] = ncol;
	out["nrow"] = nrow;
}

template<>
void translation2(const nlohmann::json& in, NamedMatrix2<Real>& out) {
	Index nrow = in.at("nrow").get<Index>();
	Index ncol = in.at("ncol").get<Index>();
	out.mat_.resize(nrow, ncol);

	out.rowNames_ = in.at("rowNames").get<std::vector<std::string>>();
	out.colNames_ = in.at("colNames").get<std::vector<std::string>>();

	std::vector<std::vector<Real>> dataRaw = in.at("data").get<std::vector<std::vector<Real>>>();
	for (Index j = 0; j < ncol; ++j) {
		for (Index i = 0; i < nrow; ++i) {
			out.mat_(i, j) = dataRaw[i][j];
		}
	}
}

}
