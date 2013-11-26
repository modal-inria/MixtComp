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
int mixtCompCluster(Rcpp::List rList, Rcpp::S4 RStrategy, int nbClusters)
{
  mixt::DataHandlerR handler;
  if (!handler.readDataFromRList(rList)) return -1;
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
  std::string s_initMethod = RStrategy.slot("initMethod");
  if      (s_initMethod == std::string("randomInit"      ))
    initMethod = STK::Clust::randomInit_     ;
  else if (s_initMethod == std::string("randomClassInit_"))
    initMethod = STK::Clust::randomClassInit_;
  else if (s_initMethod == std::string("randomFuzzyInit_"))
    initMethod = STK::Clust::randomFuzzyInit_;
  std::cout << s_initMethod << std::endl;
  
  strategy.createSemStrategy( initMethod // init type
                            , RStrategy.slot("nbTrialInInit") // number of initialization trials
                            , RStrategy.slot("nbBurnInIter") // number of burn-in iterations
                            , RStrategy.slot("nbIter")
                            ); // number of iterations

  // run the facade
  strategy.run(); 

  // write the results
  composer.writeParameters(std::cout);
  return 0;
}