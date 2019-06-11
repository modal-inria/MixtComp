/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 29, 2017
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Various/Constants.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>


namespace mixt {

bool deterministicMode() {
	static const char *str_ptr = std::getenv(deterministicEnvVar);
	static bool detMode = (str_ptr == NULL) ? (false) : (true);
	return detMode;
}

int deterministicInitialSeed() {
	const char *str_ptr = std::getenv(deterministicEnvVar);

	if (str_ptr == NULL) {
		return 0;
	} else {
		int seed = std::atoi(str_ptr);
#ifdef MC_VERBOSE
		std::cout << "Initial random seed value: " << seed << std::endl;
#endif
		return seed;
	}
}

} // namespace mixt
