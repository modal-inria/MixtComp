/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: Sep 24, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// #include <omp.h>
#include <Rcpp.h>

#include <Run/Learn.h>
#include <Run/Predict.h>
#include <MixtComp.h>

#include "RGraph.h"

using namespace mixt;

// [[Rcpp::export]]
Rcpp::List rmc(Rcpp::List algoR, Rcpp::List dataR, Rcpp::List descR, Rcpp::List resLearnR) {
	RGraph resRG;
  std::string warnLog;
  
	try {
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
	} catch (const std::string& s) {
	  warnLog += s;
	}
	
	if (warnLog.size() > 0) {
		resRG.add_payload( { }, "warnLog", warnLog);
	}

	return resRG.getL();
}
