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
void translation<Index, Real>(const Index& in, Real& out) {
	out = in; // use implicit conversion
}

template<>
void translation<Index, Index>(const Index& in, Index& out) {
	out = in;
}

template<>
void translation<Real, Real>(const Real& in, Real& out) {
	out = in;
}

template<>
void translation<std::string, std::string>(const std::string& in, std::string& out) {
	out = in;
}

template<>
void translation<NamedVector<Real>, NamedVector<Real>>(const NamedVector<Real>& in, NamedVector<Real>& out) {
	out = in;
}

template<>
void translation<NamedVector<Index>, NamedVector<Index>>(const NamedVector<Index>& in, NamedVector<Index>& out) {
	out = in;
}

template<>
void translation<NamedMatrix<Real>, NamedMatrix<Real>>(const NamedMatrix<Real>& in, NamedMatrix<Real>& out) {
	out = in;
}

}
