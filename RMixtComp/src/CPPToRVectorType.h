/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_CPPTORMATRIXTYPE
#define RMIXTCOMP_SRC_CPPTORMATRIXTYPE

#include <Rcpp.h>
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename T>
class CPPToRVectorType {};

template<>
class CPPToRVectorType<Index> {
public:
	typedef Rcpp::IntegerVector ctype;
};

template<>
class CPPToRVectorType<Integer> {
public:
	typedef Rcpp::IntegerVector ctype;
};

template<>
class CPPToRVectorType<Real> {
public:
	typedef Rcpp::NumericVector ctype;
};

}

#endif
