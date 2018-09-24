/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 23, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_RTOCPPTRANSLATE_H
#define RMIXTCOMP_SRC_RTOCPPTRANSLATE_H

#include "Rcpp.h"

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>
#include <LinAlg/names.h>
#include <IO/NamedAlgebra.h>

#include "CPPToRMatrixType.h"
#include "CPPToRVectorType.h"

namespace mixt {

template<typename OutType>
void RToCPPTranslate(SEXP in, OutType& out) {
	out = Rcpp::as<OutType>(in);
}

template<typename T>
void RToCPPTranslate(SEXP in, NamedVector<T>& out) {
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
void RToCPPTranslate(SEXP in, NamedMatrix<T>& out) {
	typename CPPToRMatrixType<T>::ctype temp(in);

	Index nrow = temp.rows();
	Index ncol = temp.cols();
	out.mat_.resize(nrow, ncol);

	std::vector<std::string> namesRowVec;
	std::vector<std::string> namesColVec;
	SEXP namesRowR = temp.attr("rownames");
	SEXP namesColR = temp.attr("colnames");

	if (!Rf_isNull(namesRowR)) {
		out.rowNames_ = Rcpp::as<std::vector<std::string>>(namesRowR);
	}

	if (!Rf_isNull(namesColR)) {
		out.colNames_ = Rcpp::as<std::vector<std::string>>(namesColR);
	}

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			out.mat_(i, j) = temp(i, j);
		}
	}
}

}

#endif
