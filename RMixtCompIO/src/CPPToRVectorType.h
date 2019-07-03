/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_CPPTORMATRIXTYPE
#define RMIXTCOMP_SRC_CPPTORMATRIXTYPE

#include <Rcpp.h>
#include <LinAlg/LinAlg.h>

namespace mixt {

template<typename T>
class CPPToRVectorType {};

template<>
class CPPToRVectorType<int> {
public:
	typedef Rcpp::IntegerVector ctype;
};

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
