/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 29, 2017
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <Various/mixt_Constants.h>

namespace mixt {

bool deterministicMode() {
	static const char *str_ptr = std::getenv(deterministicEnvVar);

	if (str_ptr == NULL) {
		return false;
	} else {
		return true;
	}
}

} // namespace mixt
