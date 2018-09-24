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
#include "types.h"

namespace mixt {

template<typename OutType>
void RToCPPTranslate(SEXP in, OutType& out) {
	out = Rcpp::as<OutType>(in);
}

template<typename T>
void RToCPPTranslate(SEXP in, NamedVector<T>& out) {
	typename types<T>::ctype temp(in);

	Index nrow = temp.length();
	out.vec_.resize(nrow);

	std::vector<std::string> namesVec;
	SEXP namesR = temp.attr("names");

	if (!Rf_isNull(namesR)) {
		out.rowNames_ = Rcpp::as<std::vector<std::string>>(namesR);
	}

	std::vector<T> dataRaw = Rcpp::as<std::vector<T>>(temp);
	for (Index i = 0; i < nrow; ++i) {
		out.vec_(i) = dataRaw[i];
	}
}

}

#endif
