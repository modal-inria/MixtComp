/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 17, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "AlgTypeVisitor.h"

#include <iostream>

namespace mixt {

template<>
Real translation<Index, Real>(const Index& in) {
	return in;
}

template<>
Index translation<Index, Index>(const Index& in) {
	return in;
}

template<>
Real translation<Real, Real>(const Real& in) {
	return in;
}

template<>
std::string translation<std::string, std::string>(const std::string& in) {
	return in;
}

template<>
std::vector<std::string> translation<std::vector<std::string>, std::vector<std::string>>(const std::vector<std::string>& in) {
	return in;
}

template<>
NamedVector<Real> translation<NamedVector<Real>, NamedVector<Real>>(const NamedVector<Real>& in) {
	return in;
}

template<>
NamedVector<Index> translation<NamedVector<Index>, NamedVector<Index>>(const NamedVector<Index>& in) {
	return in;
}

template<>
NamedVector<Integer> translation<NamedVector<Integer>, NamedVector<Integer>>(const NamedVector<Integer>& in) {
	return in;
}

template<>
NamedMatrix<Real> translation<NamedMatrix<Real>, NamedMatrix<Real>>(const NamedMatrix<Real>& in) {
	return in;
}

}
