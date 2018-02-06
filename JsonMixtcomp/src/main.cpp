/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/

#include <iostream>
#include "json.hpp"
#include "mixt_DataHandlerJson.h"
#include "mixt_DataExtractorJson.h"
#include "mixt_ParamExtractorJson.h"
#include "mixt_ParamSetterJson.h"
#include "mixt_Function_Json.h"
#include "../../MixtComp/src/mixt_MixtComp.h"
using namespace nlohmann;

std::string learn_mixtcomp(json argument_list,std::string json_file_output){

  std::string warnLog;
  mixt::Timer totalTimer("Total Run");

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
  #ifdef MC_VERBOSE
    handler.writeDataMap();
  #endif

  // create the data extractor
  mixt::DataExtractorJson dataExtractor;

  // create the parameters extractor
  mixt::ParamExtractorJson paramExtractor;

  // create the parameters setter and mixture manager
  mixt::ParamSetterDummy paramSetter;
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

           mixt::StrategyParam strategyParam;
           paramJsonToCpp(mcStrategy, strategyParam);

           // Run the SEM strategy

           mixt::SemStrategy semStrategy(&composer, strategyParam);
           mixt::Timer semStratTimer("SEM Strategy Run");
           warnLog += semStrategy.run();
           semStratTimer.top("SEM strategy run complete");

           // Run the Gibbs strategy

           mixt::GibbsStrategy gibbsStrategy(&composer, strategyParam, 2);
           mixt::Timer gibbsStratTimer("Gibbs Strategy Run");
           warnLog += gibbsStrategy.run();
           gibbsStratTimer.top("Gibbs strategy run complete");

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

             json pGC;
             mixt::lnProbaGivenClass(composer, pGC);
             mcMixture["lnProbaGivenClass"] = pGC;

             json delta;
             mixt::matDelta(composer, delta);
             mcMixture["delta"] =delta;

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

  return warnLog;
}

std::string predict_mixtcomp(json argument_list,std::string json_file_output){

  std::string warnLog;
  mixt::Timer totalTimer("Total Run");

  json resGetData_lm         = argument_list["resGetData_lm"];
  std::string pathParamList  = argument_list["pathParamList"];
  double confidenceLevel     = argument_list["confidenceLevel"];
  const json& mcStrategy     = argument_list["mcStrategy"];
  int nbClass                = argument_list["nbClass"];

  // read the parameters from mixtcomp
  std::ifstream infile_mc(pathParamList);
  json paramList;
  std::ifstream ifs_mc(pathParamList);
  ifs_mc >> paramList;

  // lists to export results
  json mcMixture;
  json mcVariable;

  // create the data handler
  mixt::DataHandlerJson handler(resGetData_lm);
  warnLog += handler.listData();
  handler.writeInfo();
  #ifdef MC_VERBOSE
    handler.writeDataMap();
  #endif

  // create the data extractor
  mixt::DataExtractorJson dataExtractor;

  // create the parameters setter

  mixt::ParamSetterJson paramSetter(paramList["variable"]["param"]);

  // create the parameters extractor
  mixt::ParamExtractorJson paramExtractor;

  // create the mixture manager
  mixt::MixtureManager<mixt::DataHandlerJson,
                       mixt::DataExtractorJson,
                       mixt::ParamSetterJson,
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

      warnLog += composer.setDataParam<mixt::ParamSetterJson,
                                       mixt::DataHandlerJson>(paramSetter,
                                                           handler,
                                                           mixt::prediction_);

      readTimer.top("data has been read");

      if (warnLog.size() == 0) { // all data has been read, checked and transmitted to the mixtures
        dataExtractor .setNbMixture(handler.nbVariable()); // all data has been read, checked and transmitted to the mixtures
        paramExtractor.setNbMixture(handler.nbVariable());

        mixt::StrategyParam strategyParam;
        paramJsonToCpp(mcStrategy,
                    strategyParam);

        mixt::GibbsStrategy strategy(&composer, strategyParam, 0);

        // Run the strategy

        mixt::Timer stratTimer("Gibbs Strategy Run");
        warnLog += strategy.run();
        stratTimer.top("Gibbs strategy run complete");

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
             mcMixture["mode"] = "predict";

             json idc;
             mixt::IDClass(composer, idc);
             mcMixture["IDClass"] = idc;

             json pGC;
             mixt::lnProbaGivenClass(composer, pGC);
             mcMixture["lnProbaGivenClass"] = pGC;

             json delta;
             mixt::matDelta(composer, delta);
             mcMixture["delta"] =delta;

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

  return warnLog;
}

int main(int argc, char* argv[]) {

  std::string warnLog;
  std::string json_file_input  = argv[1];
  std::string json_file_output = argv[2];
  std::ifstream infile(json_file_input);

  if(infile.good() == false){
    warnLog += "the file "+json_file_input+" does not exist";
  } else {
    std :: cout << "Reading the data: "+json_file_input << std::endl;
    json argument_list;
    std::ifstream ifs(json_file_input);
    ifs >> argument_list;
    std::string mode = argument_list["mode"];

//    Fork by_row = true or false

    if (!(argument_list.find("by_row") != argument_list.end())) {
         std::cout << "The 'by_row' boolean parameter can't be found in the 'argument_list' json list" << std::endl;
         return 0;
       }
      bool by_row            = argument_list["by_row"].get<bool>();
      if(by_row == true){
        if (argument_list.find("data") != argument_list.end()) {
          json data_input = argument_list["data"];
          for(int j = 0 ; j < data_input[0].size() ; j++){
            for(int i = 0 ; i < data_input.size() ; i++){
              argument_list["resGetData_lm"][j]["data"][i] = data_input[i][j].get<std::string>();
            }
          }
        } else {
          std::cout << "The 'by_row' parameter is equal to true but there is not 'data' element in the 'argument_list' json list" << std::endl;
          return 0;
        }
      }

    if(mode=="learn"){
      warnLog += learn_mixtcomp(argument_list,json_file_output);
    } else if(mode=="predict"){

      warnLog += predict_mixtcomp(argument_list,json_file_output);
    } else {
      warnLog += "the field [\"mode\"] contained in the json file "+ json_file_input +" must be either \"learn\" or \"predict\" but is: " + mode;
    }
  }

  if( warnLog == "" ){
       std::cout << "WarnLog empty, run successful" << std::endl;
       std::cout << "Results recorded in the file: "+json_file_output << std::endl;
     } else {
       std::cout << "WarnLog not empty :" << std::endl;
       std::cout << warnLog << std::endl;
     }
  return 0;
}
