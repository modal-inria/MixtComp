/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_CPPTORVECTORTYPE
#define RMIXTCOMP_SRC_CPPTORVECTORTYPE

#include <Rcpp.h>
#include <LinAlg/LinAlg.h>

namespace mixt {

template<typename T>
class CPPToRMatrixType {};

template<>
class CPPToRMatrixType<int> {
public:
	typedef Rcpp::IntegerMatrix ctype;
};

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
