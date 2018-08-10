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

void SGraphVisJSON::operator()(const std::vector<std::string>& vec) const {
	j_[payloadName_] = vec;
}

void SGraphVisJSON::operator()(const NamedVector<Integer>& vec) const {
	nlohmann::json res; // Since a vector is an union of fields, it is contained in its own json object

	Index nrow = vec.vec_.size();
	const Integer* rawData = vec.vec_.data();
	std::vector<Integer> data(rawData, rawData + vec.vec_.size());

	res["ctype"] = "Vector";
	res["data"] = data;
	res["dtype"] = "Integer";
	res["rowNames"] = vec.rowNames_;
	res["nrow"] = nrow;

	j_[payloadName_] = res;
}

void SGraphVisJSON::operator()(const NamedVector<Real>& vec) const {
	nlohmann::json res; // Since a vector is an union of fields, it is contained in its own json object

	Index nrow = vec.vec_.size();
	const Real* rawData = vec.vec_.data();
	std::vector<Real> data(rawData, rawData + vec.vec_.size());

	res["ctype"] = "Vector";
	res["data"] = data;
	res["dtype"] = "Real";
	res["rowNames"] = vec.rowNames_;
	res["nrow"] = nrow;

	j_[payloadName_] = res;
}

void SGraphVisJSON::operator()(const NamedMatrix<Real>& mat) const {
	nlohmann::json res; // Since a matrix is an union of fields, it is contained in its own json object

	Index nrow = mat.mat_.rows();
	Index ncol = mat.mat_.cols();
	std::vector<std::vector<Real>> data(nrow, std::vector<Real>(ncol));

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			data[i][j] = mat.mat_(i, j);
		}
	}

	res["colNames"] = mat.colNames_;
	res["ctype"] = "Matrix";
	res["data"] = data;
	res["dtype"] = "Real";
	res["rowNames"] = mat.rowNames_;
	res["ncol"] = ncol;
	res["nrow"] = nrow;

	j_[payloadName_] = res;
}

}
