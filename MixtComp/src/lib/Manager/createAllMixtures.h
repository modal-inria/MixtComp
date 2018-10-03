/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 18, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef LIB_MANAGER_CREATEALLMIXTURES_H
#define LIB_MANAGER_CREATEALLMIXTURES_H

#include <Composer/mixt_MixtureComposer.h>
#include <Mixture/Simple/SimpleMixture.h>
#include <Mixture/Simple/Categorical/Categorical.h>
#include <Mixture/Simple/Gaussian/Gaussian.h>
#include <Mixture/Simple/NegativeBinomial/NegativeBinomial.h>
#include <Mixture/Simple/Poisson/Poisson.h>
#include <Mixture/Simple/Weibull/Weibull.h>
#include <Mixture/Functional/mixt_FunctionalMixture.h>
#include <Mixture/Functional/mixt_FunctionalSharedAlphaMixture.h>
#include <Mixture/Rank/mixt_RankMixture.h>

namespace mixt {

template<typename Graph>
std::string createAllMixtures(const Graph& algo, const Graph& desc, const Graph& data, const Graph& param, Graph& out, MixtureComposer& composer) {
	std::string warnLog;

	Index nClass = algo.template get_payload<Index>( { }, "nClass");
	Real confidenceLevel = algo.template get_payload<Real>( { }, "confidenceLevel");
	Index nInd = algo.template get_payload<Index>( { }, "nInd");

	std::list<std::string> varNames;
	desc.name_payload( { }, varNames);
	std::cout << "Variables: " << itString(varNames) << std::endl;

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

			//		if (idModel == "Rank") {
			//			RankMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>* p_bridge = new RankMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>(idName, nbCluster,
			//					dummyNObs, p_handler_, p_dataExtractor_, p_paramSetter_, p_paramExtractor_, confidenceLevel);
			//			return p_bridge;
			//		}

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

#endif /* MIXT_MIXTUREMANAGER_H */
