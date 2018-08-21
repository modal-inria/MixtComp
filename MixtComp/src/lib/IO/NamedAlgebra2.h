/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_IO_NAMEDALGEBRA2_H
#define LIB_IO_NAMEDALGEBRA2_H

#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename Type>
struct NamedVector2 {
	const std::vector<std::string>& rowNames_;
	const Vector<Type>& vec_;
};

template<typename Type>
struct NamedMatrix2 {
	const std::vector<std::string>& rowNames_;
	const std::vector<std::string>& colNames_;
	const Matrix<Type>& mat_;
};

}

#endif
