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
 *  Created on: June 10, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#include <Various/Constants.h>
#include <Various/Various.h>
#include <LinAlg/LinAlg.h>
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
