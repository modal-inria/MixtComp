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
OutType translation(const InType& in) {
	throw(std::string("Impossible conversion  between types."));
}

/**
 * Visitor used for conversion of an AlgType.
 * Not that input and output types are templated. This way, it is not necessary to generate one visitor per output type.
 * Even better, the operator is translated, and only specialized translation function have to be supplied for the necessary cases only.
 * The trivial translation is also taken care of :).
 */
template<typename OutType>
class AlgTypeVisitor: public boost::static_visitor<OutType> {
public:
	template<typename InType>
	OutType operator()(const InType& i) const {
		return translation<InType, OutType>(i);
	}
};

// non trivial translation

template<>
Real translation<Index, Real>(const Index& in);

// trivial translations
// Index, Real, std::string, std::vector<std::string>, NamedVector<Real>, NamedVector<Index>, NamedVector<Integer>, NamedMatrix<Real>

template<>
Index translation<Index, Index>(const Index& in);

template<>
Real translation<Real, Real>(const Real& in);

template<>
std::string translation<std::string, std::string>(const std::string& in);

template<>
std::vector<std::string> translation<std::vector<std::string>, std::vector<std::string>>(const std::vector<std::string>& in);

template<>
NamedVector<Real> translation<NamedVector<Real>, NamedVector<Real>>(const NamedVector<Real>& in);

template<>
NamedVector<Index> translation<NamedVector<Index>, NamedVector<Index>>(const NamedVector<Index>& in);

template<>
NamedVector<Integer> translation<NamedVector<Integer>, NamedVector<Integer>>(const NamedVector<Integer>& in);

template<>
NamedMatrix<Real> translation<NamedMatrix<Real>, NamedMatrix<Real>>(const NamedMatrix<Real>& in);

}

#endif
