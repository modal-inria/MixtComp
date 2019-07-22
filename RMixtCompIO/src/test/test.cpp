/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

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
 *  Created on: Feb 25, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <RInside.h> // for the embedded R via RInside
#include <fstream>

int main(int argc, char *argv[]) {
	RInside R(argc, argv); // create an embedded R instance

//	R.parseEvalQ(R"(setwd("/media/sf_Documents/160803 - MixtComp/160803 - debug MixtComp/180129 - Crashs Aléatoires - Etienne"))"); // note the use of string literals to avoid cluttering the R command with escape characters
//	R.parseEvalQ(R"(source("run.R"))");

//	R.parseEvalQ("library(RMixtComp)");
//	R.parseEvalQ("testFunctionalSharedAlpha()");
//	R.parseEvalQ("testOrdinal()");
//	R.parseEvalQ("testRank()");
//	R.parseEvalQ("testCategorical()");
//	R.parseEvalQ("testGaussian()");
//	R.parseEvalQ("testPoisson()");
//	R.parseEvalQ("testWeibull()");

//	exit(0);
	return 0;
}
