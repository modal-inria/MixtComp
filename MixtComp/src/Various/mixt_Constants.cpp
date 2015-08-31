/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: Sept 4, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_Constant.cpp
 *  @brief In this file we define various constants
 **/

#include "mixt_Constants.h"

namespace mixt
{
const int maxWeakDegeneracyTrial = 100;
const int nbSamplingAttempts = 10000;

const Real epsilon = 1.e-8;
const Real logEpsilon = std::log(epsilon);

const int minModality = 1;
const int minIndPerClass = 1;
const Real pi = 3.141592653589793;
const Real l2pi = 0.9189385332046727417;
const int nbClusterMax = 30;
const std::string progressFile = "progress";
const Real minInf = std::log(0.);

const int sizeTupleBOS = 2;
const int nbSampleBOS = 200000;
const int nbGibbsIniBOS = 1000;
const Real piInitBOS = 0.99;
const Real piThreshold = 0.05;

const int nbGibbsIterRank = 50;
} // namespace mixt
