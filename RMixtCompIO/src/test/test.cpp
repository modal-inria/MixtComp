/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

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
