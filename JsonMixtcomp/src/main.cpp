/*
 * main.cpp
 *
 *  Created on: 21 oct. 2016
 *      Author: etienne
 */

#include <iostream>
#include "json.hpp"
#include "mixt_DataHandlerJson.h"
#include "mixt_DataExtractorJson.h"
#include "mixt_ParamExtractorJson.h"
#include "../../MixtComp/src/mixt_MixtComp.h"
#include "../../MixtComp/src/IO/Dummy.h"


using namespace nlohmann;

int main() {
    std::string warnLog;

    /*############################# lecture du fichier de données via rapidjson ################################*/

    json j;
    std::ifstream ifs("/home/etienne/cylande/retfor/livrable_2/data/working_data/test_json.json");
    ifs >> j;

    std::cout << "########  description du contenu du document d " << std::endl;
    std::cout << " " << std::endl<< std::endl;

    // description du contenu de j

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
         std::cout << it.key() << " : " << it.value()  << "\n";
    }

    std::cout << " " << std::endl<< std::endl;
    std::cout << "########  description du contenu du sous tableau resGetData_lm " << std::endl << std::endl;

    // description du contenu du sous tableau resGetData_lm

    json resGetData_lm = j["resGetData_lm"];
    // iterate the array
    for (json::iterator it = resGetData_lm.begin(); it != resGetData_lm.end(); ++it) {
      std::cout << *it << '\n';
    }

    /*#################################################################################################*/

    // create the data handler
    mixt::DataHandlerJson handler(resGetData_lm);
    warnLog += handler.listData();
    handler.writeInfo();
    handler.writeDataMap();

    // create the data extractor
    mixt::DataExtractorJson dataExtractor;

    // create the parameters setter
    mixt::ParamSetterDummy paramSetter;

    // create the parameters extractor
    mixt::ParamExtractorJson paramExtractor;

    double confidenceLevel = j["confidenceLevel"];

    // create the mixture manager
    mixt::MixtureManager<mixt::DataHandlerJson,
                         mixt::DataExtractorJson,
                         mixt::ParamSetterDummy,
                         mixt::ParamExtractorJson> manager(&handler,
                                                        &dataExtractor,
                                                        &paramSetter,
                                                        &paramExtractor,
                                                        confidenceLevel,
                                                        warnLog);

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


    if (warnLog.size() == 0) { // all data has been read, checked and transmitted to the mixtures
         dataExtractor .setNbMixture(handler.nbVariable());
         paramExtractor.setNbMixture(handler.nbVariable());

//         mixt::StrategyParam param;
//         paramRToCpp(mcStrategy,
//                     param);
    }

    return 0;
}

