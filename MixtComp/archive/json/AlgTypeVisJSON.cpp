/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <AlgTypeVisJSON.h>

namespace mixt {

AlgTypeVisJSON::AlgTypeVisJSON(nlohmann::json& j, const std::string& payloadName) :
		j_(j), payloadName_(payloadName) {
}

void AlgTypeVisJSON::operator()(Index i) const {
	j_[payloadName_] = i;
}

void AlgTypeVisJSON::operator()(Integer i) const {
	j_[payloadName_] = i;
}

void AlgTypeVisJSON::operator()(const Real r) const {
	j_[payloadName_] = r;
}

void AlgTypeVisJSON::operator()(const std::string& str) const {
	j_[payloadName_] = str;
}

void AlgTypeVisJSON::operator()(const std::vector<std::string>& vec) const {
	j_[payloadName_] = vec;
}

void AlgTypeVisJSON::operator()(const NamedVector<Index>& vec) const {
	nlohmann::json res; // Since a vector is an union of fields, it is contained in its own json object

	Index nrow = vec.vec_.size();
	const Index* rawData = vec.vec_.data();
	std::vector<Index> data(rawData, rawData + vec.vec_.size());

	res["ctype"] = "Vector";
	res["data"] = data;
	res["dtype"] = "Index";
	res["rowNames"] = vec.rowNames_;
	res["nrow"] = nrow;

	j_[payloadName_] = res;
}

void AlgTypeVisJSON::operator()(const NamedVector<Integer>& vec) const {
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

void AlgTypeVisJSON::operator()(const NamedVector<Real>& vec) const {
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

void AlgTypeVisJSON::operator()(const NamedMatrix<Real>& mat) const {
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
