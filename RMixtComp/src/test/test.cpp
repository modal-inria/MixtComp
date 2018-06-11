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

//	R.parseEvalQ(R"(setwd("/home/elvinz/Documents/MixtCompBugData/180606 - Fares"))"); // note the use of string literals to avoid cluttering the R command with escape characters
//	R.parseEvalQ(R"(setwd("/home/elvinz/Documents/MixtCompBugData/180607 - Iris"))");
	R.parseEvalQ(R"(setwd("/home/elvinz/Documents/MixtCompBugData/180606 - Fares"))");
	R.parseEvalQ(R"(source("model.R"))");

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
