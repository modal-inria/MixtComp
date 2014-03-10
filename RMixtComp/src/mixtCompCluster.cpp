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

#include "MixtComp/src/Data/mixt_DataHandlerR.h"
#include "MixtComp/src/Composer/mixt_MixtureComposer.h"
#include "MixtComp/src/Strategy/mixt_SEMStrategy.h"

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
  
  // prepare the composer
  mixt::MixtureComposer composer(nbClusters);
  mixt::MixtureComposer* p_composer(&composer);
  composer.setDataHandler(&handler);
  composer.createMixtCompMixtures();
  
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
                             3, // minimal number of element per class
                             10); // number of sampling attempts for lowly populated classes

  // run the strategy
  strategy.run(); 

  // output the results
  composer.writeParameters(std::cout);
  
  // export the composer results to R through modifications of mcResults
  mcResults.slot("nbCluster") = nbClusters;
  mcResults.slot("lnlikelihood") = composer.lnLikelihood();
  
  Rcpp::NumericVector proportions(nbClusters);
  for (int kS = composer.p_prop()->firstIdxRows(), kR = 0; kR < nbClusters; ++kS, ++kR)
    proportions[kR] = composer.p_prop()->elt(kS);
  mcResults.slot("proportions") = proportions;
  
  Rcpp::NumericVector partition(handler.nbSample());
  for (int iS = composer.p_zi()->firstIdxRows(), iR = 0; iR < handler.nbSample(); ++iS, ++iR)
    partition[iR] = composer.p_zi()->elt(iS);
  mcResults.slot("partition") = partition;
  
  Rcpp::NumericMatrix proba(handler.nbSample(), nbClusters);
  for (int iS = composer.p_tik()->firstIdxRows(), iR = 0; iR < handler.nbSample(); ++iS, ++iR)
    for (int kS = composer.p_tik()->firstIdxRows(), kR = 0; kR < nbClusters; ++kS, ++kR)
      proba(iR, kR) = composer.p_tik()->elt(iS, kS);
  mcResults.slot("proba") = proba;
}
