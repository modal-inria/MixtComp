/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: Nov 22, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_DataHandlerR.h"
#include "mixt_DataExtractorR.h"
#include "mixt_ParamExtractorR.h"
#include "MixtComp/src/mixt_MixtComp.h"
#include "MixtComp/src/IO/mixt_ParamSetterDummy.h"
#include "MixtComp/src/Various/mixt_Def.h"

// [[Rcpp::export]]
Rcpp::List mixtCompCluster(Rcpp::List rList,
                           Rcpp::S4 mcClusters,
                           int nbClusters,
                           double confidenceLevel)
{
  // string to log warnings
  std::string warnLog;
  // parse the S4 argument into input and output
  Rcpp::S4 mcStrategy = mcClusters.slot("strategy");
  Rcpp::S4 mcResults = mcClusters.slot("results");
  
  // create the data handler
  mixt::DataHandlerR handler(rList);
  handler.listData();
  handler.writeInfo(std::cout);
  handler.writeDataMap();
  
  // create the data extractor
  mixt::DataExtractorR dataExtractor;

  // create the parameters setter
  mixt::ParamSetterDummy paramSetter;

  // create the parameters extractor
  mixt::ParamExtractorR paramExtractor;

  // create the mixture manager
  mixt::MixtureManager<mixt::DataHandlerR,
                       mixt::DataExtractorR,
                       mixt::ParamSetterDummy,
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

  manager.createMixtures(composer,
                         nbClusters);
  
  // create the appropriate strategy and transmit the parameters
  mixt::SemStrategy strategy(&composer,
                             mcStrategy.slot("nbTrialInInit"), // number of trials of the complete chain, with different initializations
                             mcStrategy.slot("nbBurnInIter"), // number of burn-in iterations
                             mcStrategy.slot("nbIter"), // number of iterations
                             mcStrategy.slot("nbGibbsBurnInIter"), // number of iterations for Gibbs sampler
                             mcStrategy.slot("nbGibbsIter"), // number of iterations for Gibbs sampler
                             10); // number of sampling attempts for lowly populated classes

  // run the strategy
  warnLog += strategy.run();
  mcResults.slot("warnLog") = warnLog;

  if (warnLog.size() == 0)
  {
    composer.writeParameters(std::cout);
    composer.exportDataParam();

    // export the composer results to R through modifications of mcResults
    mcResults.slot("nbCluster") = nbClusters;
    mcResults.slot("lnObservedLikelihood") = composer.lnLikelihood(mixt::lnObservedLikelihood_);
    mcResults.slot("lnCompletedLikelihood") = composer.lnLikelihood(mixt::lnCompletedLikelihood_);

    Rcpp::NumericVector proportions(nbClusters);
    for (int kS = 0, kR = 0; kR < nbClusters; ++kS, ++kR)
      proportions[kR] = composer.p_pk()->elt(kS);
    mcResults.slot("proportions") = proportions;

    Rcpp::NumericMatrix proportionsLog(composer.p_pkLog()->sizeRows(),
                                       composer.p_pkLog()->sizeCols());
    for (int i = 0; i < composer.p_pkLog()->sizeRows(); ++i)
    {
      for (int j = 0; j < composer.p_pkLog()->sizeCols(); ++j)
      {
        proportionsLog(i, j) = (*composer.p_pkLog())(i, j);
      }
    }
    mcResults.slot("proportionsLog") = proportionsLog;

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
  }
  else
  {
    std::cout << "!!! warnLog not empty !!!" << std::endl;
    std::cout << warnLog << std::endl;
  }

  Rcpp::List data = dataExtractor.rcppReturnVal();
  Rcpp::List param = paramExtractor.rcppReturnParam();

  return Rcpp::List::create(Rcpp::Named("data") = data,
                            Rcpp::Named("param") = param);
}
