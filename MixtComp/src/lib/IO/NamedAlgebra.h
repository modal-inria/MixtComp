/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_IO_NAMEDALGEBRA_H
#define LIB_IO_NAMEDALGEBRA_H

#include <LinAlg/LinAlg.h>

namespace mixt {

template<typename Type>
struct NamedVector {
	NamedVector() {
	}

	NamedVector(const std::vector<std::string>& rowNames, const Vector<Type>& vec) :
			rowNames_(rowNames), vec_(vec) {
	}

	NamedVector(Index nrow, bool named) :
			rowNames_(), vec_(nrow) {
		if (named) {
			rowNames_.resize(nrow);
		}
	}

	std::vector<std::string> rowNames_;
	Vector<Type> vec_;
};

template<typename Type>
struct NamedMatrix {
	NamedMatrix() {
	}

	NamedMatrix(const std::vector<std::string>& rowNames, const std::vector<std::string>& colNames, const Matrix<Type>& mat) :
			rowNames_(rowNames), colNames_(colNames), mat_(mat) {
	}

	NamedMatrix(Index nrow, Index ncol, bool named) :
			rowNames_(), colNames_(), mat_(nrow, ncol) {
		if (named) {
			rowNames_.resize(nrow);
			colNames_.resize(ncol);
		}
	}

	std::vector<std::string> rowNames_;
	std::vector<std::string> colNames_;
	Matrix<Type> mat_;
};

}

#endif
