/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 29, 2017
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RNG_H
#define MIXT_RNG_H

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <Various/mixt_Constants.h>

namespace mixt {

template<class T>
std::size_t seed(const T* const p_caller) {
	static Index nCall = 0;
	static const char *str_ptr = std::getenv(deterministicEnvVar);

	if (str_ptr == NULL) {
		return size_t(p_caller) + time(0);
	} else {
		++nCall;
		//	std::cout << "seed, nCall: " << nCall << std::endl;
		return nCall;
	}
}

} // namespace mixt

#endif
