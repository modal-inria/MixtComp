/*
 * Utest_learn.h
 *
 *  Created on: 9 nov. 2016
 *      Author: etienne
 */

#include "Utest.h"
#include <iostream>
#include "json.hpp"
#include "../src/mixt_DataHandlerJson.h"
#include "../src/mixt_DataExtractorJson.h"
#include "../src/mixt_ParamExtractorJson.h"
#include "../src/mixt_Function_Json.h"
#include "../../MixtComp/src/mixt_MixtComp.h"

using namespace nlohmann;

TEST(DataHandlerJson, listData_getData)
{

  // Variable declaration
  std::string warnLog;
  std::string json_file_input  = "./data/utest/arg_list_utest.json";
  std::string json_file_output = "./data/utest/output_utest.json";

  // Json data reading
  json argument_list;
  std::ifstream ifs(json_file_input);
  ifs >> argument_list;
  json resGetData_lm     = argument_list["resGetData_lm"];
  double confidenceLevel = argument_list["confidenceLevel"];
  const json& mcStrategy = argument_list["mcStrategy"];
  int nbClass            = argument_list["nbClass"];

  // TEST : warnLog from listData
  {
    json resGetData_lm_test = resGetData_lm;
    resGetData_lm_test[1]["id"] = "categorical1";
    mixt::DataHandlerJson handler(resGetData_lm_test) ;
    warnLog += handler.listData();
    ASSERT_EQ(warnLog, std::string("Several variables bear the same name: categorical1, while only a variable per name is allowed.\n"));

  }
  {
    json resGetData_lm_test = resGetData_lm;
    warnLog = "";
    resGetData_lm_test[1]["data"] = json();
    mixt::DataHandlerJson handler(resGetData_lm_test) ;
    warnLog += handler.listData();
    ASSERT_EQ(warnLog, std::string("Variable: categorical2 has 0 samples.Variable: categorical2 has 0 individuals, while the previous variable had 10 individuals. All variables must have the same number of individuals.\n"));
  }

  // TEST : contents of the HANDLER member variables
  warnLog = "";
  mixt::DataHandlerJson handler(resGetData_lm) ;
  warnLog += handler.listData();

  ASSERT_EQ(warnLog, "");
  ASSERT_EQ(handler.nbSample(), 10);
  ASSERT_EQ(handler.nbVariable(), 13);

  Vector<std::string> dataStr;
  Index nbInd;
  std::string paramStr;

  handler.getData("categorical1",dataStr, nbInd, paramStr);
  std::vector<std::string> dataStrVec(dataStr.data(), dataStr.data() + dataStr.size());
  ASSERT_EQ(dataStrVec,std::vector<std::string> ({ "5", "6", "8", "4", "?", "9", "2", "7", "3", "10" }));

  handler.getData("poisson2",dataStr, nbInd, paramStr);
  dataStrVec = std::vector<std::string> (dataStr.data(), dataStr.data() + dataStr.size());
  ASSERT_EQ(dataStrVec,std::vector<std::string> ( { "\"0\"", "\"0\"", "\"0\"", "\"0\"", "\"?\"", "\"?\"", "\"0\"", "\"0\"", "\"0\"", "\"0\"" }));

  handler.getData("ordinal3",dataStr, nbInd, paramStr);
  dataStrVec = std::vector<std::string> (dataStr.data(), dataStr.data() + dataStr.size());
  ASSERT_EQ(dataStrVec,std::vector<std::string> ( { "\"7\"", "\"?\"", "\"3\"", "\"?\"", "\"?\"", "\"?\"", "\"9\"", "\"8\"", "\"?\"", "\"6\"" }));

  warnLog = handler.getData("absent_id_var",dataStr, nbInd, paramStr);
  ASSERT_EQ(warnLog, "Data from the variable: absent_id_var has been requested but is absent from the provided data. Please check that all the necessary data is provided.\n");
}

TEST(DataHandlerJson, jsonReturnType)
{
  // Variable declaration
  std::string warnLog;
  std::string json_file_input  = "./data/utest/arg_list_utest.json";
  std::string json_file_output = "./data/utest/output_utest.json";

  // Json data reading
  json argument_list;
  std::ifstream ifs(json_file_input);
  ifs >> argument_list;
  json resGetData_lm     = argument_list["resGetData_lm"];
  double confidenceLevel = argument_list["confidenceLevel"];
  const json& mcStrategy = argument_list["mcStrategy"];
  int nbClass            = argument_list["nbClass"];

  // Handler
  mixt::DataHandlerJson handler(resGetData_lm) ;
  warnLog += handler.listData();
  handler.writeInfo();
  handler.writeDataMap();
  json type = handler.jsonReturnType();

  ASSERT_EQ(type["categorical1"],"Categorical_pjk");
  ASSERT_EQ(type["poisson2"],"Poisson_k");
  ASSERT_EQ(type["gaussian3"],"Gaussian_sjk");
  ASSERT_EQ(type["ordinal1"],"Ordinal");
}

TEST(DataExtractorJson, jsonReturnVal)
{
  mixt::Timer totalTimer("Total Run");

  // lists to export results
  json mcMixture;
  json mcVariable;
  // Variable declaration
  std::string warnLog;
  std::string json_file_input  = "./data/utest/arg_list_utest.json";
  std::string json_file_output = "./data/utest/output_utest.json";

  // Json data reading
  json argument_list;
  std::ifstream ifs(json_file_input);
  ifs >> argument_list;
  json resGetData_lm     = argument_list["resGetData_lm"];
  double confidenceLevel = argument_list["confidenceLevel"];
  const json& mcStrategy = argument_list["mcStrategy"];
  int nbClass            = argument_list["nbClass"];

  // Handler
  mixt::DataHandlerJson handler(resGetData_lm) ;
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
}

