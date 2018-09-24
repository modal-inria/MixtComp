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
#include "types.h"

namespace mixt {

template<typename InType>
void CPPToRTranslate(const InType& in, SEXP& out) {
	out = Rcpp::wrap(in);
}

template<typename T>
void CPPToRTranslate(const NamedVector<T>& in, SEXP& out) {
//	std::cout << "CPPToRTranslate, to be implemented." << std::endl;
	// out = Rcpp::wrap(in);
	out = Rcpp::wrap("To be implemented");
}

}

#endif
