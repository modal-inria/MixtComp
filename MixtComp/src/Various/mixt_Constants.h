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
 *  Created on: August 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_CONSTANTS_H_
#define MIXT_CONSTANTS_H_

#include "../LinAlg/mixt_LinAlg.h" // for the definition of Real type

namespace mixt
{

extern const Real minStat; // threshold to have equiprobabilities in categorical sampler
extern const Real probaNonMisClass; // minimal in-sample probability to avoid rejection
extern const int moduloMisClass; // modulo used to trigger misclassification step
extern const int minIndPerClass; // minimal number of individual in a class to accept a z sampling
extern const int nbSamplingAttempts; // number of sampling attempts, when a not enough individuals are sampled
extern const Real epsilon; // very small value of real to check for near zero values
extern const int minModality; // minimal modality for categorical models
extern const Real pi; // pi
extern const Real l2pi; // constant used in pdf computation for normal law
extern const int minNbPresentValues; // minimum number of present values
extern const int nbClusterMax; // maximum number of clusters that can be asked
extern const std::string progressFile; // progress file name
extern const Real minInf; // value of log(0)
extern const int sizeTupleBOS; // default size of tuple for Gibbs sampling in BOS model
extern const int nbSampleBOS; // number of sample per class to estimate the observed probability
extern const int nbGibbsIniBOS; // number of Gibbs sampling rounds in initialization to increase variability on z

} // namespace mixt

#endif /* MIXT_CONSTANTS_H_ */
