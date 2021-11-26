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
 *  Created on: Sept 4, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file Constants.cpp
 *  @brief In this file we define various constants
 **/

#include <Various/Constants.h>
#include <boost/math/constants/constants.hpp>

namespace mixt {

const std::string version = "4.0";

const char *deterministicEnvVar = "MC_DETERMINISTIC";

const bool degeneracyAuthorizedForNonBoundedLikelihood = true;

const std::string eol = "\n";

const int nbSamplingAttempts = 10000;

const Real epsilon = 1.e-10;
const std::string epsilonStr = "1.e-10";
const Real logEpsilon = std::log(epsilon);

const Real epsilonProba = std::numeric_limits<Real>::epsilon();

#if defined MINMODALITY
const int minModality = MINMODALITY;
#else
const int minModality = 1;
#endif

const int minIndex = 1;
const Real pi = boost::math::constants::pi<Real>();
const Real logsqrt2pi = std::log(boost::math::constants::root_two_pi<Real>());
const std::string progressFile = "progress";
const Real minInf = std::log(0.);

//const int sizeTupleBOS = 2;
//const int nbGibbsIniBOS = 100;
//const Real piInitBOS = 0.9;
//const Real piInitISR = 0.75;
//const Real piThreshold = 0.05;

const int nbGibbsIterRankMStep = 50;
const Index nbGibbsIniISR = 100;

const int nbSampleObserved = 10000;

const Index maxIterationOptim = 50;
const Real relTolOptim = 0.0001;

const std::string rankPosSep = ",";

const Index nCompletedInitTry = 1000;

// const Real poissonInitMinAlpha = 0.5;

} // namespace mixt
