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

// [[Rcpp::export]]
Rcpp::List mixtCompPredict(Rcpp::List dataList,
                           Rcpp::NumericVector prop,
                           Rcpp::List paramList,
                           Rcpp::S4 mcClusters,
                           int nbClusters,
                           double confidenceLevel)
{
  // string to log warnings, useless as of now, since Gibbs sampling can not fail, it lacks an mStep
  std::string warnLog;
  // parse the S4 argument into input and output
  Rcpp::S4 mcStrategy = mcClusters.slot("strategy");
  Rcpp::S4 mcResults = mcClusters.slot("results");
  
  // create the data handler
  mixt::DataHandlerR handler(dataList);
  handler.listData();
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

  manager.createMixtures(composer,
                         nbClusters);
  
  // create the appropriate strategy and transmit the parameters
  mixt::GibbsStrategy strategy(&composer,
                               mcStrategy.slot("nbGibbsBurnInIter"), // number of iterations for Gibbs sampler
                               mcStrategy.slot("nbGibbsIter")); // number of iterations for Gibbs sampler);

  // run the strategy
  strategy.run();

  composer.writeParameters(std::cout);
  composer.exportDataParam();

  // export the composer results to R through modifications of mcResults
  mcResults.slot("nbCluster") = nbClusters;
  mcResults.slot("nbFreeParameters") = composer.nbFreeParameters();
  mcResults.slot("lnObservedLikelihood") = composer.lnLikelihood(mixt::lnObservedLikelihood_);
  mcResults.slot("lnCompletedLikelihood") = composer.lnLikelihood(mixt::lnCompletedLikelihood_);

  Rcpp::NumericVector proportions(nbClusters);
  for (int kS = 0, kR = 0; kR < nbClusters; ++kS, ++kR)
    proportions[kR] = composer.p_pk()->elt(kS);
  mcResults.slot("proportions") = proportions;

  composer.mapStep(); // compute zi_ using maximum a posteriori instead of sampling it
  Rcpp::NumericVector partition(handler.nbSample());
  for (int iS = 0, iR = 0; iR < handler.nbSample(); ++iS, ++iR)
    partition[iR] = composer.p_zi()->elt(iS) + 1;
  mcResults.slot("partition") = partition;

  Rcpp::NumericMatrix proba(handler.nbSample(), nbClusters);
  for (int iS = 0, iR = 0; iR < handler.nbSample(); ++iS, ++iR)
    for (int kS = 0, kR = 0; kR < nbClusters; ++kS, ++kR)
      proba(iR, kR) = composer.p_tik()->elt(iS, kS);
  mcResults.slot("proba") = proba;

  mcResults.slot("nbFreeParameters") = composer.nbFreeParameters();

  mcResults.slot("warnLog") = warnLog;
  if (warnLog.size() > 0)
  {
    std::cout << "!!! warnLog not empty !!!" << std::endl;
    std::cout << warnLog << std::endl;
  }

  Rcpp::List data = dataExtractor.rcppReturnVal();
  Rcpp::List param = paramExtractor.rcppReturnParam();

  return Rcpp::List::create(Rcpp::Named("data") = data,
                            Rcpp::Named("param") = param);
}
