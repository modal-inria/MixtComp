/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 17, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef ALGTYPEVISITOR_H
#define ALGTYPEVISITOR_H

#include "boost/variant.hpp"
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

#include <LinAlg/mixt_LinAlg.h>

#include "NamedAlgebra.h"

namespace mixt {

/**
 * Either trivial or no conversion.
 * Non trivial conversion performed in function template specialization.
 */
template<typename InType, typename OutType>
void translation(const InType& in, OutType& out) {
	throw(std::string("Impossible conversion  between types."));
}

/**
 * Visitor used for conversion of an AlgType.
 * Not that input and output types are templated. This way, it is not necessary to generate one visitor per output type.
 * Even better, the operator is translated, and only specialized translation function have to be supplied for the necessary cases only.
 * The trivial translation is also taken care of :).
 */
template<typename OutType>
class AlgTypeVisitor: public boost::static_visitor<> {
public:
	AlgTypeVisitor(OutType& out) :
			out_(out) {
	}
	;

	template<typename InType>
	void operator()(const InType& i) const {
		translation<InType, OutType>(i, out_);
	}

private:
	OutType& out_;
};

// Trivial specializations

template<>
void translation<Index, Index>(const Index& in, Index& out);

template<>
void translation<Real, Real>(const Real& in, Real& out);

template<>
void translation<std::string, std::string>(const std::string& in, std::string& out);

template<>
void translation<NamedVector<Real>, NamedVector<Real>>(const NamedVector<Real>& in, NamedVector<Real>& out);

template<>
void translation<NamedVector<Index>, NamedVector<Index>>(const NamedVector<Index>& in, NamedVector<Index>& out);

template<>
void translation<NamedMatrix<Real>, NamedMatrix<Real>>(const NamedMatrix<Real>& in, NamedMatrix<Real>& out);

}

#endif
