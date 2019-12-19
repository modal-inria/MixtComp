/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

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
 *  Created on: Aug 13, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iomanip>
#include <iostream>
#include <fstream>
#include "json.hpp"

#include <Run/Learn.h>
#include <Run/Predict.h>
#include <Various/Constants.h>
#include "JSONGraph.h"

using namespace mixt;

int main(int argc, char* argv[]) {
	try {
		std::cout << "JMixtComp" << std::endl;

		if (argc < 4) {
			std::cout
					<< "JMixtComp should be called with 4 parameters (paths to algo, data, model, resLearn) in learn mode and 5 parameters (paths to algo, data, model, resLearn, resPredict) in predict mode. It has been called with "
					<< argc - 1 << " parameters." << std::endl;
			return 0;
		}

		std::string warnLog;
		std::string algoFile = argv[1];
		std::string dataFile = argv[2];
		std::string descFile = argv[3];
		std::string resLearnFile = argv[4];

		std::ifstream algoStream(algoFile);
		std::ifstream dataStream(dataFile);
		std::ifstream descStream(descFile);

		if (algoStream.good() == false || dataStream.good() == false || descStream.good() == false) {
			std::cout << "File(s) not found: check that algo: " << algoFile << ", data: " << dataFile << ", and model: " << descFile << " paths are correct." << std::endl;
		} else {
			nlohmann::json algoJSON;
			algoStream >> algoJSON;
			JSONGraph algoG(algoJSON);

			nlohmann::json dataJSON;
			dataStream >> dataJSON;
			JSONGraph dataG(dataJSON);

			nlohmann::json descJSON;
			descStream >> descJSON;
			JSONGraph descG(descJSON);

			std::string mode = algoG.get_payload<std::string>( { }, "mode");

			JSONGraph resG;
			std::string resFile;

			if (mode == "learn") {
				resFile = resLearnFile;
				learn(algoG, dataG, descG, resG);
			} else if (mode == "predict") {
				if (argc != 6) {
					std::cout << "JMixtComp should be called with 5 parameters (paths to algo, data, model, resLearn, resPredict) in predict mode. It has been called with " << argc - 1 << " parameters."
							<< std::endl;
					return 0;
				}

				resFile = argv[5];

				std::ifstream resLearnStream(resLearnFile);

				if (resLearnStream.good() == false) {
					std::cout << "File not found: check that resLearn: " << resLearnFile << " path is correct." << std::endl;
				}else{
					nlohmann::json resLearnJSON;
					resLearnStream >> resLearnJSON;

					try {
						JSONGraph paramG(resLearnJSON["variable"]["param"]);
						predict(algoG, dataG, descG, paramG, resG);
					} catch (const std::string& s) {
						warnLog += s;
					}
				}


			} else {
				warnLog += "mode :" + mode + " not recognized. Please choose learn or predict." + eol;
			}

			if (warnLog.size() > 0) {
				resG.add_payload( { }, "warnLog", warnLog);
			}

			std::ofstream o(resFile);
			o << std::setw(4) << resG.getJ() << std::endl;
		}
	} catch (const std::string& s) {
		std::cout << s << std::endl;
	}

	return 0;
}
