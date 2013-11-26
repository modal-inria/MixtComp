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
#include "stkpp/include/STKpp.h"

// [[Rcpp::export]]
void mixtCompCluster(Rcpp::List rList, Rcpp::S4 mcClusters, int nbClusters)
{
  Rcpp::S4 mcStrategy = mcClusters.slot("strategy");
  std::cout << "top" << std::endl;
  Rcpp::S4 mcResults = mcClusters.slot("results");
  
  mixt::DataHandlerR handler;
  handler.readDataFromRList(rList);
  handler.writeInfo(std::cout);
  handler.writeDataMap();
  
  STK::MixtureComposer composer(nbClusters);
  STK::IMixtureModelBase* p_composer = &composer;
  composer.setDataHandler(&handler);
  composer.createIngredients();
  composer.setData();
  composer.initializeModel();

  STK::StrategyFacade strategy(p_composer);
  
  STK::Clust::initType initMethod;
  std::string s_initMethod = mcStrategy.slot("initMethod");
  if      (s_initMethod == std::string("randomInit"      ))
    initMethod = STK::Clust::randomInit_     ;
  else if (s_initMethod == std::string("randomClassInit_"))
    initMethod = STK::Clust::randomClassInit_;
  else if (s_initMethod == std::string("randomFuzzyInit_"))
    initMethod = STK::Clust::randomFuzzyInit_;
  std::cout << s_initMethod << std::endl;
  
  strategy.createSemStrategy( initMethod // init type
                            , mcStrategy.slot("nbTrialInInit") // number of initialization trials
                            , mcStrategy.slot("nbBurnInIter") // number of burn-in iterations
                            , mcStrategy.slot("nbIter")
                            ); // number of iterations

  // run the facade
  strategy.run(); 

  // write the results
  composer.writeParameters(std::cout);
  
  // export the results  
  mcResults.slot("nbCluster") = nbClusters;
  mcResults.slot("lnlikelihood") = composer.lnLikelihood();
  
  Rcpp::NumericVector proportions(nbClusters);
  for (int k = 0; k < nbClusters; ++k)
    proportions[k] = composer.p_prop()->elt(k+1);
  mcResults.slot("proportions") = proportions;
  
  Rcpp::NumericVector partition(handler.nbSample());
  for (int i = 0; i < handler.nbSample(); ++i)
    partition[i] = composer.p_zi()->elt(i+1);
  mcResults.slot("partition") = partition;
  
  Rcpp::NumericMatrix proba(handler.nbSample(), nbClusters);
  for (int i = 0; i < handler.nbSample(); ++i)
    for (int k = 0; k < nbClusters; ++k)
      proba(i, k) = composer.p_tik()->elt(i+1, k+1);
  mcResults.slot("proba") = proba;
}