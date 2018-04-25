/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March the 21st, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cstdlib>
#include <iostream>

/**
 * Check how to get environment variables.
 */
int main() {
//	const char *varName = "PATH";
	const char *varName = "MC_DETERMINISTIC";

	char *str_ptr = std::getenv(varName);

	if (str_ptr == NULL) {
		std::cout << varName << ": does not exist." << std::endl;
	} else {
		std::cout << varName << ": " << str_ptr << std::endl;
	}

	return 0;
}
