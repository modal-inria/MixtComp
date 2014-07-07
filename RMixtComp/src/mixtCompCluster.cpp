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
#include "MixtComp/src/mixt_MixtComp.h"

// [[Rcpp::export]]
void mixtCompCluster(Rcpp::List rList, Rcpp::S4 mcClusters, int nbClusters)
{  
  // parse the S4 argument into input and output
  Rcpp::S4 mcStrategy = mcClusters.slot("strategy");
  Rcpp::S4 mcResults = mcClusters.slot("results");
  
  // map the data structure
  mixt::DataHandlerR handler;
  handler.readDataFromRList(rList);
  handler.writeInfo(std::cout);
  handler.writeDataMap();
  
  // create the mixture manager
  mixt::MixtureManager<mixt::DataHandlerR> manager(handler);

  // prepare the composer
  mixt::MixtureComposer composer(handler.nbSample(), handler.nbVariable(), nbClusters);
  mixt::MixtureComposer* p_composer(&composer);
  composer.createMixtures(manager);
  
  // instantiate the SEMStrategy
  STK::Clust::initType initMethod;
  std::string s_initMethod = mcStrategy.slot("initMethod");
  if      (s_initMethod == std::string("randomInit"      ))
    initMethod = STK::Clust::randomInit_     ;
  else if (s_initMethod == std::string("randomClassInit"))
    initMethod = STK::Clust::randomClassInit_;
  else if (s_initMethod == std::string("randomFuzzyInit"))
    initMethod = STK::Clust::randomFuzzyInit_;

  // create the appropriate strategy and transmit the parameters
  mixt::SemStrategy strategy(p_composer,
                             initMethod, // init type
                             3, // number of trials of the complete chain
                             mcStrategy.slot("nbTrialInInit"), // number of initialization trials
                             mcStrategy.slot("nbBurnInIter"), // number of burn-in iterations
                             mcStrategy.slot("nbIter"), // number of iterations
                             mcStrategy.slot("nbGibbsIter"), // number of iterations for Gibbs sampler
                             3, // minimal number of element per class
                             10); // number of sampling attempts for lowly populated classes

  // run the strategy
  if (strategy.run())
    mcResults.slot("runOK") = true;
  else
    mcResults.slot("runOK") = false;

  // output and export the results
  const mixt::DataHandlerR* p_handler(&handler);
  mixt::DataExtractorR dataExtractor(p_composer, p_handler);
  composer.writeParameters(std::cout);
  dataExtractor.extractVal();

  // export the composer results to R through modifications of mcResults
  mcResults.slot("nbCluster") = nbClusters;
  mcResults.slot("lnlikelihood") = composer.lnLikelihood();

  Rcpp::NumericVector proportions(nbClusters);
  for (int kS = 0, kR = 0; kR < nbClusters; ++kS, ++kR)
    proportions[kR] = composer.p_pk()->elt(kS);
  mcResults.slot("proportions") = proportions;

  Rcpp::NumericVector partition(handler.nbSample());
  for (int iS = 0, iR = 0; iR < handler.nbSample(); ++iS, ++iR)
    partition[iR] = composer.p_zi()->elt(iS);
  mcResults.slot("partition") = partition;

  Rcpp::NumericMatrix proba(handler.nbSample(), nbClusters);
  for (int iS = 0, iR = 0; iR < handler.nbSample(); ++iS, ++iR)
    for (int kS = 0, kR = 0; kR < nbClusters; ++kS, ++kR)
      proba(iR, kR) = composer.p_tik()->elt(iS, kS);
  mcResults.slot("proba") = proba;
}
