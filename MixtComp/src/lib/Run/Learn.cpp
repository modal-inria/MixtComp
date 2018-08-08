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

	std::cout << "MixtComp, learn, version: " << version << std::endl;
	std::cout << "Deterministic mode: " << deterministicMode() << std::endl;
	//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;

	Timer totalTimer("Total Run");

	std::string warnLog; // string to log warnings

	DataHandlerDummy datahandler;
	DataExtractorDummy dataextractor;
	ParamSetterDummy paramsetter;
	ParamExtractorDummy paramextractor;

	Real confidenceLevel = boost::get<Real>(
			algo.get_payload("confidenceLevel"));

	MixtureManager<DataHandlerDummy, // create the mixture manager
			DataExtractorDummy, ParamSetterDummy, ParamExtractorDummy> manager(
			&datahandler, &dataextractor, &paramsetter, &paramextractor,
			confidenceLevel, warnLog);

	return res;
}

}
