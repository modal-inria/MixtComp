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
/*  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Create composer and setDataHandler                +\n");
  STK::MixtureComposer composer(3);
  composer.setDataHandler(&handler);
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ CreateIngredients and setData                     +\n");
  composer.createIngredients();
  composer.setData();
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ initializeModel                                   +\n");
  composer.initializeModel();
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ composer                                          +\n");
  composer.writeParameters(stk_cout);
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ randomClassInit                                   +\n");
  composer.randomClassInit();
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ composer                                          +\n");
  composer.writeParameters(stk_cout);
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ 20 sem iterations                                  +\n");
  for (int i=0; i< 20; i++)
  {
    composer.mStep();
    composer.sStep();
    composer.eStep();
    stk_cout << _T("i= ") << i
             << _T(", lnLikelihood =") << composer.lnLikelihood() << _T("\n");
  }
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ 50 em iterations                                  +\n");
  for (int i=0; i< 50; i++)
  {
    composer.mStep();
    composer.eStep();
    stk_cout << _T("i= ") << i
             << _T(", lnLikelihood =") << composer.lnLikelihood() << _T("\n");
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ composer                                          +\n");
  composer.writeParameters(stk_cout);
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ End of test composer : no error detected          +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0; */
}