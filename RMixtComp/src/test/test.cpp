/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Feb 25, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <RInside.h>                    // for the embedded R via RInside
#include <fstream>

int main(int argc, char *argv[]) {
	std::cout << "toto" << std::endl;
  RInside R(argc, argv);              // create an embedded R instance
//  R.parseEvalQ("library(RMixtComp)");
  R.parseEvalQ("setwd(\"/home/elvinz/Documents/170705 - test\")");
//  R.parseEvalQ("gdfgdgdfdf");
  R.parseEvalQ("source(\"test.R\")");
  exit(0);
}
