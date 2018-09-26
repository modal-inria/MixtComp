/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_CPPTORTRANSLATE_H
#define RMIXTCOMP_SRC_CPPTORTRANSLATE_H

#include "Rcpp.h"

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>
#include <LinAlg/names.h>
#include <IO/NamedAlgebra.h>

#include "CPPToRMatrixType.h"
#include "CPPToRVectorType.h"

namespace mixt {

template<typename InType>
void CPPToRTranslate(const InType& in, SEXP& out) {
	out = Rcpp::wrap(in);
}

template<typename T>
void CPPToRTranslate(const NamedVector<T>& in, SEXP& out) {
	Index nrow = in.vec_.size();
	typename CPPToRVectorType<T>::ctype temp(nrow);

	for (Index i = 0; i < nrow; ++i) {
		temp[i] = in.vec_(i);
	}

	if (in.rowNames_.size() != 0) {
		Rcpp::CharacterVector tempName(nrow);
		for (Index i = 0; i < nrow; ++i) {
			tempName(i) = in.rowNames_[i];
		}
		temp.attr("names") = tempName;
	}

	out = temp;
}

template<typename T>
void CPPToRTranslate(const NamedMatrix<T>& in, SEXP& out) {
	Index nrow = in.mat_.rows();
	Index ncol = in.mat_.cols();

	typename CPPToRMatrixType<T>::ctype temp(nrow, ncol);

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			temp(i, j) = in.mat_(i, j);
		}
	}

	if (in.rowNames_.size() != 0) {
		Rcpp::CharacterVector tempName(nrow);
		for (Index i = 0; i < nrow; ++i) {
			tempName[i] = in.rowNames_[i];
		}
		temp.attr("rownames") = tempName;
	}

	if (in.colNames_.size() != 0) {
		Rcpp::CharacterVector tempName(ncol);
		for (Index i = 0; i < ncol; ++i) {
			tempName[i] = in.colNames_[i];
		}
		temp.attr("colnames") = tempName;
	}

	out = temp;
}

}

#endif
