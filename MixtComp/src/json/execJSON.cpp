/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 13, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include <fstream>
#include "json.hpp"

#include "JSONToSGraph.h"
#include "SGraphToJSON.h"
#include <Run/Learn.h>
#include <Various/mixt_Constants.h>

using namespace mixt;

int main(int argc, char* argv[]) {
	std::cout << "JMixtComp, learn" << std::endl;

	std::string warnLog;
	std::string algoFile = argv[1];
	std::string dataFile = argv[2];
	std::string descFile = argv[3];
	std::string outFile; // position 4 or 5 depends wether learn or predict mode

	std::ifstream algoStream(algoFile);
	std::ifstream dataStream(dataFile);
	std::ifstream descStream(descFile);

	if (algoStream.good() == false || dataStream.good() == false || descStream || false) {
		warnLog += "Check that algo: " + algoFile + ", data: " + dataFile + ", and desc: " + descFile + " paths are correct" + eol;
	} else {
		nlohmann::json algoJSON;
		algoStream >> algoJSON;
		SGraph algoG = JSONToSGraph(algoJSON);

		nlohmann::json dataJSON;
		dataStream >> dataJSON;
		SGraph dataG = JSONToSGraph(dataJSON);

		nlohmann::json descJSON;
		descStream >> descJSON;
		SGraph descG = JSONToSGraph(descJSON);

		std::string mode = algoJSON["mode"].get<std::string>();

		SGraph resG;

		if (mode == "learn") {
			outFile = argv[4];
			resG = learn(algoG, dataG, descG);
		} else if (mode == "predict") {

		} else {
			warnLog += "mode :" + mode + " not recognized" + eol;
		}

		if (warnLog.size() > 0) {
			resG.add_payload("warnLog", warnLog);
		}

		nlohmann::json resJ = SGraphToJSON(resG);
		std::ofstream o(outFile);
		o << std::setw(4) << resJ << std::endl;
	}

	return 0;
}
