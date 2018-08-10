/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 22, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// #include <omp.h>

#include "mixt_Function.h"
#include "mixt_MixtComp.h"

// [[Rcpp::export]]
Rcpp::List mixtCompCluster(Rcpp::List dataList, Rcpp::List mcStrategy, int nbClass, double confidenceLevel) {
	mixt::SGraph algo;
	mixt::SGraph data;
	mixt::SGraph desc;
	mixt::SGraph param;

	std::cout << "MixtComp, learn, version: " << mixt::version << std::endl;
	std::cout << "Deterministic mode: " << mixt::deterministicMode() << std::endl;
//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;

	mixt::Timer totalTimer("Total Run");

	std::string warnLog; // string to log warnings

	Rcpp::List mcMixture; // lists to export results
	Rcpp::List mcVariable;

	if (handler.nbSample() < 1 || handler.nbVariable() < 1) {
		std::stringstream sstm;
		sstm << "No valid data provided. Please check the descriptor file." << std::endl;
		warnLog += sstm.str();
	}

	if (0 < warnLog.size()) {
		mcMixture["warnLog"] = warnLog;

		return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy, Rcpp::Named("mixture") = mcMixture, Rcpp::Named("variable") = mcVariable);
	}

	// Create the composer and read the data

	mixt::MixtureComposer composer(handler.nbSample(), nbClass, confidenceLevel, dummySGraph, dummySGraph, dummySGraph);

	mixt::Timer readTimer("Read Data");
	warnLog += manager.createMixtures(composer, nbClass);
	warnLog += composer.setDataParam<mixt::ParamSetterDummy, mixt::DataHandlerR>(paramSetter, handler, mixt::learning_);
	readTimer.top("data has been read");

	if (0 < warnLog.size()) {
		mcMixture["warnLog"] = warnLog;

		return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy, Rcpp::Named("mixture") = mcMixture, Rcpp::Named("variable") = mcVariable);
	}

	// Get the parameters

	dataExtractor.setNbMixture(handler.nbVariable()); // all data has been read, checked and transmitted to the mixtures
	paramExtractor.setNbMixture(handler.nbVariable());

	mixt::StrategyParam strategyParam;
	paramRToCpp(mcStrategy, strategyParam);

	// Run the SEM strategy

	mixt::SemStrategy semStrategy(&composer, strategyParam);
	mixt::Timer semStratTimer("SEM Strategy Run");
	warnLog += semStrategy.run();
	semStratTimer.top("SEM strategy run complete");

	if (0 < warnLog.size()) {
		std::cout << warnLog << std::endl;
		mcMixture["warnLog"] = warnLog;

		return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy, Rcpp::Named("mixture") = mcMixture, Rcpp::Named("variable") = mcVariable);
	}

	// Run the Gibbs strategy

	mixt::GibbsStrategy gibbsStrategy(&composer, strategyParam, 2);
	mixt::Timer gibbsStratTimer("Gibbs Strategy Run");
	warnLog += gibbsStrategy.run();
	gibbsStratTimer.top("Gibbs strategy run complete");

	if (0 < warnLog.size()) {
		mcMixture["warnLog"] = warnLog;

		return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy, Rcpp::Named("mixture") = mcMixture, Rcpp::Named("variable") = mcVariable);
	}

	// Run has been successful, export everything

	composer.writeParameters();

	composer.exportDataParam<mixt::DataExtractorR, mixt::ParamExtractorR>(dataExtractor, paramExtractor);

	mcMixture["nbCluster"] = nbClass; // export the composer results to R through modifications of mcResults
	mcMixture["nbFreeParameters"] = composer.nbFreeParameters();
	mixt::Real lnObsLik = composer.lnObservedLikelihood();
	mixt::Real lnCompLik = composer.lnCompletedLikelihood();
	mcMixture["lnObservedLikelihood"] = lnObsLik;
	mcMixture["lnCompletedLikelihood"] = lnCompLik;
	mcMixture["BIC"] = lnObsLik - 0.5 * composer.nbFreeParameters() * std::log(composer.nbInd());
	mcMixture["ICL"] = lnCompLik - 0.5 * composer.nbFreeParameters() * std::log(composer.nbInd());

	std::cout << "lnObservedLikelihood: " << lnObsLik << std::endl << std::endl;

	mcMixture["runTime"] = totalTimer.top("end of run");
	mcMixture["nbInd"] = composer.nbInd();
	mcMixture["mode"] = "learn";

	Rcpp::NumericMatrix idc;
	mixt::IDClass(composer, idc);
	mcMixture["IDClass"] = idc;

	Rcpp::NumericMatrix pGC;
	mixt::lnProbaGivenClass(composer, pGC);
	mcMixture["lnProbaGivenClass"] = pGC;

	Rcpp::NumericVector completedProbabilityLogBurnIn, completedProbabilityLogRun;
	mixt::completedProbaLog(composer, completedProbabilityLogBurnIn, completedProbabilityLogRun);
	mcMixture["completedProbabilityLogBurnIn"] = completedProbabilityLogBurnIn;
	mcMixture["completedProbabilityLogRun"] = completedProbabilityLogRun;

	Rcpp::NumericMatrix delta;
	mixt::matDelta(composer, delta);
	mcMixture["delta"] = delta;
	mcMixture["nbSample"] = handler.nbSample();

	Rcpp::List type = handler.rcppReturnType();
	Rcpp::List data = dataExtractor.rcppReturnVal();
	Rcpp::List param = paramExtractor.rcppReturnParam();

	mcVariable = Rcpp::List::create(Rcpp::Named("type") = type, Rcpp::Named("data") = data, Rcpp::Named("param") = param);

	std::cout << "End of run." << std::endl;

	return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy, Rcpp::Named("mixture") = mcMixture, Rcpp::Named("variable") = mcVariable);
}
