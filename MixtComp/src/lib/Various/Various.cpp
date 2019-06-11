/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 10, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#include <Various/Constants.h>
#include <Various/Various.h>
#include "../LinAlg/LinAlg.h"
#include <iostream>
#include <fstream>

namespace mixt {

void writeProgress(int group, int groupMax, int iteration, int iterationMax) {
	Real groupSize = 1. / (Real) (groupMax + 1); // group size in the progress bar
	Real startPoint = (Real) group / (Real) (groupMax + 1);
	Real inGroupPoint = (Real) iteration / (Real) (iterationMax + 1);
	Real globalPoint = startPoint + inGroupPoint * groupSize;

	std::ofstream myfile;
	if (const char* env_p = std::getenv("WORKER_PROGRESS_FILE")) { // has the WORKER_PROGRESS_FILE variable been defined ?
		myfile.open(env_p, std::ios::out | std::ios::binary);
	} else {
		myfile.open(progressFile.c_str(), std::ios::out | std::ios::binary);
	}
	myfile << globalPoint;
	myfile.close();
}

} // namespace mixt
