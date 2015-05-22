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

const Real minStat = 1.e-5; // threshold to have equiprobabilities in categorical sampler
const Real probaNonMisClass = 0.95; // minimal in-sample probability to avoid rejection
const int moduloMisClass = 10; // modulo used to trigger misclassification step
const int minIndPerClass = 3; // minimal number of individual in a class to accept a z sampling
const int nbSamplingAttempts = 20; // number of sampling attempts, when not enough individuals are sampled
const Real epsilon = 1.e-8; // very small value of real to check for near zero values
const int minModality = 1; // minimal modality for categorical models
const Real pi = 3.141592653589793; // pi
const Real l2pi = 0.9189385332046727417; // constant used in pdf computation for normal law, 1/2 * log(2 * pi)
const int minNbPresentValues = 3; // minimum number of present values for a given variable below which data is considered uninformative
const int nbClusterMax = 30; // maximum number of clusters that can be asked
const Real minInf = std::log(0.); // value of log(0)
const int sizeTupleConst = 2;

} // namespace mixt
