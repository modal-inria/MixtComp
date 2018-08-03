/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 3, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef ALGTYPE_H
#define ALGTYPE_H

#include "boost/variant.hpp"
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

/**
 * boost::variant provides the equivalent of algebraic type. The main advantage vs polymorphism is that the syntax is lighter. The serialization graph is only used to carry the data from the depths of the model to
 * the IO module. Therefore using polymorphism would be overkill.
 * - Index
 * - Real
 * - std::string
 * - Matrix<Real>
 */
typedef typename boost::variant<Index, Real, std::string, Vector<Real>,
		Matrix<Real>> AlgType;

}

template<typename Type>
struct NamedVector {
	std::string rowNames_;
	Vector<Type> vec_;
};

template<typename Type>
struct NamedMatrix {
	std::string rowNames_;
	std::string colNames_;
	Matrix<Type> vec_;
};

#endif
