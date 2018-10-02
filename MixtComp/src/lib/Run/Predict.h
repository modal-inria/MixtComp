/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 10, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_RUN_PREDICT_H
#define LIB_RUN_PREDICT_H

#include <LinAlg/mixt_LinAlg.h>
#include <Manager/createAllMixtures.h>
#include <Statistic/mixt_RNG.h>
#include <Strategy/mixt_GibbsStrategy.h>
#include <Various/mixt_Constants.h>
#include <Various/mixt_Timer.h>

namespace mixt {

/**
 * The learn algorithm is encapsulated in this function.
 */
template<typename Graph>
void predict(const Graph& algo, const Graph& data, const Graph& desc, const Graph& param, Graph& out) {
	std::cout << "MixtComp, predict, version: " << version << std::endl;
	std::cout << "Deterministic mode: " << deterministicMode() << std::endl;
//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;

	Timer totalTimer("Total Run");

	std::string warnLog; // string to log warnings

	// Create the composer and read the data

	MixtureComposer composer(algo);

	Timer readTimer("Read Data");
	warnLog += createAllMixtures(algo, desc, data, param, out, composer);
	warnLog += composer.setDataParam(prediction_, data, param);
	readTimer.top("data has been read");

	if (0 < warnLog.size()) {
		out.add_payload({}, "warnLog", warnLog);
		return;
	}

	// Run the Gibbs strategy

	GibbsStrategy<Graph> gibbsStrategy(composer, algo, 0);
	Timer gibbsStratTimer("Gibbs Strategy Run");
	warnLog += gibbsStrategy.run();
	gibbsStratTimer.top("Gibbs strategy run complete");

	if (0 < warnLog.size()) {
		out.add_payload({}, "warnLog", warnLog);
		return;
	}

	// Run has been successful, export everything

	composer.writeParameters();

	Real runTime = totalTimer.top("end of run");
	std::string mode = "learn";

	composer.exportMixture(runTime, out);
	composer.exportDataParam(out);
}

}

#endif
