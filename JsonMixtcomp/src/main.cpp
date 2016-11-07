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
#include "mixt_Function_Json.h"

#include "../../MixtComp/src/Various/mixt_Enum.h"
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
    mixt::Timer totalTimer("Total Run");

    double confidenceLevel = j["confidenceLevel"];
    const json& mcStrategy = j["mcStrategy"];
    int nbClass = j["nbClass"];

    // lists to export results
    json mcMixture;
    json mcVariable;

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

    if (warnLog.size() == 0) { // data is correct in descriptors, proceed with reading
        mixt::MixtureComposer composer(handler.nbSample(),
                                       nbClass,
                                       confidenceLevel);

        mixt::Timer readTimer("Read Data");
        warnLog += manager.createMixtures(composer,
                                          nbClass);
        std::cout<< mixt::learning_;

        warnLog += composer.setDataParam<mixt::ParamSetterDummy,
                                         mixt::DataHandlerJson>(paramSetter,
                                                             handler,
                                                             mixt::learning_);
        readTimer.top("data has been read");


        if (warnLog.size() == 0) { // all data has been read, checked and transmitted to the mixtures
             dataExtractor .setNbMixture(handler.nbVariable());
             paramExtractor.setNbMixture(handler.nbVariable());

             mixt::StrategyParam param;
             mixt::paramJsonToCpp(mcStrategy,
                         param);
             // create the appropriate strategy and transmit the parameters
             mixt::SemStrategy strategy(&composer,
                                        param); // number of iterations for Gibbs sampler

             // run the strategy
             mixt::Timer stratTimer("Strategy Run");
             warnLog += strategy.run();
             stratTimer.top("strategy run complete");
             if (warnLog.size() == 0) { // all data has been read, checked and transmitted to the mixtures
       #ifdef MC_VERBOSE
               composer.writeParameters();
       #endif
               composer.exportDataParam<mixt::DataExtractorJson,
                                        mixt::ParamExtractorJson>(dataExtractor,
                                                               paramExtractor);

               // export the composer results to R through modifications of mcResults
               mcMixture["nbCluster"] = nbClass;
               mcMixture["nbFreeParameters"] = composer.nbFreeParameters();
               Real lnObsLik = composer.lnObservedLikelihood();
               Real lnCompLik = composer.lnCompletedLikelihood();
               mcMixture["lnObservedLikelihood"] = lnObsLik;
               mcMixture["lnCompletedLikelihood"] = lnCompLik;
               mcMixture["BIC"] = lnObsLik  - 0.5 * composer.nbFreeParameters() * std::log(composer.nbInd());
               mcMixture["ICL"] = lnCompLik - 0.5 * composer.nbFreeParameters() * std::log(composer.nbInd());

               mcMixture["runTime"] = totalTimer.top("end of run");
               mcMixture["nbInd"] = composer.nbInd();
               mcMixture["mode"] = "learn";

               json idc;
               mixt::IDClass(composer, idc);
               mcMixture["IDClass"] = idc;
             }
        }
    }
    mcMixture["warnLog"] = warnLog;
    #ifdef MC_VERBOSE
      if (warnLog.size() != 0) {
        std::cout << "!!! warnLog not empty !!!" << std::endl;
        std::cout << warnLog << std::endl;
      }
    #endif

    mcMixture["runTime"] = totalTimer.top("end of run");
    mcMixture["nbSample"] = handler.nbSample();


    json type = handler.jsonReturnType();
    json data = dataExtractor.jsonReturnVal();
    json param = paramExtractor.jsonReturnParam();

    mcVariable["type"]  = type;
    mcVariable["data"]  = data;
    mcVariable["param"] = param;


    json res;
    res["strategy"] = mcStrategy ;
    res["mixture"] = mcMixture;
    res["variable"] = mcVariable;

    std::string output_str = res.dump();
    std::ofstream out("/home/etienne/cylande/retfor/livrable_2/data/working_data/test_json_output.json");
    out << output_str;
    out.close();

    return 0;
}

