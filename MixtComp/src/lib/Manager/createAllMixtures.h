/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: March 18, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef LIB_MANAGER_CREATEALLMIXTURES_H
#define LIB_MANAGER_CREATEALLMIXTURES_H

#include <Composer/MixtureComposer.h>
#include <Mixture/Simple/SimpleMixture.h>
#include <Mixture/Simple/Categorical/Categorical.h>
#include <Mixture/Simple/Gaussian/Gaussian.h>
#include <Mixture/Simple/NegativeBinomial/NegativeBinomial.h>
#include <Mixture/Simple/Poisson/Poisson.h>
#include <Mixture/Simple/Weibull/Weibull.h>
#include <Mixture/Functional/FunctionalMixture.h>
#include <Mixture/Functional/FunctionalSharedAlphaMixture.h>
#include <Mixture/Rank/RankMixture.h>

namespace mixt {

template<typename Graph>
std::string createAllMixtures(const Graph& algo, const Graph& desc, const Graph& data, const Graph& param, Graph& out, MixtureComposer& composer) {
	std::string warnLog;

	Index nClass = algo.template get_payload<Index>( { }, "nClass");
	Real confidenceLevel = algo.template get_payload<Real>( { }, "confidenceLevel");
	Index nInd = algo.template get_payload<Index>( { }, "nInd");

	std::list<std::string> varNames;
	desc.name_payload( { }, varNames);
#ifdef MC_VERBOSE
	std::cout << "Variables: " << itString(varNames) << std::endl;
#endif

	for (std::list<std::string>::const_iterator it = varNames.begin(), itEnd = varNames.end(); it != itEnd; ++it) {
		std::string idName = *it;
		std::string idModel = desc.template get_payload<std::string>( { *it }, "type");
		std::string paramStr = desc.template get_payload<std::string>( { *it }, "paramStr");

		if (idModel != "LatentClass") { // LatentClass type is managed directly in the composer
			IMixture* p_mixture = NULL;

			if (idModel == "Multinomial") {
				p_mixture = new SimpleMixture<Graph, Categorical>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}
			else if (idModel == "Gaussian") {
				p_mixture = new SimpleMixture<Graph, Gaussian>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			else if (idModel == "Poisson") {
				p_mixture = new SimpleMixture<Graph, Poisson>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			else if (idModel == "Weibull") {
				p_mixture = new SimpleMixture<Graph, Weibull>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			else if (idModel == "NegativeBinomial") {
				p_mixture = new SimpleMixture<Graph, NegativeBinomial>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			if (idModel == "Func_CS") {
				p_mixture = new FunctionalMixture<Graph>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			if (idModel == "Func_SharedAlpha_CS") {
				p_mixture = new FunctionalSharedAlphaMixture<Graph>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			if (idModel == "Rank_ISR") {
				p_mixture = new RankMixture<Graph>(data, param, out, idName, nClass, nInd, confidenceLevel, paramStr);
			}

			if (p_mixture) {
				composer.registerMixture(p_mixture);
			} else {
				std::stringstream sstm;
				sstm << "The model " << idModel << " has been selected to describe the variable " << idName << " but it is not implemented yet. Please choose an available model for this variable."
						<< std::endl;
				warnLog += sstm.str();
			}
		}
	}

	return warnLog;
}

} // namespace mixt

#endif /* CREATEALLMIXTURES_H */
