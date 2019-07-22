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
 *  Created on: September 28, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_TRANSLATECPPTOR_H
#define RMIXTCOMP_SRC_TRANSLATECPPTOR_H

#include "Rcpp.h"

#include <iostream>
#include <LinAlg/LinAlg.h>
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

}

#endif
