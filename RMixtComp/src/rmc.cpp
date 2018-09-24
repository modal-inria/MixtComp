/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Sep 24, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// #include <omp.h>
#include <Rcpp.h>

#include <Run/Learn.h>
#include <Run/Predict.h>
#include <mixt_MixtComp.h>

#include "RGraph.h"

using namespace mixt;

Rcpp::List rmc(Rcpp::List algoR, Rcpp::List dataR, Rcpp::List descR, Rcpp::List resLearnR) {
	RGraph resRG;

	try {
		std::cout << "JMixtComp" << std::endl;

		std::string warnLog;

		RGraph algoRG(algoR);
		RGraph dataRG(dataR);
		RGraph descRG(descR);

		std::string mode = algoRG.get_payload<std::string>( { }, "mode");

		if (mode == "learn") {
			learn(algoRG, dataRG, descRG, resRG);
		} else if (mode == "predict") {
			RGraph resLearnRG(resLearnR);

			try {
				RGraph paramRG;
				resLearnRG.getSubGraph( { "variable", "param" }, paramRG);
				predict(algoRG, dataRG, descRG, paramRG, resRG);
			} catch (const std::string& s) {
				warnLog += s;
			}
		} else {
			warnLog += "mode :" + mode + " not recognized. Please choose learn or predict." + eol;
		}

		if (warnLog.size() > 0) {
			resRG.add_payload( { }, "warnLog", warnLog);
		}
	} catch (const std::string& s) {
		std::cout << s << std::endl;
	}

	return resRG.getL();
}

// [[Rcpp::export]]
Rcpp::List rmcLearn(Rcpp::List algoR, Rcpp::List dataR, Rcpp::List descR) {
	Rcpp::List resLearn;

	return rmc(algoR, dataR, descR, resLearn);
}

// [[Rcpp::export]]
Rcpp::List rmcPredict(Rcpp::List algoR, Rcpp::List dataR, Rcpp::List descR, Rcpp::List resLearn) {
	return rmc(algoR, dataR, descR, resLearn);
}
