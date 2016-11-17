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

#include "../../MixtComp/src/mixt_MixtComp.h"

using namespace nlohmann;

int main(int argc, char* argv[]) {

    mixt::Timer totalTimer("Total Run");

    std::string warnLog;
    std::string json_file_input  = "./data/utest/arg_list_utest.json";
    std::string json_file_output = "./data/utest/output_utest.json";

    /*############################# lecture du fichier json contenant les arguments via rapidjson ################################*/

    json argument_list;
    std::ifstream ifs(json_file_input);
    ifs >> argument_list;

    json resGetData_lm     = argument_list["resGetData_lm"];
    double confidenceLevel = argument_list["confidenceLevel"];
    const json& mcStrategy = argument_list["mcStrategy"];
    int nbClass            = argument_list["nbClass"];

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
        warnLog += manager.createMixtures(composer,nbClass);


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

               // export the composer results through modifications of mcResults
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

    mcMixture["runTime"]  = totalTimer.top("end of run");
    mcMixture["nbSample"] = handler.nbSample();

    json type  = handler       .jsonReturnType();
    json data  = dataExtractor .jsonReturnVal();
    json param = paramExtractor.jsonReturnParam();

    mcVariable["type"]  = type  ;
    mcVariable["data"]  = data  ;
    mcVariable["param"] = param ;

    json res_list;
    res_list["strategy"] = mcStrategy ;
    res_list["mixture"]  = mcMixture  ;
    res_list["variable"] = mcVariable ;

    std::string output_str = res_list.dump();
    std::ofstream out(json_file_output);
    out << output_str;
    out.close();

    return 0;
}

