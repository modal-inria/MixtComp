/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 29, 2017
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RNG_H
#define RNG_H

#include <iostream>
#include <time.h>

#include <Various/Constants.h>

namespace mixt {

bool deterministicMode();

int deterministicInitialSeed();

template<class T>
std::size_t seed(const T* const p_caller) {
	static Index nCall = deterministicInitialSeed();

	if (deterministicMode() == false) {
		return size_t(p_caller) + time(0);
	} else {
		++nCall;
		//	std::cout << "seed, nCall: " << nCall << std::endl;
		return nCall;
	}
}

} // namespace mixt

#endif
