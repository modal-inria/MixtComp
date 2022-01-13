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
 *  Created on: August 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "../LinAlg/LinAlg.h" // for the definition of Real type

namespace mixt {

extern const std::string version; // version number, see http://semver.org/

extern const char *deterministicEnvVar; // name of the env variable used to flag deterministic run

extern const bool degeneracyAuthorizedForNonBoundedLikelihood; // are degeneracies authorized for non bounded likelihood ?

extern const std::string eol;

extern const int nbSamplingAttempts; // number of sampling attempts, when not enough individuals are sampled

extern const Real epsilon; // very small value of real to check for near zero values
extern const std::string epsilonStr; // previous value, in scientific notation
extern const Real logEpsilon; // log of very small value

extern const int minModality; // minimal modality for categorical models (for example, 0-based or 1-based numbering)
extern const int minIndex;
extern const Real pi; // pi
extern const Real logsqrt2pi; // constant used in pdf computation for normal law: log(sqrt(2pi))
extern const std::string progressFile; // progress file name
extern const Real minInf; // value of log(0)

//extern const int sizeTupleBOS; // default size of tuple for Gibbs sampling in BOS model
//extern const int nbGibbsIniBOS; // number of Gibbs sampling rounds in initialization to increase variability on z
//extern const Real piInitBOS; // value of pi used in Gibbs used in initialization
//extern const Real piInitISR; // value of pi used in ISR initialization
//extern const Real piThreshold; //threashold on pi to detect degeneracy

extern const int nbGibbsIterRankMStep; // number of iterations in the Gibbs used in the estimation of mu
extern const Index nbGibbsIniISR; // number of iterations in the initializing Gibbs

extern const int nbSampleObserved; // number of sample per class to estimate the observed probability, for example in Ordinal or Rank data

extern const Index maxIterationOptim; // maximum number of evaluation of cost function for optimization in Functional and in Weibull
extern const Real relTolOptim;

extern const std::string rankPosSep; // separator used in Rank positions

extern const Index nCompletedInitTry;

// extern const Real poissonInitMinAlpha; // minimal value that can be used for alpha estimation in a Poisson distribution

} // namespace mixt

#endif /* CONSTANTS_H_ */
