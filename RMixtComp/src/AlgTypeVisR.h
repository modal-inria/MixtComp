/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 20, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef ALGTYPEVISR_H
#define ALGTYPEVISR_H

#include "Rcpp.h"
#include "boost/variant.hpp"

#include <IO/NamedAlgebra.h>

using namespace mixt;

/**
 * Visitor for boost::variant, to perform the correct operation on every payload element in a SGraph
 */
class AlgTypeVisR: public boost::static_visitor<> {
public:
	AlgTypeVisR(Rcpp::List& l, const std::string& payloadName);

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
	Rcpp::List& l_;
	const std::string& payloadName_;
};

#endif
