/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

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
