/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

//    R.parseEvalQ("testNewLearn()");
//    R.parseEvalQ("testNewLearn(regen = FALSE)");
//    R.parseEvalQ("testNewLearnPredict()");
    R.parseEvalQ("testGenDataLearn()");
//    R.parseEvalQ("testLearnPredict()");
//    R.parseEvalQ("testAERES()");

//    R.parseEvalQ("testRankWords()");

//    R.parseEvalQ("clustering()");
//    R.parseEvalQ("clustAuchan()");
//    R.parseEvalQ("linAlg()");
//    R.parseEvalQ("testRougeGorge()");

//    R.parseEvalQ("setwd(\"C:/Users/Vincent KUBICKI/Data/analysis/150330 - test donn�es prostate/02 - crash en pr�diction\")");
//    R.parseEvalQ("source(\"learn.R\")");
//    R.parseEvalQ("source(\"predict.R\")");

//    R.parseEvalQ("setwd(\"/Users/kubicki/Documents/analysis/151214 - données Etienne Gibbs trop lent\")");
//    R.parseEvalQ("source(\"test_mixtcomp.R\")");

//    R.parseEvalQ("setwd(\"/Users/kubicki/Documents/analysis/160226 - crash Christophe\")");
//    R.parseEvalQ("source(\"test_mixtcomp.R\")");

//    R.parseEvalQ("webDemoLearn(\"/home/elvinz/Data/workspace/mixtcomp.master/RMixtComp/dataServer/exp_68\")");
//    R.parseEvalQ("webDemoLearn(\"/home/elvinz/Data/analysis/150702 - crash paugam\")");

//    R.parseEvalQ("setwd(\"/Users/kubicki/Documents/analysis/151123 - MixtComp crash\")");
//    R.parseEvalQ("source(\"main.R\")");

//    R.parseEvalQ("setwd(\"/Users/kubicki/Documents/analysis/151126 - test webDemo\")");
//    R.parseEvalQ("webDemoLearn(\".\")");
//    R.parseEvalQ("webDemoPredict(\".\")");

  exit(0);
}
