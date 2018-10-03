/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 23, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_TRANSLATERTOCPP_H
#define RMIXTCOMP_SRC_TRANSLATERTOCPP_H

#include "Rcpp.h"

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>
#include <LinAlg/names.h>
#include <IO/NamedAlgebra.h>

#include "CPPToRMatrixType.h"
#include "CPPToRVectorType.h"

namespace mixt {

template<typename OutType>
void translateRToCPP(SEXP in, OutType& out) {
	out = Rcpp::as<OutType>(in);
}

template<typename T>
void translateRToCPP(SEXP in, NamedVector<T>& out) {
	typename CPPToRVectorType<T>::ctype temp(in);

	Index nrow = temp.length();
	out.vec_.resize(nrow);

	std::vector<std::string> namesVec;
	SEXP namesR = temp.attr("names");

	if (!Rf_isNull(namesR)) {
		out.rowNames_ = Rcpp::as<std::vector<std::string>>(namesR);
	}

	for (Index i = 0; i < nrow; ++i) {
		out.vec_(i) = temp(i);
	}
}

template<typename T>
void translateRToCPP(SEXP in, NamedMatrix<T>& out) {
	std::cout << "RToCPPTranslate" << std::endl;
	typename CPPToRMatrixType<T>::ctype temp(in);

	Index nrow = temp.rows();
	Index ncol = temp.cols();
	out.mat_.resize(nrow, ncol);

	std::vector<std::string> namesRowVec;
	SEXP namesRowR = rownames(temp);
	if (!Rf_isNull(namesRowR)) {
		out.rowNames_ = Rcpp::as<std::vector<std::string>>(namesRowR);
	}

	std::vector<std::string> namesColVec;
	SEXP namesColR = colnames(temp);
	if (!Rf_isNull(namesColR)) {
		out.colNames_ = Rcpp::as<std::vector<std::string>>(namesColR);
	}

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			out.mat_(i, j) = temp(i, j);
		}
	}
}

//template<typename T>
//void RToCPPTranslate(SEXP in, std::vector<std::vector<T>>& out) {
//	std::cout << "RToCPPTranslate(SEXP in, std::vector<std::vector<T>>& out) not implemented yet." << std::endl;
//}

}

#endif
