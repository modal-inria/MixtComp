/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_ALGTYPEVISJSON_H
#define JSON_ALGTYPEVISJSON_H

#include "json.hpp"
#include "boost/variant.hpp"

#include <IO/NamedAlgebra.h>

/**
 * Visitor for boost::variant, to perform the correct operation on every payload element in a SGraph
 */
namespace mixt {

class AlgTypeVisJSON: public boost::static_visitor<> {
public:
	AlgTypeVisJSON(nlohmann::json& j, const std::string& payloadName);

	void operator()(Index i) const;

	void operator()(Integer i) const;

	void operator()(const Real r) const;

	void operator()(const std::string& str) const;

	void operator()(const std::vector<std::string>& vec) const;

	void operator()(const NamedVector<Index>& vec) const;

	void operator()(const NamedVector<Integer>& vec) const;

	void operator()(const NamedVector<Real>& vec) const;

	void operator()(const NamedMatrix<Real>& mat) const;

private:
	nlohmann::json& j_;
	const std::string& payloadName_;
};

}

#endif
