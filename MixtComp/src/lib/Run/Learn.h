/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_RUN_LEARN_H
#define LIB_RUN_LEARN_H

#include <Composer/mixt_MixtureComposer.h>
#include <Statistic/mixt_RNG.h>
#include <Strategy/mixt_GibbsStrategy.h>
#include <Strategy/mixt_SEMStrategy.h>
#include <Various/mixt_Timer.h>

namespace mixt {

/**
 * The learn algorithm is encapsulated in this function.
 */
template<typename Graph>
void learn(const Graph& algo, const Graph& data, const Graph& desc, Graph& out) {
	Graph param; // dummy Graph to be provided as unused argument in setDataParam in learning

	std::cout << "MixtComp, learn, version: " << version << std::endl;
	std::cout << "Deterministic mode: " << deterministicMode() << std::endl;
//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;

	Timer totalTimer("Total Run");

	std::string warnLog;// string to log warnings

	// Create the composer and read the data
	MixtureComposer composer(algo);

	Timer readTimer("Read Data");
	warnLog += createAllMixtures(algo, desc, data, param, out, composer);
	warnLog += composer.setDataParam(learning_, data, param);
	readTimer.top("data has been read");

	if (0 < warnLog.size()) {
		out.add_payload({}, "warnLog", warnLog);
		return;
	}

	// Run the SEM strategy

	SemStrategy<Graph> semStrategy(composer, algo);
	Timer semStratTimer("SEM Strategy Run");
	warnLog += semStrategy.run();
	semStratTimer.top("SEM strategy run complete");

	if (0 < warnLog.size()) {
		out.add_payload({}, "warnLog", warnLog);
		return;
	}

	// Run the Gibbs strategy

	GibbsStrategy<Graph> gibbsStrategy(composer, algo, 2);
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
