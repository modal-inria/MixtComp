/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 20, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "AlgTypeVisR.h"

AlgTypeVisR::AlgTypeVisR(Rcpp::List& l, const std::string& payloadName) :
		l_(l), payloadName_(payloadName) {
}

void AlgTypeVisR::operator()(Index i) const {
	l_[payloadName_] = i;
}

void AlgTypeVisR::operator()(const Real r) const {
	l_[payloadName_] = r;
}

void AlgTypeVisR::operator()(const std::string& str) const {
	l_[payloadName_] = str;
}

void AlgTypeVisR::operator()(const std::vector<std::string>& vec) const {
	l_[payloadName_] = vec;
}

void AlgTypeVisR::operator()(const NamedVector<Index>& vec) const {
	std::cout << "not implemented yet" << std::endl;
}

void AlgTypeVisR::operator()(const NamedVector<Integer>& vec) const {
	std::cout << "not implemented yet" << std::endl;
}

void AlgTypeVisR::operator()(const NamedVector<Real>& vec) const {
	std::cout << "not implemented yet" << std::endl;
}

void AlgTypeVisR::operator()(const NamedMatrix<Real>& mat) const {
	std::cout << "not implemented yet" << std::endl;
}
