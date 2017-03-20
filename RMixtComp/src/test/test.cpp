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
  RInside R(argc, argv);              // create an embedded R instance
  R.parseEvalQ("library(RMixtComp)");
  R.parseEvalQ("library(FunctionAnalysis)");
  R.parseEvalQ("setwd(\"/Users/kubicki/Documents/160519 - Vallourec/170213 - R launch and post\")");
  R.parseEvalQ("f07Learn(\"data/170222 - multivariate\"");
  exit(0);
}
