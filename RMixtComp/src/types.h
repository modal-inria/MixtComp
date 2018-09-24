/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_TYPES
#define RMIXTCOMP_SRC_TYPES

#include <Rcpp.h>
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename T>
class types {};

template<>
class types<Index> {
public:
	typedef Rcpp::IntegerVector ctype;
};

template<>
class types<Integer> {
public:
	typedef Rcpp::IntegerVector ctype;
};

template<>
class types<Real> {
public:
	typedef Rcpp::NumericVector ctype;
};

}

#endif
