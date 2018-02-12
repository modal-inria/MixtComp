/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Feb 25, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <RInside.h> // for the embedded R via RInside
#include <fstream>

int main(int argc, char *argv[]) {
	RInside R(argc, argv); // create an embedded R instance

//	R.parseEvalQ(R"(setwd("/media/sf_Documents/160803 - debug MixtComp/180130 - Fonctionnelles - Etienne"))"); // note the use of string literals to avoid cluttering the R command with escape characters
	R.parseEvalQ(R"(setwd("/media/sf_Documents/160803 - debug MixtComp/180208 - Quentin"))");
	R.parseEvalQ(R"(source("runB.R"))");

//	R.parseEvalQ("library(RMixtComp)");
//	R.parseEvalQ("testFunctional()");
//	R.parseEvalQ("testOrdinal()");
//	R.parseEvalQ("testRank()");
//	R.parseEvalQ("testCategorical()");
//	R.parseEvalQ("testGaussian()");
//	R.parseEvalQ("testPoisson()");
//	R.parseEvalQ("testWeibull()");

	exit(0);
}
