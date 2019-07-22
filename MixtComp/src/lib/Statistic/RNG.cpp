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
