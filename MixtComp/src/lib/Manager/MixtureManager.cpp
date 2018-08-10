/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 10, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#include "MixtureManager.h"

namespace mixt {

std::string createAllMixtures(const SGraph& algo, const SGraph& desc, MixtureComposer& composer) {
	std::string warnLog;

	Index nClass = algo.get_payload<Index>("nClass");
	Real confidenceLevel = algo.get_payload<Real>("confidenceLevel");
	Index nObs = algo.get_payload<Index>("confidenceLevel");

	const std::map<std::string, SGraph>& allVar = desc.get_children();

	for (std::map<std::string, SGraph>::const_iterator it = allVar.begin(), itEnd = allVar.end(); it != itEnd; ++it) {
		std::string idName = it->first;
		std::string idModel = it->second.get_payload<std::string>("type");
		std::string paramStr = it->second.get_payload<std::string>("paramStr");

		if (idModel != "LatentClass") { // LatentClass type is managed directly in the composer
			IMixture* p_mixture = createIndividualMixture(idModel, idName, nClass, nObs, confidenceLevel, paramStr);
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

IMixture* createIndividualMixture(std::string const& idModel, std::string const& idName, Index nbCluster, Index nObs, Real confidenceLevel, const std::string& paramStr) {
	if (idModel == "Categorical") {
		SimpleMixture<Categorical>* p_bridge = new SimpleMixture<Categorical>(idName, nbCluster, nObs, confidenceLevel, paramStr);
		return p_bridge;
	}

	if (idModel == "Gaussian") {
		SimpleMixture<Gaussian>* p_bridge = new SimpleMixture<Gaussian>(idName, nbCluster, nObs, confidenceLevel, paramStr);
		return p_bridge;
	}

	if (idModel == "Poisson") {
		SimpleMixture<Poisson>* p_bridge = new SimpleMixture<Poisson>(idName, nbCluster, nObs, confidenceLevel, paramStr);
		return p_bridge;
	}

	if (idModel == "Weibull") {
		SimpleMixture<Weibull>* p_bridge = new SimpleMixture<Weibull>(idName, nbCluster, nObs, confidenceLevel, paramStr);
		return p_bridge;
	}

	if (idModel == "NegativeBinomial") {
		SimpleMixture<NegativeBinomial>* p_bridge = new SimpleMixture<NegativeBinomial>(idName, nbCluster, nObs, confidenceLevel, paramStr);
		return p_bridge;
	}

//		if (idModel == "Functional") {
//			FunctionalMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>* p_bridge = new FunctionalMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>(idName,
//					nbCluster, dummyNObs, p_handler_, p_dataExtractor_, p_paramSetter_, p_paramExtractor_, confidenceLevel);
//			return p_bridge;
//		}

//		if (idModel == "FunctionalSharedAlpha") {
//			FunctionalSharedAlphaMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>* p_bridge = new FunctionalSharedAlphaMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>(
//					idName, nbCluster, dummyNObs, p_handler_, p_dataExtractor_, p_paramSetter_, p_paramExtractor_, confidenceLevel);
//			return p_bridge;
//		}

//		if (idModel == "Ordinal") {
//			Ordinal<DataHandler, DataExtractor, ParamSetter, ParamExtractor>* p_bridge =
//					new Ordinal<DataHandler, DataExtractor, ParamSetter,
//							ParamExtractor>(indexMixture, idName, nbCluster,
//							p_handler_, p_dataExtractor_, p_paramSetter_,
//							p_paramExtractor_, confidenceLevel);
//			return p_bridge;
//		}

//		if (idModel == "Rank") {
//			RankMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>* p_bridge = new RankMixture<DataHandler, DataExtractor, ParamSetter, ParamExtractor>(idName, nbCluster,
//					dummyNObs, p_handler_, p_dataExtractor_, p_paramSetter_, p_paramExtractor_, confidenceLevel);
//			return p_bridge;
//		}

	return 0;
}

} // namespace mixt
