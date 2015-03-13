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
#include "mixt_ParamSetterComposerR.h"
#include "MixtComp/src/mixt_MixtComp.h"
#include "MixtComp/src/Various/mixt_Def.h"
#include "MixtComp/src/Various/mixt_Timer.h"

// [[Rcpp::export]]
Rcpp::List mixtCompPredict(Rcpp::List dataList,
                           Rcpp::NumericVector prop,
                           Rcpp::List paramList,
                           Rcpp::List mcStrategy,
                           int nbClusters,
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
  handler.writeInfo(std::cout);
  handler.writeDataMap();
  
  // create the data extractor
  mixt::DataExtractorR dataExtractor;

  // create the parameters setter
  mixt::ParamSetterR paramSetter(paramList);

  // create the parameters extractor
  mixt::ParamExtractorR paramExtractor;

  // create the parameters setter for the composer
  mixt::ParamSetterComposerR paramSetterComposer(prop);

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

  // prepare the composer
  mixt::MixtureComposer composer(handler.nbSample(),
                                 handler.nbVariable(),
                                 nbClusters,
                                 confidenceLevel);
  composer.setProportions(paramSetterComposer.getProportions());

  if (confidenceLevel < 0. || 1. < confidenceLevel)
  {
    warnLog +=   std::string("confidenceLevel should be in the interval [0;1], but value input is: ")
               + mixt::type2str(confidenceLevel)
               + std::string(".\n");
  }

  if (warnLog.size() == 0) // data is correct in descriptors, proceed with reading
  {
    // create the mixtures, and read / set the data
    mixt::Timer readTimer("Read Data");
    warnLog += composer.setZi(handler);
    manager.createMixtures(composer,
                           nbClusters);
    readTimer.top("data has been read");
  }
  
  if (warnLog.size() == 0) // all data has been read, checked and transmitted to the mixtures
  {
    // create the appropriate strategy and transmit the parameters
    mixt::GibbsStrategy strategy(&composer,
                                 mcStrategy["nbGibbsBurnInIter"], // number of iterations for Gibbs sampler
                                 mcStrategy["nbGibbsIter"]); // number of iterations for Gibbs sampler);

    // run the strategy
    mixt::Timer stratTimer("Strategy Run");
    warnLog += strategy.run();
    stratTimer.top("strategy run complete");
  }

  if (warnLog.size() == 0) // all data has been read, checked and transmitted to the mixtures
  {
    composer.writeParameters(std::cout);
    composer.exportDataParam();
    composer.exportZi(dataExtractor);

    // export the composer results to R through modifications of mcResults
    mcMixture["nbCluster"] = nbClusters;
    mcMixture["nbFreeParameters"] = composer.nbFreeParameters();
    mixt::Real lnObsLik = composer.lnObservedLikelihood();
    mixt::Real lnCompLik = composer.lnCompletedLikelihood();
    mixt::Real lnSemiCompLik = composer.lnSemiCompletedLikelihood();
    mcMixture["lnObservedLikelihood"] = lnObsLik;
    mcMixture["lnSemiCompletedLikelihood"] = lnSemiCompLik;
    mcMixture["lnCompletedLikelihood"] = lnCompLik;
    mcMixture["BIC"] = lnObsLik      - 0.5 * composer.nbFreeParameters() * std::log(composer.nbSample());
    mcMixture["ICL"] = lnSemiCompLik - 0.5 * composer.nbFreeParameters() * std::log(composer.nbSample());

    Rcpp::NumericVector proportions(nbClusters);
    for (int kS = 0, kR = 0; kR < nbClusters; ++kS, ++kR)
      proportions[kR] = (*composer.p_pk())(kS);
    mcMixture["proportions"] = proportions;

    mcMixture["runTime"] = totalTimer.top("end of run");
    mcMixture["nbSample"] = composer.nbSample();
  }

  mcMixture["warnLog"] = warnLog;
  if (warnLog.size() != 0)
  {
    std::cout << "!!! warnLog not empty !!!" << std::endl;
    std::cout << warnLog << std::endl;
  }

  mcMixture["runTime"] = totalTimer.top("end of run");
  mcMixture["nbSample"] = composer.nbSample();

  Rcpp::List data = dataExtractor.rcppReturnVal();
  Rcpp::List param = paramExtractor.rcppReturnParam();
  mcVariable = Rcpp::List::create(Rcpp::Named("data") = data,
                                  Rcpp::Named("param") = param);


  return Rcpp::List::create(Rcpp::Named("strategy") = mcStrategy,
                            Rcpp::Named("mixture") = mcMixture,
                            Rcpp::Named("variable") = mcVariable);
}
