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
 *  Created on: August 23, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_TRANSLATERTOCPP_H
#define RMIXTCOMP_SRC_TRANSLATERTOCPP_H

#include "Rcpp.h"

#include <iostream>
#include <LinAlg/LinAlg.h>
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

}

#endif
