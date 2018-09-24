/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_CPPTORVECTORTYPE
#define RMIXTCOMP_SRC_CPPTORVECTORTYPE

#include <Rcpp.h>
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename T>
class CPPToRMatrixType {};

template<>
class CPPToRMatrixType<Index> {
public:
	typedef Rcpp::IntegerMatrix ctype;
};

template<>
class CPPToRMatrixType<Integer> {
public:
	typedef Rcpp::IntegerMatrix ctype;
};

template<>
class CPPToRMatrixType<Real> {
public:
	typedef Rcpp::NumericMatrix ctype;
};

}

#endif
