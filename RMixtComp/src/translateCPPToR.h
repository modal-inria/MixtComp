/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_TRANSLATECPPTOR_H
#define RMIXTCOMP_SRC_TRANSLATECPPTOR_H

#include "Rcpp.h"

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>
#include <LinAlg/names.h>
#include <IO/NamedAlgebra.h>

#include "CPPToRMatrixType.h"
#include "CPPToRVectorType.h"

namespace mixt {

template<typename InType>
void translateCPPToR(const InType& in, SEXP& out) {
	out = Rcpp::wrap(in);
}

template<typename T>
void translateCPPToR(const NamedVector<T>& in, SEXP& out) {
	Index nrow = in.vec_.size();
	typename CPPToRVectorType<T>::ctype temp(nrow);

	for (Index i = 0; i < nrow; ++i) {
		temp[i] = in.vec_(i);
	}

	if (in.rowNames_.size() != 0) {
		temp.attr("names") = in.rowNames_;
	}

	out = temp;
}

template<typename T>
void translateCPPToR(const NamedMatrix<T>& in, SEXP& out) {
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
		Rcpp::rownames(temp) = tempName;
	}

	if (in.colNames_.size() != 0) {
		Rcpp::CharacterVector tempName(ncol);
		for (Index i = 0; i < ncol; ++i) {
			tempName[i] = in.colNames_[i];
		}
		Rcpp::colnames(temp) = tempName;
	}

	out = temp;
}

template<typename T>
void translateCPPToR(const std::vector<std::vector<T>>& in, SEXP& out) {
	std::cout << "CPPToRTranslate(const std::vector<std::vector<T>>& in, SEXP& out) not implemented yet." << std::endl;
}

}

#endif
