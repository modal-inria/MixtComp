/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/variant.hpp>
#include <iostream>

#include "Learn.h"
#include <IO/Dummy.h>
#include <LinAlg/mixt_LinAlg.h>
#include <Manager/mixt_MixtureManager.h>
#include <Statistic/mixt_RNG.h>
#include <Various/mixt_Constants.h>
#include <Various/mixt_Timer.h>

namespace mixt {

/**
 * The learn algorithm is encapsulated in a function.
 */
SGraph learn(const SGraph& data, const SGraph& desc, const SGraph& algo) {
	SGraph res;
//
//	std::cout << "MixtComp, learn, version: " << version << std::endl;
//	std::cout << "Deterministic mode: " << deterministicMode() << std::endl;
//	//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;
//
//	Timer totalTimer("Total Run");
//
//	std::string warnLog; // string to log warnings
//
//	DataHandlerDummy handler; // TODO: remove those classes that are only here for the transition to SGraph
//	DataExtractorDummy dataextractor;
//	ParamSetterDummy paramsetter;
//	ParamExtractorDummy paramextractor;
//
//	Real nClass = boost::get<Real>(algo.get_payload("confidenceLevel"));
//
//	Real confidenceLevel = boost::get<Real>(algo.get_payload("confidenceLevel"));
//
//	MixtureManager<DataHandlerDummy, DataExtractorDummy, ParamSetterDummy, ParamExtractorDummy> manager(&handler, &dataextractor, &paramsetter, &paramextractor, confidenceLevel, warnLog);
//
//	// TODO: add the basic checks for confidence level existence and type, the nb of variables and observations
//
//	mixt::MixtureComposer composer(handler.nbSample(), nClass, confidenceLevel);
//
//	mixt::Timer readTimer("Read Data");
//	warnLog += manager.createMixtures(composer, nClass);
////	warnLog += composer.setDataParam<ParamSetterDummy, DataHandlerDummy>(paramsetter, handler, mixt::learning_);
//	readTimer.top("data has been read");
//
	return res;
}

}
