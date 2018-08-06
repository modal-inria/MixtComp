/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 6, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef SGRAPHVISJSON_H
#define SGRAPHVISJSON_H

#include "json.hpp"
#include "mixt_MixtComp.h"

/**
 * Visitor for boost::variant, to perform the correct operation on every payload element in a SGraph
 */
namespace mixt {

class SGraphVisJSON: public boost::static_visitor<> {
public:
	SGraphVisJSON(nlohmann::json& j);

	void operator()(Index i) const;

	void operator()(const Real r) const;

	void operator()(const std::string& str) const;

	void operator()(const Vector<Real>& vec) const;

	void operator()(const Matrix<Real>& mat) const;

private:
	nlohmann::json& j_;
};

}

#endif