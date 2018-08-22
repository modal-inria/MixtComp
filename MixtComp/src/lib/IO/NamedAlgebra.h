/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_IO_NAMEDALGEBRA_H
#define LIB_IO_NAMEDALGEBRA_H

#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename Type>
struct NamedVector {
	std::vector<std::string> rowNames_;
	Vector<Type> vec_;
};

template<typename Type>
struct NamedMatrix {
	std::vector<std::string> rowNames_;
	std::vector<std::string> colNames_;
	Matrix<Type> mat_;
};

}

#endif
