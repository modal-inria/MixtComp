/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Nov 11, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_DataHandlerR.h"
#include "mixt_DataExtractorR.h"
#include "mixt_ParamExtractorR.h"
#include "mixt_ParamSetterR.h"
#include "mixt_Function.h"
#include "MixtComp/src/mixt_MixtComp.h"

// [[Rcpp::export]]
Rcpp::List mixtCompPredict(Rcpp::List dataList,
                           Rcpp::List paramList,
                           Rcpp::List mcStrategy,
                           int nbClass,
                           double confidenceLevel)
{
  mixt::Timer totalTimer("Total Run");

  // string to log warnings
  std::string warnLog;

  // lists to export results
  Rcpp::List mcMixture;
  Rcpp::List mcVariable;

  // create the data handler
  mixt::DataHandlerR handler(dataList);
  warnLog += handler.listData();
  handler.writeInfo();
  handler.writeDataMap();

  // create the data extractor
  mixt::DataExtractorR dataExtractor;

  // create the parameters setter
  mixt::ParamSetterR paramSetter(paramList);

  // create the parameters extractor
  mixt::ParamExtractorR paramExtractor;

  // create the mixture manager
  mixt::MixtureManager<mixt::DataHandlerR,
                       mixt::DataExtractorR,
                       mixt::ParamSetterR,
                       mixt::ParamExtractorR> manager(&handler,
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
    warnLog += composer.setDataParam<mixt::ParamSetterR,
                                     mixt::DataHandlerR>(paramSetter,
                                                         handler,
                                                         mixt::prediction_);
    readTimer.top("data has been read");

    if (warnLog.size() == 0) { // all data has been read, checked and transmitted to the mixtures
      dataExtractor .setNbMixture(handler.nbVariable());
      paramExtractor.setNbMixture(handler.nbVariable());

      mixt::StrategyParam param;
      paramRToCpp(mcStrategy,
                  param);

      // create the appropriate strategy and transmit the parameters
      mixt::GibbsStrategy strategy(&composer,
                                   param); // number of iterations for Gibbs sampler);

      // run the strategy
      mixt::Timer stratTimer("Strategy Run");
      warnLog += strategy.run();
      stratTimer.top("strategy run complete");

      if (warnLog.size() == 0) { // all data has been read, checked and transmitted to the mixtures
#ifdef MC_VERBOSE
        composer.writeParameters();
#endif
        composer.exportDataParam<mixt::DataExtractorR,
                                 mixt::ParamExtractorR>(dataExtractor,
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
        mcMixture["mode"] = "predict";

        Rcpp::NumericMatrix idc;
        mixt::IDClass(composer, idc);
        mcMixture["IDClass"] = idc;

        Rcpp::NumericMatrix pGC;
        mixt::lnProbaGivenClass(composer, pGC);
        mcMixture["lnProbaGivenClass"] = pGC;
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

  Rcpp::List type = handler.rcppReturnType();
  Rcpp::List data = dataExtractor.rcppReturnVal();
  Rcpp::List param = paramExtractor.rcppReturnParam();

  mcVariable = Rcpp::List::create(Rcpp::Named("type") = type,
                                  Rcpp::Named("data") = data,
                                  Rcpp::Named("param") = param);


  return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy,
                            Rcpp::Named("mixture") = mcMixture,
                            Rcpp::Named("variable") = mcVariable);
}
