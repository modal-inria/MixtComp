/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 */

/*
 * Project:  MixtComp
 * created on: Nov 22, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_DataHandlerR.cpp
 *  @brief In this file 
 **/

#include "mixt_DataHandlerR.h"
#include "mixt_MixtureComposer.h"
#include "mixt_SemStrategy.h"

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
  STK::IMixtureComposerBase* p_composer = &composer;
  composer.setDataHandler(&handler);
  composer.createMixtCompMixtures();
  
  // instantiate the SemStrategy
  STK::Clust::initType initMethod;
  std::string s_initMethod = mcStrategy.slot("initMethod");
  if      (s_initMethod == std::string("randomInit"      ))
    initMethod = STK::Clust::randomInit_     ;
  else if (s_initMethod == std::string("randomClassInit"))
    initMethod = STK::Clust::randomClassInit_;
  else if (s_initMethod == std::string("randomFuzzyInit"))
    initMethod = STK::Clust::randomFuzzyInit_;
  
  // create the apropriate strategy and transmit the parameters
  mixt::SemStrategy strategy( p_composer
                            , initMethod // init type
                            , mcStrategy.slot("nbTrialInInit") // number of initialization trials
                            , mcStrategy.slot("nbBurnInIter") // number of burn-in iterations
                            , mcStrategy.slot("nbIter")
                            ); // number of iterations

  // run the strategy facade
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