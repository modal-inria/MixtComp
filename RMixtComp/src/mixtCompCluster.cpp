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
int mixtCompCluster(Rcpp::List rList)
{
  mixt::DataHandlerR handler;
  if (!handler.readDataFromRList(rList)) return -1;
  handler.writeInfo(std::cout);
  handler.writeDataMap();
  
  STK::MixtureComposer composer(3);
  STK::IMixtureModelBase* p_composer = &composer;
  composer.setDataHandler(&handler);
  composer.createIngredients();
  composer.setData();
  composer.initializeModel();

  STK::StrategyFacade strategy(p_composer);
  strategy.createSemStrategy( STK::Clust::randomInit_ // init type
                            , 2 // number of initialization trials
                            , 20 // number of burn-in iterations
                            , 100 ); // number of iterations

  // run the facade
  strategy.run(); 

  // write the results
  composer.writeParameters(std::cout);
  return 0;
}