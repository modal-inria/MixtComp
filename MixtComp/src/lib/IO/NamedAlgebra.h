/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

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
