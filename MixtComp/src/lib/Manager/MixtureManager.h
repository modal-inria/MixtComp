/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 18, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_MIXTUREMANAGER_H
#define MIXT_MIXTUREMANAGER_H

#include "Composer/mixt_MixtureComposer.h"
#include "Mixture/Simple/SimpleMixture.h"
#include "Mixture/Simple/Categorical/Categorical.h"
#include "Mixture/Simple/Gaussian/Gaussian.h"
#include "Mixture/Simple/NegativeBinomial/NegativeBinomial.h"
#include "Mixture/Simple/Poisson/Poisson.h"
#include "Mixture/Simple/Weibull/Weibull.h"
#include "Mixture/Functional/mixt_FunctionalMixture.h"
#include "Mixture/Functional/mixt_FunctionalSharedAlphaMixture.h"
#include "Mixture/Rank/mixt_RankMixture.h"

namespace mixt {

std::string createAllMixtures(const SGraph& algo, const SGraph& desc, MixtureComposer& composer);

/** create a mixture and initialize it*
 *  @param idModel id of the model
 *  @param idName name of the model
 *  @param nbCluster number of cluster of the model
 **/
IMixture* createIndividualMixture(std::string const& idModel, std::string const& idName, Index nbCluster, Index nObs, Real confidenceLevel, const std::string& paramStr);

} // namespace mixt

#endif /* MIXT_MIXTUREMANAGER_H */
