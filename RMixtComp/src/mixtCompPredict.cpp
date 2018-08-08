/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// #include <omp.h>
#include "mixt_DataHandlerR.h"
#include "mixt_DataExtractorR.h"
#include "mixt_ParamExtractorR.h"
#include "mixt_ParamSetterR.h"
#include "mixt_Function.h"
#include "mixt_MixtComp.h"

// [[Rcpp::export]]
Rcpp::List mixtCompPredict(Rcpp::List dataList, Rcpp::List paramList, Rcpp::List mcStrategy, int nbClass, double confidenceLevel) {

	std::cout << "MixtComp, predict, version: " << mixt::version << std::endl;
	std::cout << "Deterministic mode: " << mixt::deterministicMode() << std::endl;
//	std::cout<< "Number of threads: " << omp_get_num_threads() << std::endl;

	mixt::Timer totalTimer("Total Run");

	std::string warnLog; // string to log warnings

	Rcpp::List mcMixture; // lists to export results
	Rcpp::List mcVariable;

	mixt::DataHandlerR handler(dataList); // create the data handler
	warnLog += handler.listData();
	handler.writeInfo();
	handler.writeDataMap();

	mixt::DataExtractorR dataExtractor; // create the data extractor

	mixt::ParamSetterR paramSetter(paramList); // create the parameters setter

	mixt::ParamExtractorR paramExtractor; // create the parameters extractor

	mixt::MixtureManager<mixt::DataHandlerR, // create the mixture manager
			mixt::DataExtractorR, mixt::ParamSetterR, mixt::ParamExtractorR> manager(&handler, &dataExtractor, &paramSetter, &paramExtractor, confidenceLevel, warnLog);

	// Basic checks

	if (confidenceLevel < 0. || 1. < confidenceLevel) {
		std::stringstream sstm;
		sstm << "ConfidenceLevel should be in the interval [0;1], but current value is: " << confidenceLevel << std::endl;
		warnLog += sstm.str();
	}

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

	mixt::MixtureComposer composer(handler.nbSample(), nbClass, confidenceLevel);

	mixt::Timer readTimer("Read Data");
	warnLog += manager.createMixtures(composer, nbClass);
	warnLog += composer.setDataParam<mixt::ParamSetterR, mixt::DataHandlerR>(paramSetter, handler, mixt::prediction_);
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

	// Run the Gibbs strategy

	mixt::GibbsStrategy strategy(&composer, strategyParam, 0);
	mixt::Timer stratTimer("Gibbs Strategy Run");
	warnLog += strategy.run();
	stratTimer.top("Gibbs strategy run complete");

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
	mcMixture["mode"] = "predict";

	Rcpp::NumericMatrix idc;
	mixt::IDClass(composer, idc);
	mcMixture["IDClass"] = idc;

	Rcpp::NumericMatrix pGC;
	mixt::lnProbaGivenClass(composer, pGC);
	mcMixture["lnProbaGivenClass"] = pGC;

	Rcpp::NumericMatrix delta;
	mixt::matDelta(composer, delta);
	mcMixture["delta"] = delta;
	mcMixture["runTime"] = totalTimer.top("end of run");
	mcMixture["nbSample"] = handler.nbSample();

	Rcpp::List type = handler.rcppReturnType();
	Rcpp::List data = dataExtractor.rcppReturnVal();
	Rcpp::List param = paramExtractor.rcppReturnParam();

	mcVariable = Rcpp::List::create(Rcpp::Named("type") = type, Rcpp::Named("data") = data, Rcpp::Named("param") = param);

	std::cout << "End of run." << std::endl;

	return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy, Rcpp::Named("mixture") = mcMixture, Rcpp::Named("variable") = mcVariable);
}
