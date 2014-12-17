/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::tests
 * created on: 8 août 2011
 * Purpose:  test the Normal and MultiNormal classes.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file testJointModels.cpp
 *  @brief In this file we test the joint Statistical models.
 **/

#include "StrategiesUtil.h"

namespace STK
{

void testSimpleStrategy( IMixtureComposer*& p_composer)
{
  StrategyFacade facade(p_composer);
  facade.createSimpleStrategy( Clust::randomInit_, 3, Clust::semAlgo_, 20, 1e-04
                             , 5, Clust::emAlgo_, 500, 1e-08);
  facade.run();
  stk_cout << _T("\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ SimpleStrategy terminated. Results:               +\n");
  p_composer->writeParameters(stk_cout);
}

void testFullStrategy( IMixtureComposer*& p_composer
                     , Clust::initType init, int nbTryInInit, Clust::algoType initAlgo
                     , int nbTry, int nbInitRun, int nbShortRun
                     , Clust::algoType shortAlgo
                     , Clust::algoType longAlgo)
{
  StrategyFacade facade(p_composer);
  facade.createFullStrategy( init, nbTryInInit, initAlgo, 20, 1e-02
                           , nbTry, nbInitRun, nbShortRun
                           , shortAlgo, 100, 1e-04
                           , longAlgo, 500, 1e-08);
  stk_cout << _T("\n")
           << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  if (!facade.run())
  { stk_cout << _T("+ FullStrategy terminated without success+\n");}
  else
  { stk_cout << _T("+ FullStrategy terminated with success+\n");}
  stk_cout << _T("Computed lnLikelihood =") <<p_composer->computeLnLikelihood() << _T("\n");
  p_composer->writeParameters(stk_cout);
  stk_cout << _T("\n\n");
}

} // namespace STK
